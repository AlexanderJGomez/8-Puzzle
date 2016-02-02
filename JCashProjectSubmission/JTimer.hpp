//
//  JTimer.hpp
//  JCashProjectSubmission

#ifndef __JCashProjectSubmission__JTimer__
#define __JCashProjectSubmission__JTimer__

#include "Piece.cpp"

//The application time based timer
class JTimer
{
public:
    //Initializes variables
    JTimer();
    
    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();
    
    //Gets the timer's time
    Uint32 getTicks();
    
    //Checks the status of the timer
    bool isStarted();
    bool isPaused();
    
private:
    //The clock time when the timer started
    Uint32 mStartTicks;
    
    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;
    
    //The timer status
    bool mPaused;
    bool mStarted;
};

#endif
