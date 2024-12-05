#include "textures.h"

struct Textures textures;

void TexturesInit()
{
    textures.player = LoadTexture("resources/sprites/player.png");
}
