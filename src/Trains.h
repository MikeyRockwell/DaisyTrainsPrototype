#pragma once

#include "Engine.h"
#include "Grid.h"
#include "UI.h"

namespace Trains
{
    //const i32 MAX_TRAINS = 64;
    const i32 MAX_CARS = 64;
    const float MIN_TRAIN_SPAWN_DISTANCE = 300.0f;

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
        i32 index = -1;
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
        bool trainPlacementValid = false;
        float trainAudioTimer = 0.0f;
        float trainAudioLength = 5.794f;
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
    void Update             ();
    void UpdateTransform    (TrainEngine& engine, TrainTransform& transform);
    void UpdateCarTransform (Car& car, TrainTransform& parentTransform, float speed);
    void EnterCell          (TrainTransform& transform);
    void Draw               ();

    void RemoveTrain(i32 index);
}