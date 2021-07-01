#ifndef LTEXTURE_H
#define LTEXTURE_H

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
    bool loadFromFile(const std::string& path);

    //Dealocate texture
    void free();

    //Renders texture at a given point
    void render(int x, int y, SDL_Rect* clip = nullptr);

    //Gets image dimesions
    int getWidth() const;
    int getHeight() const;

private:
    //The actual harware texture
    SDL_Texture* mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};

#endif