#pragma once

#include "Engine.h"
#include "Grid.h"
#include "UI.h"

namespace Trains
{
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

        std::bitset<4> direction;

        float speed;
        float acceleration;
        float maxSpeed;
        float rotation;
        float distanceToNextCell;
        float distanceTravelled;

        Color color = RAYWHITE;
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
        std::vector<Train> trains;
    };
    extern State state;

    void Init  ();
    void Update(Grid::Grid& grid);
    void Draw  (Grid::Grid& grid);

    inline void EnterCell(Train& train, Grid::Cell* cell)
    {
    }

    inline void SetDirectionRight(Train& train)
    {
        train.direction.reset();
        train.direction.set(Direction::RIGHT);
    }
    
    inline void SetDirectionLeft(Train& train)
    {
        train.direction.reset();
        train.direction.set(Direction::LEFT);
    }

    inline void SetDirectionUp(Train& train)
    {
        train.direction.reset();
        train.direction.set(Direction::UP);
    }

    inline void SetDirectionDown(Train& train)
    {
        train.direction.reset();
        train.direction.set(Direction::DOWN);
    }

    inline void SetDirectionUpAndRight(Train& train)
    {
        train.direction.reset();
        train.direction.set(Direction::UP);
        train.direction.set(Direction::RIGHT);
    }

    inline void SetDirectionUpAndLeft(Train& train)
    {
        train.direction.reset();
        train.direction.set(Direction::UP);
        train.direction.set(Direction::LEFT);
    }

    inline void SetDirectionDownAndRight(Train& train)
    {
        train.direction.reset();
        train.direction.set(Direction::DOWN);
        train.direction.set(Direction::RIGHT);
    }

    inline void SetDirectionDownAndLeft(Train& train)
    {
        train.direction.reset();
        train.direction.set(Direction::DOWN);
        train.direction.set(Direction::LEFT);
    }

    
}