//
//  Piece.hpp
//  JCashProjectSubmission

#ifndef __JCashProjectSubmission__Piece__
#define __JCashProjectSubmission__Piece__

#include "JTexture.cpp"
#include "SDL2_mixer/SDL_mixer.h"

//The mouse button
class Piece
{
public:
    //Initializes internal variables
    Piece();
    
    //Sets top left position
    void setButton( int x, int y, int picNum);
    
    //sets picNum
    void setPicNum( int picNum);
    
    //gets picNum
    int getPicNum();
    
    //Sets top left position
    void setPosition( int x, int y );
    
    //Returns position
    SDL_Point getPosition();
    
    //Handles mouse event
    void handleEvent( SDL_Event* e );
    
    //Shows button sprite
    void render();
    
    
private:
    //Top left position
    SDL_Point mPosition;
    
    //Currently used global sprite
    int picture;
};

#endif
