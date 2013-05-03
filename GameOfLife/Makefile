CC=g++
CCWIN=/usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/bin/i586-mingw32-g++
EMCC=emcc 
OBJS=main.cpp
OUTPUT=bin/GameofLife
SDL_CFLAGS := $(shell pkg-config --cflags sdl SDL_image SDL_gfx)
SDL_LIBS := $(shell pkg-config --libs sdl SDL_image SDL_gfx)
SDL_MINGW_CFLAGS=`/opt/SDL-1.2.15/bin/i586-mingw32msvc-sdl-config --cflags`
SDL_MINGW_LIBS=`/opt/SDL-1.2.15/bin/i586-mingw32msvc-sdl-config --libs` -lSDL_image -lSDL_ttf -lSDL_mixer
PRELOADED=creep.png

OSX:
	$(CC) -o $(OUTPUT) $(OBJS) $(SDL_CFLAGS) $(SDL_LIBS)

Win:
	$(CCWIN) -o $(OUTPUT).exe $(OBJS) $(SDL_MINGW_CFLAGS) $(SDL_MINGW_LIBS)

Web:
	$(EMCC) $(OBJS) -o public/index.html --preload-file $(PRELOADED)
