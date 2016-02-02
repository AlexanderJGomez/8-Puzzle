// J-Cash Puzzle main.cpp

#include "JTimer.cpp"
#include <stdio.h>
#include <sstream>
#include <ctime>

//Screen dimension constants
const int SCREEN_WIDTH = 650;
const int SCREEN_HEIGHT = 650;

//Time to complete
const int TOTAL_TIME = 120;

//Array of the textures for our puzzle
SDL_Surface images[9];

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Set buttons in corners
void arrangePuzzle();

//Shuffles an array of pieces
void shuffle(Piece * pieces);

//Find the white tile
int findEmptyIndex();

//Finds index of a Piece at a given point
int findPieceAtPosition(SDL_Point pos);

//Move the puzzle a given direction
bool move(std::string dir);

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//Scene textures
JTexture gTimeTextTexture;
JTexture splash;

bool init()
{
    //Initialization flag
    bool success = true;
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }
        
        //Create window
        gWindow = SDL_CreateWindow( "J-Ca$h Puzzle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", SDL_GetError() );
                    success = false;
                }
                
                //Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
                
                //Initialize SDL_mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
            }
        }
    }
    
    return success;
}








bool loadMedia()
{
    //Loading success flag
    bool success = true;
    
    //Load splash texture
    if( !splash.loadFromFile( "splash.png" ) )
    {
        printf( "Failed to load splash texture!\n" );
        success = false;
    }
    
    //Open the font
    gFont = TTF_OpenFont("SIXTY.ttf", 28 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    
    //Load sprites
    if( !gButtonSpriteSheetTexture.loadFromFile( "altchuler.bmp" ) )
    {
        printf( "Failed to load button sprite texture!\n" );
        success = false;
    }
    else
    {
        //Set sprites
        for( int xPos = 0; xPos < TOTAL_BUTTONS / 3; ++xPos )
        {
            for(int yPos = 0; yPos < TOTAL_BUTTONS / 3; ++yPos)
            {
                gSpriteClips[ xPos * 3 + yPos ].x = 200 * xPos;
                gSpriteClips[ xPos * 3 + yPos ].y = yPos * 200;
                gSpriteClips[ xPos * 3 + yPos ].w = BUTTON_WIDTH;
                gSpriteClips[ xPos * 3 + yPos ].h = BUTTON_HEIGHT;
            }
        }
        
        arrangePuzzle();
        
    }
    
    //Load music
    gMusic = Mix_LoadMUS( "song.wav" );
    if( gMusic == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    //Load sound effects
    gSound = Mix_LoadWAV( "sound.wav" );
    if( gSound == NULL )
    {
        printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    return success;
}






void arrangePuzzle() {
    gButtons[ 0 ].setButton( 0, 0, 0);
    gButtons[ 1 ].setButton( BUTTON_WIDTH, 0, 3);
    gButtons[ 2 ].setButton( BUTTON_WIDTH * 2, 0, 6);
    gButtons[ 3 ].setButton( 0, BUTTON_HEIGHT, 1);
    gButtons[ 4 ].setButton( BUTTON_WIDTH, BUTTON_HEIGHT, 4);
    gButtons[ 5 ].setButton( BUTTON_WIDTH * 2, BUTTON_HEIGHT, 7);
    gButtons[ 6 ].setButton( 0, BUTTON_HEIGHT * 2, 2);
    gButtons[ 7 ].setButton( BUTTON_WIDTH, BUTTON_HEIGHT * 2, 5);
    
}





void shuffle(Piece * pieces) {
    unsigned int time_ui = (unsigned int)( time(NULL) );
    srand( time_ui );
    //Takes anywhere from 25-34 moves
    int moves = (rand() % 10) + 25;
    for(int x = 0; x < moves; ++x) {
        bool moved = false;
        while(!moved) {
            int direction = rand() % 4;
            if(direction == 0) {
                moved = move("up");
            }
            else if(direction == 1) {
                moved = move("down");
            }
            else if(direction == 2) {
                moved = move("left");
            }
            else if(direction == 3) {
                moved = move("right");
            }
        }
    }
}

int findEmptyIndex() {
    int empty = 0;
    for(int y = 0; y < TOTAL_BUTTONS / 3; ++y) {
        for(int x = 0; x < TOTAL_BUTTONS / 3; ++x) {
            int xPosToCheck = x * BUTTON_WIDTH;
            int yPosToCheck = y * BUTTON_HEIGHT;
            int occupied = false;
            for(int index = 0; index < TOTAL_BUTTONS; ++index) {
                occupied = gButtons[index].getPosition().x == xPosToCheck && gButtons[index].getPosition().y == yPosToCheck;
                if(occupied) break;
            }
            if(!occupied) {
                empty = y*3+x;
                break;
            }
        }
    }
    return empty;
}

int findPieceAtPosition(SDL_Point pos) {
    for(int x = 0; x < TOTAL_BUTTONS; ++x) {
        if(gButtons[x].getPosition().x == pos.x && gButtons[x].getPosition().y == pos.y)
            return x;
    }
    return 0;
}

//moves the pieces
bool move(std::string dir) {
    if(dir == "up") {
        //Move piece up
        int emptySpace = findEmptyIndex();
        if(emptySpace >= 6 && emptySpace <= 8) return false;
        int spaceToMove = emptySpace+3;
        int piece = findPieceAtPosition(positions[spaceToMove]);
        gButtons[piece].setPosition(positions[emptySpace].x, positions[emptySpace].y);
    }
    else if(dir == "down") {
        //Move piece down
        int emptySpace = findEmptyIndex();
        if(emptySpace >= 0 && emptySpace <= 2) return false;
        int spaceToMove = emptySpace-3;
        int piece = findPieceAtPosition(positions[spaceToMove]);
        gButtons[piece].setPosition(positions[emptySpace].x, positions[emptySpace].y);
    }
    else if(dir == "left") {
        //Move piece left
        int emptySpace = findEmptyIndex();
        if(emptySpace == 2 || emptySpace == 5 || emptySpace == 8) return false;
        int spaceToMove = emptySpace+1;
        int piece = findPieceAtPosition(positions[spaceToMove]);
        gButtons[piece].setPosition(positions[emptySpace].x, positions[emptySpace].y);
    }
    else if(dir == "right") {
        //Move piece right
        int emptySpace = findEmptyIndex();
        if(emptySpace == 0 || emptySpace == 3 || emptySpace == 6) return false;
        int spaceToMove = emptySpace-1;
        int piece = findPieceAtPosition(positions[spaceToMove]);
        gButtons[piece].setPosition(positions[emptySpace].x, positions[emptySpace].y);
    }
    return true;
}




void close()
{
    //Free loaded images
    splash.free();
    gButtonSpriteSheetTexture.free();
    
    //Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;
    
    //Free the sound effects
    Mix_FreeChunk( gSound );
    gSound = NULL;
    
    //Free the music
    Mix_FreeMusic( gMusic );
    gMusic = NULL;
    
    //Destroy window	
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;
    
    //Quit SDL subsystems
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}




bool gameOver() {
    for(int i = 0 ; i < 8; i++) {
        if(!(gButtons[i].getPosition().x == (gButtons[i].getPicNum() / 3 ) * BUTTON_WIDTH &&
             gButtons[i].getPosition().y == (gButtons[i].getPicNum() % 3 ) * BUTTON_HEIGHT)) {
            return false;
        }
    }
    return true;
}



int main( int argc, char* args[] )
{
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {	
            //Main loop flag
            bool quit = false;
            
            //Event handler
            SDL_Event e;
            
            //Set text color as black
            SDL_Color textColor = { 0, 0, 0, 255 };
            
            //The application timer
            JTimer timer;
            
            //In memory text stream
            std::stringstream timeText;
            
            //Set anchor positions
            positions = new SDL_Point[9];
            positions[0] = {0, 0};
            positions[1] = {BUTTON_WIDTH, 0};
            positions[2] = {BUTTON_WIDTH*2, 0};
            positions[3] = {0, BUTTON_HEIGHT};
            positions[4] = {BUTTON_WIDTH, BUTTON_HEIGHT};
            positions[5] = {BUTTON_WIDTH*2, BUTTON_HEIGHT};
            positions[6] = {0, BUTTON_HEIGHT*2};
            positions[7] = {BUTTON_WIDTH, BUTTON_HEIGHT*2};
            positions[8] = {BUTTON_WIDTH*2, BUTTON_HEIGHT*2};
            
            bool started = false;
            bool gameover = false;
            bool toQuit = false;
            //While application is running
            while( !quit )
            {
                
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    else if( e.type == SDL_KEYDOWN)
                    {
                        //Start/stop
                        if( e.key.keysym.sym == SDLK_s )
                        {
                            started = true;
                        }
                        else if( e.key.keysym.sym == SDLK_SPACE )
                        {
                            if( !timer.isStarted() && started )
                            {
                                //Start the timer
                                timer.start();
                                
                                //Shuffle the pieces
                                shuffle(gButtons);
                                
                                gButtons[8].setPosition(BUTTON_WIDTH*4, BUTTON_HEIGHT*4);
                                
                                //Play the music
                                Mix_PlayMusic( gMusic, -1 );
                            }
                        }
                        else if( e.key.keysym.sym == SDLK_r )
                        {
                            if( timer.isStarted() )
                            {
                                //Stop the timer
                                timer.stop();
                                
                                //Rearrange pieces
                                arrangePuzzle();
                                
                                //Stop the music
                                Mix_HaltMusic();
                                
                                gameover = false;
                            }
                        }
                        // Keyboard-based movement
                        else if( timer.isStarted() && !gameover) {
                            if( e.key.keysym.sym == SDLK_UP )
                            {
                                move("up");
                                Mix_PlayChannel( -1, gSound, 0 );
                            }
                            else if( e.key.keysym.sym == SDLK_DOWN )
                            {
                                move("down");
                                Mix_PlayChannel( -1, gSound, 0 );
                            }
                            else if( e.key.keysym.sym == SDLK_LEFT ){
                                move("left");
                                Mix_PlayChannel( -1, gSound, 0 );
                            }
                            else if( e.key.keysym.sym == SDLK_RIGHT )
                            {
                                move("right");
                                Mix_PlayChannel( -1, gSound, 0 );
                            }
                        }
                    }
                    
                    if(!timer.isStarted()) break;
                    
                    if(gameOver()) {
                        gameover = true;
                    }
                    else {
                        int remaining = (TOTAL_TIME - timer.getTicks() / 1000 );
                        if(remaining > 0) {
                            //Handle button events
                            for( int i = 0; i < TOTAL_BUTTONS; ++i )
                            {
                                gButtons[ i ].handleEvent( &e );
                            }
                        }
                    }
                
                }
                
                //Set text to be rendered
                timeText.str( "" );
                int remaining = (TOTAL_TIME - timer.getTicks() / 1000 );
                
                if(remaining < 0) {
                    timeText << "Game Over! The Altschuler's are disappointed! 'R' to restart";
                    gameover = true;
                }
                else if(!timer.isStarted()) {
                    timeText << "Press the space bar to start the puzzle!";
                }
                else if(gameover) {
                    timeText << "You Win! Your're officially an Altschuler! 'R' to try again.";
                }
                else {
                    timeText << "Seconds Remaining: " << remaining << ". Press 'R' to restart";
                }
                
                //Render text
                if( !gTimeTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
                {
                    printf( "Unable to render time texture!\n" );
                }
                
                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );
                
                //Render textures
                gTimeTextTexture.render( ( SCREEN_WIDTH - gTimeTextTexture.getWidth() ) / 2, SCREEN_HEIGHT - 40 );
                
                //Render buttons
                for( int i = 0; i < TOTAL_BUTTONS; ++i )
                {
                    gButtons[ i ].render();
                }
                
                //Render splash
                if(!started) splash.render( 0, 0 );
                
                //Update screen
                SDL_RenderPresent( gRenderer );
                
                if(toQuit) {
                    SDL_Delay(2000);
                    quit = true;
                }
                
            }
            
            
        }
        
    }
    
    //Free resources and close SDL
    close();
    
    return 0;
}