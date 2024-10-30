#include "Textures.h"

namespace Textures
{
    State state;
}

Texture2D* Textures::Load(const char* path)
{
    Texture2D texture = LoadTexture(path);
    state.textures[state.textureCount++] = texture;
    return &state.textures[state.textureCount-1];
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
}
