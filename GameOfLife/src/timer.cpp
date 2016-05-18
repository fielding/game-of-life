//
//  timer.cpp
//  GameOfLife
//
//  Created by Fielding Johnston on 5/18/16.
//  Copyright © 2016 Fielding Johnston. All rights reserved.
//

#include <stdio.h>
#include <SDL2/SDL.h>
#include "timer.h"


Timer::Timer()
{
  mStartTicks = 0;
  mPausedTicks = 0;
  
  mPaused = false;
  mRunning = false;
}

void Timer::start()
{
  mRunning = true;
  mPaused = false;
  mStartTicks = SDL_GetTicks();
  mPausedTicks = 0;
}


void Timer::stop()
{
  mRunning = false;
  mPaused = false;
  mStartTicks = 0;
  mPausedTicks = 0;
}

void Timer::pause()
{
  if ( mRunning && !mPaused )
  {
    mPaused = true;
    
    mPausedTicks = SDL_GetTicks() - mStartTicks;
    mStartTicks = 0;
  }
}

void Timer::unpause()
{
  if ( mRunning && mPaused )
  {
    mPaused = false;
    mStartTicks = SDL_GetTicks() - mPausedTicks;
    mPausedTicks = 0;
  }
}

Uint32 Timer::getTicks()
{
  Uint32 time = 0;
  
  if ( mRunning )
  {
    if ( mPaused )
    {
      time = mPausedTicks;
    }
    else
    {
      time = SDL_GetTicks() - mStartTicks;
    }
  }
  return time;
}

bool Timer::isRunning()
{
  return mRunning;
}

bool Timer::isPaused()
{
  return mPaused;
}