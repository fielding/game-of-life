//
//  texture.hpp
//  GameOfLife
//
//  Created by Fielding Johnston on 5/18/16.
//  Copyright © 2016 Fielding Johnston. All rights reserved.
//

#ifndef texture_hpp
#define texture_hpp

#include <stdio.h>
#include <string>

class Texture
{
public:
  Texture();
  ~Texture();

  bool loadFromFile( std::string path, SDL_Renderer* renderer);
  bool loadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font* font, SDL_Renderer* renderer );
  
  void free();
  void render( int x, int y, SDL_Renderer* renderer, SDL_Rect* clip = NULL );
  
  int getWidth();
  int getHeight();
  
private:
  SDL_Texture* mTexture;
  int mWidth;
  int mHeight;
  
};

#endif /* texture_hpp */
