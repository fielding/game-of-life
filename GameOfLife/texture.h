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

class Texture
{
public:
  Texture();
  ~Texture();

  bool loadFromFile( std::string path );
  void free();
  void render( int x, int y, SDL_Rect* clip = NULL );
  
};

#endif /* texture_hpp */
