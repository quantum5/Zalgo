SRCDIR=src
INCDIR=include

CXX=cl /nologo
LD=link /nologo
CXXFLAGS=/c /O1 /I$(INCDIR) /W4 /DWIN32_LEAN_AND_MEAN /DWINVER=0x0501 /D_WIN32_WINNT=0x0501 /wd4100 /DUNICODE /D_UNICODE /EHsc
LDFLAGS=/subsystem:windows /incremental:no /opt:REF
RC=rc /nologo
RCFLAGS=/i$(INCDIR)
LIBS=

!IFDEF DEBUG
BUILD=Debug
CXXFLAGS=$(CXXFLAGS) /DDEBUG
!ELSE
BUILD=Release
!ENDIF

!IFNDEF NOPDB
CXXFLAGS=$(CXXFLAGS) /Zi
LDFLAGS=$(LDFLAGS) /debug
!ENDIF

OUTDIR=build\$(BUILD)
DISTDIR=dist\$(BUILD)
FILES=$(OUTDIR)\Zalgo.obj \
      $(OUTDIR)\MainWindow.obj \
      $(OUTDIR)\Window.obj \
      $(OUTDIR)\DropTarget.obj \
      $(OUTDIR)\MainLogic.obj \
      $(OUTDIR)\MainLoadSave.obj \
      $(OUTDIR)\PreviewWindow.obj \
      $(OUTDIR)\NLSWrap.obj \
      $(OUTDIR)\MyDropTarget.obj \
      $(OUTDIR)\Zalgo.res
DISTS=$(DISTDIR)\Zalgo.exe \
      $(DISTDIR)\hecomes.exe \
      $(DISTDIR)\hegoes.exe

CMDFILE=$(OUTDIR)\ZalgoLib.obj \
        $(OUTDIR)\OptionParser.obj

all: initdir $(DISTS)
hecomes: $(DISTDIR)\hecomes.exe
hecomes: $(DISTDIR)\hegoes.exe

initdir:
	@if not exist build md build
	@if not exist $(OUTDIR) md $(OUTDIR)
	@if not exist build md dist
	@if not exist $(DISTDIR) md $(DISTDIR)

compress: $(DISTS)
	upx --best $**

$(INCDIR)\MainWindow.hpp: $(INCDIR)\Window.hpp $(INCDIR)\MyDropTarget.hpp $(INCDIR)\PreviewWindow.hpp
$(INCDIR)\PreviewWindow.hpp: $(INCDIR)\Window.hpp
$(INCDIR)\MyDropTarget.hpp: $(INCDIR)\DropTarget.hpp

$(SRCDIR)\MainWindow.cpp: $(INCDIR)\MainWindow.hpp $(INCDIR)\PreviewWindow.hpp
$(SRCDIR)\MainLoadSave.cpp: $(INCDIR)\MainWindow.hpp
$(SRCDIR)\DropTarget.cpp: $(INCDIR)\DropTarget.hpp
$(SRCDIR)\MyDropTarget.cpp: $(INCDIR)\MyDropTarget.hpp
$(SRCDIR)\Zalgo.cpp: $(INCDIR)\MainWindow.hpp
$(SRCDIR)\PreviewWindow.cpp: $(INCDIR)\PreviewWindow.hpp
$(SRCDIR)\Window.cpp: $(INCDIR)\Window.hpp
$(SRCDIR)\NLSWrap.cpp: $(INCDIR)\NLSWrap.hpp
$(SRCDIR)\MainLogic.cpp: $(INCDIR)\MainWindow.hpp $(INCDIR)\NLSWrap.hpp $(INCDIR)\ConversionData.inc

$(SRCDIR)\ZalgoLib.cpp: $(INCDIR)\ZalgoLib.h $(INCDIR)\ConversionData.inc
$(SRCDIR)\OptionParser.cpp: $(INCDIR)\OptionParser.h
$(SRCDIR)\hecomes.cpp: $(INCDIR)\OptionParser.h $(INCDIR)\ZalgoLib.h
$(SRCDIR)\hegoes.cpp: $(INCDIR)\OptionParser.h $(INCDIR)\ZalgoLib.h

$(OUTDIR)\Zalgo.res: Zalgo.rc res\x-sampa.txt res\init.txt commctrl6.manifest
	$(RC) $(RCFLAGS) /fo$@ Zalgo.rc

{$(SRCDIR)}.cpp{$(OUTDIR)}.obj::
	$(CXX) $(CXXFLAGS) /Fo$(OUTDIR)\ /Fd$(OUTDIR)\ $<

$(DISTDIR)\Zalgo.exe: $(FILES)
	$(LD) /out:$@ $(LDFLAGS) $** $(LIBS)

$(DISTDIR)\hecomes.exe: $(CMDFILE) $(OUTDIR)\hecomes.obj
	$(LD) /out:$@ $**

$(DISTDIR)\hegoes.exe: $(CMDFILE) $(OUTDIR)\hegoes.obj
	$(LD) /out:$@ $**
