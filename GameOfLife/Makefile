CC=$(shell which clang++)
EMCC=emcc

WINTOOLS=/usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/bin

SRC=$(wildcard src/**/*.cpp src/*.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRC))

BIN=bin/
OUTPUT=GameOfLife

ASSET_DIR=assets/
IMG_DIR=img/
ICON_DIR=icn/

PRELOADED=--preload-file $(ASSET_DIR)$(IMG_DIR)creep.png --preload-file $(ASSET_DIR)$(IMG_DIR)bglight.png --preload-file $(ASSET_DIR)$(IMG_DIR)bgdark.png
PREJS=pre.js

CPPFLAGS += $(shell pkg-config --cflags sdl SDL_image SDL_gfx)
LDFLAGS += $(shell pkg-config --libs sdl SDL_image SDL_gfx)

SDL_MINGW_CPPFLAGS=-I/usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/include/SDL
SDL_MINGW_LDFLAGS=-L/usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/lib/SDL -lmingw32 -lSDLmain -lSDL -lSDL_image
SDL_MINGW_DLLS=/usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/dll/

OSX: $(SRC)
	$(CC) -o $(BIN)$(OUTPUT) $(SRC) $(CPPFLAGS) $(LDFLAGS)

OSX_APP:
	mkdir -p $(BIN)$(OUTPUT).app
	mkdir -p $(BIN)$(OUTPUT).app/Contents/Frameworks
	mkdir -p $(BIN)$(OUTPUT).app/Contents/MacOS
	mkdir -p $(BIN)$(OUTPUT).app/Contents/Resources/img
	mkdir -p $(BIN)$(OUTPUT).app/Contents/PlugIns
	mkdir -p $(BIN)$(OUTPUT).app/Contents/SharedFrameWorks
	cp Info.plist $(BIN)$(OUTPUT).app/Contents/Info.plist
	cp $(ASSET_DIR)$(IMG_DIR)creep.png $(BIN)$(OUTPUT).app/Contents/Resources/img/creep.png
	cp $(ASSET_DIR)$(IMG_DIR)bglight.png $(BIN)$(OUTPUT).app/Contents/Resources/img/bglight.png
	cp $(ASSET_DIR)$(IMG_DIR)bgdark.png $(BIN)$(OUTPUT).app/Contents/Resources/img/bgdark.png
	cp $(ASSET_DIR)$(ICON_DIR)creep.icns $(BIN)$(OUTPUT).app/Contents/Resources/creep.icns
	touch $(BIN)$(OUTPUT).app/Contents/PkgInfo
	$(CC) -o $(BIN)$(OUTPUT).app/Contents/MacOS/$(OUTPUT) $(SRC) $(CPPFLAGS) $(LDFLAGS) -DAPP

WIN:
	mkdir -p $(BIN)win
	$(WINTOOLS)/i586-mingw32-g++ -o $(BIN)win/$(OUTPUT).exe $(SRC) $(SDL_MINGW_CPPFLAGS) $(SDL_MINGW_LDFLAGS)
	cp $(SDL_MINGW_DLLS)SDL.dll $(BIN)win/SDL.dll
	cp $(SDL_MINGW_DLLS)SDL_image.dll $(BIN)win/SDL_image.dll
	cp $(SDL_MINGW_DLLS)libpng15-15.dll $(BIN)win/libpng15-15.dll
	cp $(SDL_MINGW_DLLS)zlib1.dll $(BIN)win/zlib1.dll
	cp $(ASSET_DIR)$(IMG_DIR)creep.png $(BIN)win/creep.png
	cp $(ASSET_DIR)$(IMG_DIR)bglight.png $(BIN)win/bglight.png
	cp $(ASSET_DIR)$(IMG_DIR)bgdark.png $(BIN)win/bgdark.png

HTML: $(SRC)
	$(EMCC) $(SRC) --preload-file $(PRELOADED) -o public/$(OUTPUT).html

JS: $(SRC) js/pre.js
	$(EMCC) $(SRC) --pre-js js/$(PREJS) $(PRELOADED) -o public/$(OUTPUT).js

#clean: