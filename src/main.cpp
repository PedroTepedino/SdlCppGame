//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <string>
#include "LTexture.h"
#include "globals.h"

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
SDL_Texture* loadTexture(const std::string path);

//The window we'll be rendering to
SDL_Window* gWindow = nullptr;

//The window renderer
SDL_Renderer* gRenderer = nullptr;

//Scene textures
LTexture gFooTexture;
LTexture gBackgroundTexture;

//Current displayed texture
SDL_Texture* gTexture = nullptr;

//The surface contained by the window
SDL_Surface* gScreenSurface = nullptr;

// The images that correspond to a keypress
SDL_Surface* gStrachedSurface = nullptr;

//The image we will load and show on the screen

int main( int argc, char* args[] )
{
    // Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if ( !loadMedia() )
        {
            printf("Failed to load media!\n");
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
                        quit =  true;
                    }
                }

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear( gRenderer );

                //Render bakcground texture to the screen
                gBackgroundTexture.render(0, 0);

                //Render Foo' to the screen
                gFooTexture.render(240, 190);

                // //Top left corner viewport
                // SDL_Rect topLeftViewport;
                // topLeftViewport.x = 0;
                // topLeftViewport.y = 0;
                // topLeftViewport.w = SCREEN_WIDTH / 2;
                // topLeftViewport.h = SCREEN_HEIGHT / 2;
                // SDL_RenderSetViewport(gRenderer, &topLeftViewport);

                // //Render texture to screen
                // SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);


                // //Top right corner viewport
                // SDL_Rect topRightViewport;
                // topRightViewport.x = SCREEN_WIDTH / 2;
                // topRightViewport.y = 0;
                // topRightViewport.w = SCREEN_WIDTH / 2;
                // topRightViewport.h = SCREEN_HEIGHT / 2;
                // SDL_RenderSetViewport(gRenderer, &topRightViewport);

                // //Render texture to screen
                // SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);


                // //Bottom viewport
                // SDL_Rect bottomViewport;
                // bottomViewport.x = 0;
                // bottomViewport.y = SCREEN_HEIGHT / 2;
                // bottomViewport.w = SCREEN_WIDTH;
                // bottomViewport.h = SCREEN_HEIGHT / 2;
                // SDL_RenderSetViewport(gRenderer, &bottomViewport);

                // //Render texture to screen
                // SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);

                // //Render red filled quad
                // SDL_Rect fillRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
                // SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
                // SDL_RenderFillRect(gRenderer, &fillRect);

                // //Render green outlined quad
                // SDL_Rect outlinedRect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};
                // SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
                // SDL_RenderDrawRect(gRenderer, &outlinedRect);

                // //Draw horizontal line
                // SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
                // SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);
                
                // //Draw vertcal line of yellow dots
                // SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
                // for(int i = 0; i < SCREEN_HEIGHT; i += 4)
                // {
                //     SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, i);
                // }

                //Apply image stretched
                // SDL_Rect stretchRect;
                // stretchRect.x = 0;
                // stretchRect.y = 0;
                // stretchRect.w = SCREEN_WIDTH;
                // stretchRect.h = SCREEN_HEIGHT;
                // SDL_BlitScaled(gStrachedSurface, nullptr, gScreenSurface, &stretchRect);

                //Render texture to screen
                // SDL_RenderCopy( gRenderer, gTexture, nullptr, nullptr );

                //Update the surface
                // SDL_UpdateWindowSurface(gWindow);

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
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texturing filter to linear
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
        else
        {    
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if( gRenderer == nullptr )
            {
                printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
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
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
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

    //Load default surface          
    // gTexture = loadTexture("../assets/viewport.png");
    // if (gTexture == nullptr)
    // {
    //     printf("Failed to load streched image!\n");
    //     success = false;
    // }

    //Load Foo' texture
    if(!gFooTexture.loadFromFile("../assets/foo.png"))
    {
        printf("Failed to load Foo' texture image!\n");
        success = false;
    }

    //Load background texture
    if(!gBackgroundTexture.loadFromFile("../assets/background.png"))
    {
        printf("Failed to load background texture image!\n");
        success = false;
    }

    //Nothig to load
    return success;
}

void close()
{
    //Free loaded images
    gFooTexture.free();
    gBackgroundTexture.free();
    // SDL_DestroyTexture( gTexture );
    // gTexture = nullptr;
    
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
        printf("Unable to load image %s! SDL_image: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if (optimizedSurface == nullptr)
        {
            printf("Unable to optimize image %s!\nSDL Error: %s\n", path.c_str(), SDL_GetError());
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
        printf("Unable to load image %s! SDL_image: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Create texture from surface
        newTexture =  SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if( newTexture == nullptr )
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}