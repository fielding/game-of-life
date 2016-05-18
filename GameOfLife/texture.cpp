//
//  texture.cpp
//  GameOfLife
//
//  Created by Fielding Johnston on 5/18/16.
//  Copyright © 2016 Fielding Johnston. All rights reserved.
//
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "texture.h"

Texture::Texture()
{
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
}

Texture::~Texture()
{
  free();
}


void Texture::render( int x, int y, SDL_Renderer* renderer, SDL_Rect* clip )
{
  SDL_Rect renderQuad = { x , y, mWidth, mHeight };
  
  if( clip != NULL )
  {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }
  SDL_RenderCopy( renderer, mTexture, clip, &renderQuad );
}

bool Texture::loadFromFile( std::string path, SDL_Renderer* renderer )
{
  free();
  
  //The final texture
  SDL_Texture* newTexture = NULL;
  
  //Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
  if( loadedSurface == NULL )
  {
    printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
  }
  else
  {
    //Color key image
    SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
    if( newTexture == NULL )
    {
      printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }
    else
    {
      mWidth = loadedSurface->w;
      mHeight = loadedSurface->h;
    }
    
    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );
  }
  
  mTexture = newTexture;
  return mTexture != NULL;
}

bool Texture::loadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font* font, SDL_Renderer* renderer )
{
  free();
  
  SDL_Surface* textSurface = TTF_RenderText_Solid( font, textureText.c_str(), textColor );
  if ( textSurface == NULL )
  {
    printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
  }
  else
  {
    mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
    if ( mTexture == NULL )
    {
      printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
    }
    else
    {
      mWidth = textSurface->w;
      mHeight = textSurface->h;
    }
    SDL_FreeSurface( textSurface );
  }
  return mTexture != NULL;
}

void Texture::free()
{
  if( mTexture != NULL )
  {
    SDL_DestroyTexture( mTexture );
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
  }
}

int Texture::getWidth()
{
  return mWidth;
}

int Texture::getHeight()
{
  return mHeight;
}