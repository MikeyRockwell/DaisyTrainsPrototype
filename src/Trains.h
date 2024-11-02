#pragma once

#include "Engine.h"
#include "Grid.h"
#include "UI.h"

namespace Trains
{
    const i32 MAX_TRAINS = 64;
    const i32 MAX_CARS = 64;

    enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    enum Type
    {
        ENGINE,
        CAR
    };

    struct TrainEngine;

    struct TrainTransform
    {
        Type type;

        Vector2 worldPosition;
        Vector2 entryPosition;
        Vector2 targetPosition;
        Vector2 direction;

        Grid::Cell* currentCell;
        Grid::Cell* nextCell;

        float speed;
        float rotation;
        float distanceToNextCell;
        float distanceTravelled;
        float tValue;
    };

    struct Car
    {
        i32 index = -1;

        CargoType cargoType;
        
        TrainTransform transform;

        bool loaded = false;
    };

    // The trail engine
    struct TrainEngine
    {
        TrainTransform transform;

        float acceleration;
        float maxSpeed;
       
        i32 carCount = 1;
        Car cars[MAX_CARS];
    };

    struct UIButton
    {
        Rectangle bounds;
        Color color;
        bool hovered;
    };

    struct TrainState
    {
        Vector2 trainPlacementPosition;
        i32 trainCount = 0;
        TrainEngine trains[MAX_TRAINS];
        
        TrainEngine levelElevenTrains[4];
    };
    extern TrainState state;

    struct TrainResources
    {
        Textures::Sprite trainSprite;
        Textures::Sprite carSprites[PALETTE_SIZE];
        Textures::Sprite removeTrain;
    };
    extern TrainResources resources;

    void Init  ();
    void Update             (i32 level);
    void UpdateTransform    (i32 level, TrainEngine& engine, TrainTransform& transform);
    void UpdateCarTransform (i32 level, Car& car, TrainTransform& parentTransform, float speed);
    void EnterCell          (TrainTransform& transform);
    void Draw               (i32 level);

    inline void RemoveTrain(i32 index)
    {
        state.trains[index] = state.trains[state.trainCount - 1];
        state.trainCount--;
    }

}