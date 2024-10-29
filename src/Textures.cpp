#include "Textures.h"

namespace Textures
{
    State state;
}

Texture2D* Textures::Load(const char* path)
{
    Texture2D texture = LoadTexture(path);
    state.textures.push_back(texture);
    return &state.textures.back();
}

Textures::Sprite Textures::CreateSprite(Texture* texture, Rectangle source, Vector2 origin, float rotation, float scale, Color tint)
{
    if (texture)
    {
        Sprite sprite   = {};
        sprite.texture  = texture;
        sprite.source   = source;
        sprite.origin   = origin;
        sprite.rotation = rotation;
        sprite.scale    = scale;
        sprite.tint     = tint;
        return sprite;
    }
}

void Textures::Clean()
{
    for (const Texture2D& texture : state.textures)
    {
        UnloadTexture(texture);
    }
    state.textures.clear();
}
