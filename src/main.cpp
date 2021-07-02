//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "LTexture.h"
#include "../Debugging/Debug.h"

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

//Loads individual image
SDL_Surface* loadSurface(const std::string& path);

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

//The window we'll be rendering to
SDL_Window* gWindow = nullptr;

//The window renderer
SDL_Renderer* gRenderer = nullptr;

//Walking animation
const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES] ;
LTexture gSpriteSheetTexture;

//The surface contained by the window
SDL_Surface* gScreenSurface = nullptr;

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

            //Modulation components
            Uint8 r = 255;
            Uint8 g = 255;
            Uint8 b = 255;
            Uint8 a = 255;

            //Current animation frame
            int frame = 0;

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
                }

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear( gRenderer );

                //Render current frame
                SDL_Rect* currentClip = &gSpriteClips[ frame / 4 ];
                gSpriteSheetTexture.render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);

                //Update screen
                SDL_RenderPresent( gRenderer );

                //Go to next frame
                frame++;

                //Cycle animation
                if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                {
                    frame = 0;
                }
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
            }
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load sprite sheet texture
    if (!gSpriteSheetTexture.loadFromFile("../assets/foo.png"))
    {
        Debug::Log("Failied to load walking animation texture!");
        success = false;
    }
    else
    {
        //Set sprite clips
        gSpriteClips[ 0 ] = {.x = 0, .y = 0, .w = 64, .h = 205};
        gSpriteClips[ 1 ] = {.x = 64, .y = 0, .w = 64, .h = 205};
        gSpriteClips[ 2 ] = {.x = 128, .y = 0, .w = 64, .h = 205};
        gSpriteClips[ 3 ] = {.x = 196, .y = 0, .w = 64, .h = 205};
    }

    return success;
}

void close()
{
    //Free loaded images

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = nullptr;
    gRenderer = nullptr;

    //Quit SDL subsystem
    IMG_Quit();
    SDL_Quit();
}

SDL_Surface* loadSurface(const std::string& path)
{
    SDL_Surface* optimizedSurface = nullptr;

    //Load image at specific path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if (loadedSurface == nullptr)
    {
//        printf("Unable to load image %s! SDL_image: %s\n", path.c_str(), IMG_GetError());
        Debug::Log("Unable to load image %s! SDL_image: %s", path.c_str(), IMG_GetError());
    }
    else
    {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if (optimizedSurface == nullptr)
        {
//            printf("Unable to optimize image %s!\nSDL Error: %s\n", path.c_str(), SDL_GetError());
            Debug::Log("Unable to optimize image %s! SDL Error: %s", path.c_str(), SDL_GetError());
        }

        //Get Rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}

SDL_Texture* loadTexture(const std::string path)
{
    //The final texture
    SDL_Texture* newTexture = nullptr;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == nullptr )
    {
//        printf("Unable to load image %s! SDL_image: %s\n", path.c_str(), IMG_GetError());
        Debug::Log("Unable to load image %s! SDL_image: %s", path.c_str(), IMG_GetError());
    }
    else
    {
        //Create texture from surface
        newTexture =  SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if( newTexture == nullptr )
        {
//            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
            Debug::Log("Unable to create texture from %s! SDL Error: %s", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}