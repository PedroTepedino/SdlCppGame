#ifndef LTEXTURE
#define LTEXTURE
#include <string>
#include <SDL2/SDL.h>

//Texture wrapper class
class LTexture
{
public:
    //Initializes variables
    LTexture();

    //Dealocates memory
    ~LTexture();

    //Loads image at specified path
    bool loadFromFile(std::string path);

    //Dealocate texture
    void free();

    //Renders texture at a given point
    void render(int x, int y);

    //Gets image dimesions
    int getWidth();
    int getHeight();

private:
    //The actual harware texture
    SDL_Texture* mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};
#endif //LTEXTURE