//
//  Piece.cpp
//  JCashProjectSubmission

#include "Piece.hpp"

//Button constants
const int BUTTON_WIDTH = 200;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 9;

//Button Positions
SDL_Point * positions;

//Buttons objects
Piece gButtons[ TOTAL_BUTTONS ];

//Mouse button sprites
SDL_Rect gSpriteClips[ TOTAL_BUTTONS ];
JTexture gButtonSpriteSheetTexture;

//The music that will be played
Mix_Music *gMusic = NULL;

//The sound effect that will be used
Mix_Chunk *gSound = NULL;

Piece::Piece()
{
    mPosition.x = 0;
    mPosition.y = 0;
}

void Piece::setPosition( int x, int y )
{
    mPosition.x = x;
    mPosition.y = y;
}

SDL_Point Piece::getPosition() {
    return mPosition;
}

void Piece::handleEvent( SDL_Event* e )
{
    //If mouse event happened
    if( e->type == SDL_MOUSEBUTTONDOWN)
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
        
        //Check if mouse is in button
        bool inside = true;
        
        //Mouse is left of the button
        if( x < mPosition.x )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x > mPosition.x + BUTTON_WIDTH )
        {
            inside = false;
        }
        //Mouse above the button
        else if( y < mPosition.y )
        {
            inside = false;
        }
        //Mouse below the button
        else if( y > mPosition.y + BUTTON_HEIGHT )
        {
            inside = false;
        }
        
        //Mouse is inside button
        if( inside )
        {
            int index = 0;
            //Find current position index
            for(int x = 0; x < TOTAL_BUTTONS; ++x) {
                if(mPosition.x == positions[x].x && mPosition.y == positions[x].y) {
                    index = x;
                    break;
                }
            }
            //Check above, below, left, and right
            int aboveIndex = index-3;
            int rightIndex = index+1;
            int leftIndex = index-1;
            int belowIndex = index+3;
            
            bool aboveEmpty = false;
            bool rightEmpty = false;
            bool leftEmpty = false;
            bool belowEmpty = false;
            
            if(aboveIndex >= 0) {
                bool occupied = false;
                for(int x = 0; x < TOTAL_BUTTONS; ++x) {
                    if(positions[aboveIndex].x == gButtons[x].mPosition.x && positions[aboveIndex].y == gButtons[x].mPosition.y) {
                        occupied = true;
                    }
                    if(occupied) break;
                }
                aboveEmpty = !occupied;
            }
            
            if(rightIndex < TOTAL_BUTTONS) {
                bool occupied = false;
                for(int x = 0; x < TOTAL_BUTTONS; ++x) {
                    if(positions[rightIndex].x == gButtons[x].mPosition.x && positions[rightIndex].y == gButtons[x].mPosition.y) {
                        occupied = true;
                    }
                    if(occupied) break;
                }
                rightEmpty = !occupied;
            }
            
            if(leftIndex >= 0) {
                bool occupied = false;
                for(int x = 0; x < TOTAL_BUTTONS; ++x) {
                    if(positions[leftIndex].x == gButtons[x].mPosition.x && positions[leftIndex].y == gButtons[x].mPosition.y) {
                        occupied = true;
                    }
                    if(occupied) break;
                }
                leftEmpty = !occupied;
            }
            
            if(belowIndex < TOTAL_BUTTONS) {
                bool occupied = false;
                for(int x = 0; x < TOTAL_BUTTONS; ++x) {
                    if(positions[belowIndex].x == gButtons[x].mPosition.x && positions[belowIndex].y == gButtons[x].mPosition.y) {
                        occupied = true;
                    }
                    if(occupied) break;
                }
                belowEmpty = !occupied;
            }
            
            if(aboveEmpty) {
                Mix_PlayChannel( -1, gSound, 0 );
                mPosition = positions[aboveIndex];
            }
            else if(rightEmpty) {
                Mix_PlayChannel( -1, gSound, 0 );
                mPosition = positions[rightIndex];
            }
            else if(leftEmpty) {
                Mix_PlayChannel( -1, gSound, 0 );
                mPosition = positions[leftIndex];
            }
            else if(belowEmpty) {
                Mix_PlayChannel( -1, gSound, 0 );
                mPosition = positions[belowIndex];
            }
        }
    }
}

void Piece::render()
{
    //Show current button sprite
    gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ picture ] );
}

void Piece::setButton(int x, int y, int picNum) {
    mPosition.x = x;
    mPosition.y = y;
    picture = picNum;
    
}

void Piece::setPicNum(int picNum) {
    picture = picNum;
}

int Piece::getPicNum() {
    return picture;
}
