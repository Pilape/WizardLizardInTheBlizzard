#include "resources.h"

struct Textures textures;

void TexturesInit()
{
    textures.player = LoadTexture("resources/sprites/player.png");
    textures.enemy = LoadTexture("resources/sprites/enemy.png");
    textures.projectile = LoadTexture("resources/sprites/projectile.png");
}

void TexturesUnload()
{
    UnloadTexture(textures.player);
    UnloadTexture(textures.enemy);
    UnloadTexture(textures.projectile);
}

struct Sfx sounds;

void SoundInit()
{
    InitAudioDevice();
    sounds.click = LoadSound("resources/sounds/fx/click.wav");
    sounds.hurt = LoadSound("resources/sounds/fx/hitHurt.wav");
    SetSoundVolume(sounds.hurt, 0.5f);
    sounds.shoot = LoadSound("resources/sounds/fx/laserShoot.wav");
    SetSoundVolume(sounds.shoot, 0.25f);

    sounds.mainMusic = LoadMusicStream("resources/sounds/music/[No Copyright Music]  ko0x - Galaxy Guppy [Chiptune].mp3");
    PlayMusicStream(sounds.mainMusic);
}

void SoundUnload()
{
    UnloadSound(sounds.click);
    UnloadSound(sounds.hurt);
    UnloadSound(sounds.shoot);

    UnloadMusicStream(sounds.mainMusic);
}