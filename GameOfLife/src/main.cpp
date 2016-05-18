//
//  main.c
//  GameOfLife
//
//  Created by Fielding Johnston on 3/11/13.
//
//

#include <iostream>
#include <time.h>

#include "SDL.h"
#include "SDL_image.h"

#ifndef __MINGW32__
#include "SDL/SDL_rotozoom.h"
#else
#include "Windows.h"
#endif

#ifdef EMSCRIPTEN
#include "emscripten.h"
#include "SDL_rotozoom.h"
#endif 

using namespace std;

#define DEBUGINFO 0

#define CELL_SIZE 8         // length and width (in pixels) for the square cells
#define BOARD_SIZE 800     // Length and width for the square viewing area
#define SCREEN_BPP 32       // Screen bits per-pixels



#ifdef APP
#define IMG_CELL "GameOfLife.app/Contents/Resources/img/pink.png"
#define BG_LIGHT "GameOfLife.app/Contents/Resources/img/bglight.png"
#define BG_DARK "GameOfLife.app/Contents/Resources/img/bgdark.png"
#elif __MINGW32__
#define IMG_CELL "pink.png"
#define BG_LIGHT "bglight.png"
#define BG_DARK "bgdark.png"
#else
#define IMG_CELL "assets/img/pink.png"
#define BG_LIGHT "assets/img/bglight.png"
#define BG_DARK "assets/img/bgdark.png"
#endif

bool running = true;

int prevGrid[BOARD_SIZE / CELL_SIZE][BOARD_SIZE / CELL_SIZE];
int grid[BOARD_SIZE / CELL_SIZE][BOARD_SIZE / CELL_SIZE];       // Create a grid based on total board size and each cell size
int bufferGrid[BOARD_SIZE / CELL_SIZE][BOARD_SIZE / CELL_SIZE];

void init();

void handleEvents();
void update();
void draw();
void filldraw();

void mainloop(); // main loop wrapper for Emscripten

void spawn();
int checkNeighbors( int x, int y );

void fillCell( Sint16 x, Sint16 y, Uint16 w, Uint16 h, Uint32 color );
SDL_Surface* loadSurface(std::string path);

// SDL Objects
SDL_Surface *screen = NULL;
SDL_Surface *image = NULL;
SDL_Surface *scaledImage = NULL;
SDL_Surface *bgLight = NULL;
SDL_Surface *bgDark = NULL;

SDL_Event event;

int main ( int argc, char **argv )
{
  init();
  
  #ifdef EMSCRIPTEN
    emscripten_set_main_loop(mainloop, 10, 1);
    SDL_FreeSurface( image );
  #else
    while ( running )
    {
      mainloop();
      SDL_Delay( 500 );   // wait .5 seconds
    }

    SDL_FreeSurface( scaledImage );
  #endif

  SDL_Quit();
  return 0;
}

void init()
{
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
  {
    printf( "SDL_Init: %s\n", SDL_GetError() );
  }
  
  screen = SDL_SetVideoMode( BOARD_SIZE, BOARD_SIZE, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE);
        
  if ( screen == NULL )
  {
    printf( "SDL_SetVideoMode: %s\n", SDL_GetError() );
  }
  
  SDL_WM_SetCaption( "Conway's Game of Life", NULL);

  image = loadSurface(IMG_CELL);
  bgLight = loadSurface(BG_LIGHT);
  bgDark = loadSurface(BG_DARK);
    
  if ( !image )
  {
    printf( "IMG_Load: %s\n", IMG_GetError() );
  }
  else
  {

#ifdef EMSCRIPTEN
     scaledImage = image;
#elif __MINGW32__
    scaledImage = image;
#else
      scaledImage = rotozoomSurface(image, 0, ( float( CELL_SIZE ) / 16), 0);   // Scale cell image to cell size (16 is based on original png being 16px)
      SDL_FreeSurface( image );   // Finished with image, can free it back up
#endif 


  }

  spawn();    // spawn initial cell data
  draw();     // draw the initial board

}

void mainloop()
{
    handleEvents();
    update();
    draw();
}

void handleEvents()
{
    while ( SDL_PollEvent( &event ) )
    {
      if ( event.type == SDL_KEYDOWN )   // If a Key was pressed
      {
        switch ( event.key.keysym.sym )
        {
          case SDLK_ESCAPE:   // If key pressed was escape
            running = false;      // exit the program
            break;
          case SDLK_UP:       // If key pressed was up arrow
            break;
          case SDLK_DOWN:     // If key pressed was down arrow
            break;
          default:            // default for undefined keys
            break;
        }
      } else if ( event.type == SDL_QUIT )
      {
        running = false;    // exit the program
      }
    }
}

void update()
{
    // Rules:
    // 1. Any live cell with fewer than two live neighbours dies, as if caused by under-population.
    // 2. Any live cell with two or three live neighbours lives on to the next generation.
    // 3. Any live cell with more than three live neighbours dies, as if by overcrowding.
    // 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
  
  
    for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
    {
        for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
        {
            int neighbors = checkNeighbors( x, y );
            
            if ( neighbors < 2 && grid[x][y] == 1 )
            {
                if( DEBUGINFO ){ cout<<x<<","<<y<<" has died from under-population!\n"; }
                bufferGrid[x][y] = 0;
            } else if ( (neighbors == 2 || neighbors == 3) && grid[x][y] == 1)
            {
                if( DEBUGINFO ){ cout<<x<<","<<y<<" has a healthy amount of neighbors and lives on!\n"; }
                bufferGrid[x][y] = 1;
            } else if ( neighbors > 3 && grid[x][y] == 1)
            {
                if( DEBUGINFO ){ cout<<x<<","<<y<<" has died from overcrowding!\n"; }
                bufferGrid[x][y] = 0;
                
            } else if ( neighbors == 3 && grid[x][y] == 0)
            {
                if( DEBUGINFO ){ cout<<"A new cell has been born at "<<x<<","<<y<<"!\n"; }
                bufferGrid[x][y] = 1;
            }
        }
    }
    
    for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
    {
        for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
        {
            prevGrid[x][y] = grid[x][y];
            grid[x][y] = bufferGrid[x][y];
        }
    }
}

void draw()
{
  for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
  {
    for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
    {
      SDL_Rect offset;
      offset.x = x * CELL_SIZE;
      offset.y = y * CELL_SIZE;
      
      if ( grid[x][y] == 1)
      {
        SDL_BlitSurface( scaledImage, NULL, screen, &offset );
      }
      else
      {
        if ( y % 2 != 0 && x % 2 == 0 )
          SDL_BlitSurface( bgDark, NULL, screen, &offset );
        else
          SDL_BlitSurface( bgLight, NULL, screen, &offset );
      }
    }
  }
  SDL_Flip( screen );
}

void filldraw()
{
  for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
  {
    for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
    {
      if ( grid[x][y] == 1 )
      {
        fillCell(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, SDL_MapRGB(screen->format,255,0,0));
      }
      else
      {
        fillCell(x * CELL_SIZE, y * CELL_SIZE , CELL_SIZE, CELL_SIZE, SDL_MapRGB(screen->format,0,0,0));
      }
    }
  }
  SDL_Flip(screen);
}

void spawn()
{
  // fill the array with random cells
  int rando = 0;
//#ifndef __MINGW32__
  srand(( unsigned )time( NULL ));
//#else
//  srand( ( unsigned ) GetSystemTime() );
//#endif
    
  for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
  {
    for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
    {
      rando = rand() % 2;
      grid[x][y] = rando;
      if ( DEBUGINFO ){ cout<<"Setting square at "<<x<<","<<y<<" to "<<grid[x][y]<<"\n"; }
    }
  }
}

int checkNeighbors( int x, int y )
{
    int ncount = 0;
    if ( DEBUGINFO ){ cout<<"Checking neighbor cells for " << x << "," << y << ".\n"; }
    for ( int a = x - 1; a <= x + 1; a++ )
    {
        for ( int b = y - 1; b <= y + 1; b++)
        {
            if ( grid[a][b] == 1 && a >= 0 && a <= ( BOARD_SIZE / CELL_SIZE - 1 ) && b >= 0 && b <= ( BOARD_SIZE / CELL_SIZE - 1 ) )
            {
                ncount++;
            }
        }
    }
    
    if ( grid[x][y] ) { ncount--; } // subtract self from neighbor count if currently living
    if ( DEBUGINFO ){ cout<<x<<","<<y<<" has "<< ncount << " current neighbor cells.\n"; }
    
    return ncount;
}

void fillCell( Sint16 x, Sint16 y, Uint16 w, Uint16 h, Uint32 color )
{
    SDL_Rect rect = { x,y,w,h };
    SDL_FillRect( screen, &rect, color );
}

SDL_Surface* loadSurface( std::string path )
{
  //The final optimized image
  SDL_Surface* optimizedSurface = NULL;
  
  //Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
  if( loadedSurface == NULL )
  {
    printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
  }
  else
  {
    //Convert surface to screen format
    optimizedSurface = SDL_ConvertSurface( loadedSurface, screen->format, NULL );
    if( optimizedSurface == NULL )
    {
      printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }
    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );
  }
  return optimizedSurface;
}