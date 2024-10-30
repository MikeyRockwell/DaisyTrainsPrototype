#pragma once

#include "Engine.h"
#include "Grid.h"
#include "UI.h"

namespace Trains
{
    const i32 MAX_TRAINS = 10000;

    enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    struct Train
    {
        Vector2 worldPosition;
        Vector2 entryPosition;
        Vector2 targetPosition;

        Grid::Cell* currentCell;
        Grid::Cell* nextCell;

        float speed;
        float acceleration;
        float maxSpeed;
        float rotation;
        float distanceToNextCell;
        float distanceTravelled;
        float tValue;

        bool loaded = false;

        Color cargoColor;
        i32   cargoCount;
    };

    struct UIButton
    {
        Rectangle bounds;
        Color color;
        bool hovered;
    };

    struct State
    {
        Vector2 trainPlacementPosition;
        i32 trainCount = 0;
        Train trains[MAX_TRAINS];
        Textures::Sprite trainSprite;
    };
    extern State state;

    void Init  ();
    void Update(Grid::Grid& grid);
    void Draw  (Grid::Grid& grid);

}