#pragma once

#include "Engine.h"

namespace Textures
{
    const int MAX_TEXTURES = 32;

    struct Sprite
    {
        Texture2D* texture;
        Rectangle source;
        Vector2 origin;
        float rotation;
        float scale;
        Color tint;
    };

    struct State
    {
        i32 textureCount = 0;
        Texture2D textures[MAX_TEXTURES];
    };
    extern State state;

    Texture* Load(const char* path);
    Sprite CreateSprite(Texture* texture, Rectangle source, Vector2 origin, float rotation, float scale, Color tint);
    void Clean();
}