//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "LTexture.h"
#include "../Debugging/Debug.h"
#include "LButton.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Key press surfaces constants
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL,
};


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = nullptr;

//The window renderer
SDL_Renderer* gRenderer = nullptr;

//Globally used font
TTF_Font *gFont = nullptr;

LTexture gTextTexture;
LTexture gButtonSpriteSheetTexture;
SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];

//Buttons objects
LButton gButtons[ TOTAL_BUTTONS ];


//The image we will load and show on the screen
int main(int argc, char* args[] )
{
    // Start up SDL and create window
    if( !init() )
    {
//        printf( "Failed to initialize!\n" );
        Debug::Log("Failed to initialize!");
    }
    else
    {
        //Load media
        if ( !loadMedia() )
        {
//            printf("Failed to load media!\n");
            Debug::Log("Failed to load media!");
        }
        else
        {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;



            //While application is running
            while(!quit)
            {
                //Handle events on queue
                while(SDL_PollEvent(&e) != 0)
                {
                    //User requests quit
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    //On keypress change rgb values
                    else if (e.type == SDL_KEYDOWN)
                    {
                        switch (e.key.keysym.sym)
                        {
                            //Exit on ESCAPE
                            case SDLK_ESCAPE:
                                quit = true;
                                break;
                        }
                    }

                    for (int i = 0; i < TOTAL_BUTTONS; i++)
                    {
                        gButtons[i].handleEvent(&e);
                    }
                }

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear( gRenderer );

                //Render buttons
                for (auto & gButton : gButtons)
                {
                    gButton.render();
                }

                //Update screen
                SDL_RenderPresent( gRenderer );
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}


bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
//        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        Debug::Log("SDL could not initialize! SDL_Error: %s", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texturing filter to linear
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
//            printf( "Warning: Linear texture filtering not enabled!" );
            Debug::Log( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == nullptr )
		{
//			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            Debug::Log( "Window could not be created! SDL Error: %s", SDL_GetError() );
            success = false;
		}
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if( gRenderer == nullptr )
            {
//                printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
                Debug::Log( "Renderer could not be created! SDL_Error: %s", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if ( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
//                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    Debug::Log("SDL_image could not initialize! SDL_image Error: %s", IMG_GetError());
                    success = false;
                }

                //Initialize SDL_ttf
                if( TTF_Init() == -1)
                {
                    Debug::Log("SDL_ttf could not initialize! SDL_ttf Error: %s!", TTF_GetError());
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

    if (!gButtonSpriteSheetTexture.loadFromFile("../assets/button.png"))
    {
        Debug::Log("Failed to load sprite! SDL_ttf Error: %s!", TTF_GetError());
        success = false;
    }
    else
    {
        //Set sprites
        for(int i = 0; i < BUTTON_SPRITE_TOTAL; i++)
        {
            gSpriteClips[i] = {.x = 0, .y = i * 200, .w = BUTTON_WIDTH, .h = BUTTON_HEIGHT};
        }

        gButtons[0].setPosition(0,0);
        gButtons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH,0);
        gButtons[2].setPosition(0,SCREEN_HEIGHT - BUTTON_HEIGHT);
        gButtons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH,SCREEN_HEIGHT - BUTTON_HEIGHT);

    }

    return success;
}

void close()
{
    //Free loaded images
    gButtonSpriteSheetTexture.free();

    //Free global font
    TTF_CloseFont( gFont );
    gFont = nullptr;

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = nullptr;
    gRenderer = nullptr;

    //Quit SDL subsystem
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}