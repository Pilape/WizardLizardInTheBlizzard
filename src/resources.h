#ifndef TEXTURES_H
#define TEXTURES_H

#include <raylib.h>

struct Textures
{
    Texture2D player;
    Texture2D enemy;
    Texture2D projectile;
};

extern struct Textures textures;

void TexturesInit(void);
void TexturesUnload(void);

struct Sfx
{
    Sound shoot;
    Sound hurt;
    Sound click;

    Music mainMusic;
};

extern struct Sfx sounds;

void SoundInit(void);
void SoundUnload(void);

#endif