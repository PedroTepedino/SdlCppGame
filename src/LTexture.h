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
    bool loadFromFile( const std::string& path );

    //Creates image from font string
    bool loadFromRenderedText( const std::string& textureText, SDL_Color textColor );

    //Dealocate texture
    void free();

    //Set color modulation
    void setColor( Uint8 red, Uint8 green, Uint8 blue );

    //Set blending
    void setBlendMode( SDL_BlendMode blending );

    //Set alpha modulation
    void  setAlpha( Uint8 alpha );

    //Renders texture at a given point
    void render(int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

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