#pragma once

#include "Engine.h"

namespace Textures
{
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
        std::vector<Texture2D> textures;
    };
    extern State state;

    Texture* Load(const char* path);
    Sprite CreateSprite(Texture* texture, Rectangle source, Vector2 origin, float rotation, float scale, Color tint);
    void Clean();
}