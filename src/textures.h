#ifndef TEXTURES_H
#define TEXTURES_H

#include <raylib.h>

struct Textures
{
    Texture2D player;
    Texture2D enemy;
};

extern struct Textures textures;

void TexturesInit(void);

#endif