ifdef CROSS
override CROSS+=-
endif
CXX=$(CROSS)g++
RC=$(CROSS)windres

ifdef OLDGCC
override CXX += -std=c++0x
else
override CXX += -std=c++11
endif

LINK = $(CXX)
CFLAGS = -O3 -Iinclude -DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN -DWINVER=0x0501 -D_WIN32_WINNT=0x0501 -march=i686 -fpermissive
RCFLAGS = -Iinclude
LDFLAGS = -s -static -static-libstdc++ -mwindows
LIBS=-lgdi32 -lole32 -lcomctl32 -luuid -lcomdlg32

FILES=build/Keyboard.o build/MainWindow.o build/Window.o \
      build/PianoControl.o build/midifile.o build/resources.o
FILES=build/Zalgo.o build/MainWindow.o build/Window.o \
      build/DropTarget.o build/MainLogic.o build/MainLoadSave.o \
      build/PreviewWindow.o build/NLSWrap.o build/MyDropTarget.o \
      build/Zalgores.o 
all: initdir Zalgo.exe

initdir:
	[ -d build ] || mkdir build

Zalgo.exe: $(FILES)
	$(LINK) $(CFLAGS) $(LDFLAGS) $(FILES) -o $@ $(LIBS)

include/MainWindow.hpp: include/Window.hpp include/PreviewWindow.hpp
include/PreviewWindow.hpp: include/Window.hpp
include/MyDropTarget.h: include/DropTarget.h

build/Zalgo.o: src/Zalgo.cpp include/MainWindow.hpp
build/MainWindow.o: src/MainWindow.cpp include/MainWindow.hpp
build/MainLoadSave.o: src/MainLoadSave.cpp include/MainWindow.hpp
build/MainLogic.o: src/MainLogic.cpp include/MainWindow.hpp include/NLSWrap.hpp include/ConversionData.inc
build/Window.o: src/Window.cpp include/Window.hpp
build/DropTarget.o: src/DropTarget.cpp include/DropTarget.hpp
build/DropTarget.o: src/MyDropTarget.cpp include/MyDropTarget.hpp
build/PreviewWindow.o: src/PreviewWindow.cpp include/PreviewWindow.hpp
build/NLSWrap.o: src/NLSWrap.cpp include/NLSWrap.hpp

build/ZalgoLib.o: src/ZalgoLib.cpp include/ZalgoLib.h include/ConversionData.inc
build/OptionParser.o: src/OptionParser.cpp include/OptionParser.h
build/hecomes.o: src/hecomes.cpp include/OptionParser.h include/ZalgoLib.h
build/hegoes.o: src/hegoes.cpp include/OptionParser.h include/ZalgoLib.h

build/Zalgores.o: Zalgo.rc include/resource.h commctrl6.manifest
	$(RC) $(RCFLAGS) $< -o $@

build/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*.o
	rm -f Zalgo.exe
