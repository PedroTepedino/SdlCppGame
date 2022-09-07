//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_haptic.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include "LTexture.h"
#include "LButton.h"
#include "../Debugging/Debug.h"


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

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

LTexture gUpTexture;
LTexture gDownTexture;
LTexture gLeftTexture;
LTexture gRightTexture;
LTexture gPressTexture;

LTexture gArrowTexture;

//Buttons objects
LButton gButtons[ TOTAL_BUTTONS ];

//Game Controller handler with force feedback
SDL_GameController* gGameController = nullptr;

//Joystick handler with haptic
SDL_Joystick* gJoystick = nullptr;
SDL_Haptic* gJoyHaptic = nullptr;

//The image we will load and show on the screen
int main(int argc, char* args[] )
{
	// Start up SDL and create window
	if( !init() )
	{
		// printf( "Failed to initialize!\n" );
		Debug::Log("Failed to initialize!");
	}
	else
	{
		//Load media
		if ( !loadMedia() )
		{
			// printf("Failed to load media!\n");
			Debug::Log("Failed to load media!");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Current rendererd texture
			LTexture* currentTexture = nullptr;

			//Normalized direction
			int xDir = 0;
			int yDir = 0;

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
					else if (e.type == SDL_JOYAXISMOTION)
					{
						//Motion on controller 0
						if (e.jaxis.which == 0)
						{
							//X axis motion
							if (e.jaxis.axis == 0)
							{
								//Left of dead zone
								if( e.jaxis.value < -JOYSTICK_DEAD_ZONE )
								{
									xDir = -1;
								}
								//Right of dead zone
								else if ( e.jaxis.value > JOYSTICK_DEAD_ZONE )
								{
									xDir = 1;
								}
								else
								{
									xDir = 0;
								}
							}
							//Y axis motion
							else if (e.jaxis.axis == 1)
							{
								//Below of dead zone
								if( e.jaxis.value < -JOYSTICK_DEAD_ZONE )
								{
									yDir = -1;
								}
								//Above of dead zone
								else if ( e.jaxis.value > JOYSTICK_DEAD_ZONE )
								{
									yDir = 1;
								}
								else
								{
									yDir = 0;
								}
							}
						}
					}
					//Joystick button press
					else if ( e.type == SDL_JOYBUTTONDOWN )
					{
						//Use game controller
						if ( gGameController != nullptr )
						{
							//Play rumble at 75% strenght for 500 milliseconds
							if ( SDL_GameControllerRumble( gGameController, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 500 ) != 0 )
							{
								Debug::Log( "Warning: Unable to play game controller rumble! %s\n", SDL_GetError() );
							}
						}
						//Use haptics
						else if ( gJoyHaptic != nullptr )
						{
							//Play rumble at 75% strenght for 500 milliseconds
							if ( SDL_HapticRumblePlay( gJoyHaptic, 0.75, 500 ) != 0 )
							{
								Debug::Log( "Warning: Unable to play haptic rumble! %s\n", SDL_GetError() );
							}
						}
					}

					for (int i = 0; i < TOTAL_BUTTONS; i++)
					{
						gButtons[i].handleEvent(&e);
					}
				}

				//Set texture based on current keystate
				const Uint8* currentKeyStates = SDL_GetKeyboardState( nullptr );
				if ( currentKeyStates[SDL_SCANCODE_UP] )
				{
					currentTexture = &gUpTexture;
				}
				else if ( currentKeyStates[SDL_SCANCODE_DOWN] )
				{
					currentTexture = &gDownTexture;
				}
				else if ( currentKeyStates[SDL_SCANCODE_LEFT] )
				{
					currentTexture = &gLeftTexture;
				}
				else if ( currentKeyStates[SDL_SCANCODE_RIGHT] )
				{
					currentTexture = &gRightTexture;
				}
				else 
				{
					currentTexture = &gPressTexture;
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear( gRenderer );

				//Calculate angle
				double joystickAngle = atan2((double)yDir, (double)xDir) * (180.0 / M_PI);

				//Correct angle
				if (xDir == 0 && yDir == 0)
				{
					joystickAngle = 0;
				}

				//Render joystick 8 way angle
				gArrowTexture.render((SCREEN_WIDTH - gArrowTexture.getWidth()) / 2, (SCREEN_HEIGHT - gArrowTexture.getHeight()) / 2,
									 nullptr, joystickAngle);

				//Update screen
				SDL_RenderPresent( gRenderer );
				SDL_Haptic* gJoyHaptic = nullptr;
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
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER ) < 0 )
	{
		// printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		Debug::Log("SDL could not initialize! SDL_Error: %s", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texturing filter to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			// printf( "Warning: Linear texture filtering not enabled!" );
			Debug::Log( "Warning: Linear texture filtering not enabled!" );
		}

		//Check for joysticks
		if (SDL_NumJoysticks() < 1)
		{
			Debug::Log("Warning: No joysticks connected!");
		}
		else
		{
			//Check if first joystick is game controller interface compatible
			if ( !SDL_IsGameController( 0 ) )
			{
				Debug::Log( "Warning: Joystick is not game controller interface compatible! SDL Error: %s\n", SDL_GetError() );
			}
			else 
			{
				//Open game controller and check if it supports rumble
				gGameController = SDL_GameControllerOpen( 0 );
				if ( !SDL_GameControllerHasRumble( gGameController ) )
				{
					Debug::Log( "Warning: Game controller does not have rumble! SDL Error: %s\n", SDL_GetError() );
				}
			}

			//Load joystick if game controller could not be loaded
			if ( gGameController == nullptr )
			{
				//Open first joystick
				gJoystick = SDL_JoystickOpen( 0 );
				if ( gJoystick == nullptr )
				{
					Debug::Log("Warning: Unable to open game controller! SDL Error: %s");
				}
				else 
				{
					//Check if joystick supports haptic
					if ( !SDL_JoystickIsHaptic( gJoystick ) )
					{
						Debug::Log( "Warning: Controller does not support haptics! SDL Error: %s\n", SDL_GetError() );
					}
					else
					{
						//Get joystick haptic device
						gJoyHaptic = SDL_HapticOpenFromJoystick( gJoystick );
						if ( gJoyHaptic == nullptr )
						{
							Debug::Log( "Warning: Unable to get joystick haptics! SDL Error: %s\n", SDL_GetError() );
						}
						else
						{
							//Initialize rumble
							if ( SDL_HapticRumbleInit( gJoyHaptic ) < 0 )
							{
								Debug::Log( "Warning: Unable to initialize haptic rumble! SDL Error: %s\n", SDL_GetError() );
							}
						}
					}
				}
			}
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == nullptr )
		{
			// printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			Debug::Log( "Window could not be created! SDL Error: %s", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if( gRenderer == nullptr )
			{
			// printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
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
//					  printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
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

	if(!gUpTexture.loadFromFile("../assets/up.png"))
	{
		Debug::Log("Failed to load sprite! SDL_IMG Error: %s!", IMG_GetError());
		success = false;	
	}

	if(!gDownTexture.loadFromFile("../assets/down.png"))
	{
		Debug::Log("Failed to load sprite! SDL_IMG Error: %s!", IMG_GetError());
		success = false;	
	}

	if(!gLeftTexture.loadFromFile("../assets/left.png"))
	{
		Debug::Log("Failed to load sprite! SDL_IMG Error: %s!", IMG_GetError());
		success = false;	
	}

	if(!gRightTexture.loadFromFile("../assets/right.png"))
	{
		Debug::Log("Failed to load sprite! SDL_IMG Error: %s!", IMG_GetError());
		success = false;	
	}

	if(!gPressTexture.loadFromFile("../assets/press.png"))
	{
		Debug::Log("Failed to load sprite! SDL_IMG Error: %s!", IMG_GetError());
		success = false;	
	}

	if (!gArrowTexture.loadFromFile("../assets/arrow.png"))
	{
		Debug::Log("Falied to load arrow sprite! SDL_IMG Error: %s!", IMG_GetError());
		success = false;
	}

	// if (!gButtonSpriteSheetTexture.loadFromFile("../assets/button.png"))
	// {
	//	   Debug::Log("Failed to load sprite! SDL_ttf Error: %s!", TTF_GetError());
	//	   success = false;
	// }
	// else
	// {
	//	   //Set sprites
	//	   for(int i = 0; i < BUTTON_SPRITE_TOTAL; i++)
	//	   {
	//		   gSpriteClips[i] = {.x = 0, .y = i * 200, .w = BUTTON_WIDTH, .h = BUTTON_HEIGHT};
	//	   }

	//	   gButtons[0].setPosition(0,0);
	//	   gButtons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH,0);
	//	   gButtons[2].setPosition(0,SCREEN_HEIGHT - BUTTON_HEIGHT);
	//	   gButtons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH,SCREEN_HEIGHT - BUTTON_HEIGHT);

	// }

	return success;
}

void close()
{
	//Free loaded images
	gButtonSpriteSheetTexture.free();

//	  //Free global font
//	  TTF_CloseFont( gFont );
//	  gFont = nullptr;

	//Close game controller or joystick with haptics
	if ( gGameController != nullptr)
	{
		SDL_GameControllerClose( gGameController );
	}
	if ( gJoyHaptic != nullptr )
	{
		SDL_HapticClose( gJoyHaptic );
	}
	if ( gJoystick != nullptr )
	{
		SDL_JoystickClose( gJoystick );
	}
	gGameController = nullptr;
	gJoyHaptic = nullptr;
	gJoystick = nullptr;


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
