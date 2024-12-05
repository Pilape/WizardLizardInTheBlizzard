#include "textures.h"

struct Textures textures;

void TexturesInit()
{
    textures.player = LoadTexture("resources/sprites/player.png");
    textures.enemy = LoadTexture("resources/sprites/enemy.png");
    textures.projectile = LoadTexture("resources/sprites/projectile.png");
}
