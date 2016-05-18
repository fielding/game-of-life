//
//  timer.h
//  GameOfLife
//
//  Created by Fielding Johnston on 5/18/16.
//  Copyright © 2016 Fielding Johnston. All rights reserved.
//

#ifndef timer_h
#define timer_h

class Timer
{
public:
  Timer();
  
  void start();
  void stop();
  void pause();
  void unpause();
  
  Uint32 getTicks();
  
  bool isRunning();
  bool isPaused();
  
private:
  Uint32 mStartTicks;
  
  Uint32 mPausedTicks;
  
  bool mPaused;
  bool mRunning;
  
};



#endif /* timer_h */
