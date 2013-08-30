SRCDIR=src
INCDIR=include

CXX=cl /nologo
LD=link /nologo
CXXFLAGS=/c /O1 /I$(INCDIR) /W4 /Zi /DWIN32_LEAN_AND_MEAN /DWINVER=0x0501 /D_WIN32_WINNT=0x0501 /wd4100 /DUNICODE /D_UNICODE
LDFLAGS=/subsystem:windows /debug /manifest /incremental:no /opt:REF
LDFLAGS=$(LDFLAGS) "/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'"
RC=rc /nologo
RCFLAGS=/i$(INCDIR)
LIBS=

!IFDEF DEBUG
BUILD=Debug
CXXFLAGS=$(CXXFLAGS) /DDEBUG
LDFLAGS=$(LDFLAGS)
!ELSE
BUILD=Release
LDFLAGS=$(LDFLAGS)
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

all: initdir $(DISTDIR)\Zalgo.exe

initdir:
	if not exist build md build
	if not exist $(OUTDIR) md $(OUTDIR)
	if not exist build md dist
	if not exist $(DISTDIR) md $(DISTDIR)

compress: $(DISTDIR)\Zalgo.exe
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

$(OUTDIR)\Zalgo.res: Zalgo.rc res\x-sampa.txt res\init.txt
	$(RC) $(RCFLAGS) /fo$@ Zalgo.rc

{$(SRCDIR)}.cpp{$(OUTDIR)}.obj::
	$(CXX) $(CXXFLAGS) /Fo$(OUTDIR)\ /Fd$(OUTDIR)\ $<

$(DISTDIR)\Zalgo.exe: $(FILES)
	$(LD) /out:$@ $(LDFLAGS) $** $(LIBS)
	mt.exe -nologo -manifest $@.manifest -outputresource:$@;1 && del $@.manifest || set ERRORLEVEL=0
