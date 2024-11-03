#pragma once

#include "Engine.h"
#include "Grid.h"
#include "Rail.h"

namespace Mines
{
    struct Cargo
    {
        CargoType cargoType;
        i32 count;
    };

    struct Stack
    {
        Vector2Int coordinate;
        Vector2 worldPosition;
        CargoType cargoType;
        i32 count = 0;
        i32 capacity = 200;
    };

    // FOR PICKUPS
    //struct Mine
    //{
    //    CargoType cargoType;

    //    Vector2Int coordinate;
    //    Vector2 worldPosition;

    //    Stack* stack;
    //    
    //    bool vertical = false;
    //    Rail::RailType compatibleRailType;

    //    Textures::Sprite sprite;
    //};

    // FOR DELIVERIES
    struct Station
    {
        bool pickup = false;

        CargoType cargoType;

        Vector2Int coordinate;
        Vector2 worldPosition;

        bool hasStack = false;
        Stack* stack;

        Rail::RailType compatibleRailType;

        Textures::Sprite sprite;

        bool vertical = false;
    };

    // FOR CONVERTING
    struct Converter
    {
        Vector2Int inputCoordinate;
        Vector2Int outputCoordinate;
        Vector2Int stackCoordinate;

        Vector2 inputWorldPosition;
        Vector2 outputWorldPosition;
        Vector2 stackWorldPosition;

        CargoType input;
        CargoType output;
        i32 outputCount = 0;
        i32 capacity = 200;
    };

    struct MineState
    {
        bool buildableCell = false;
        bool flipped = false;

        //std::unordered_map<Vector2Int, Mine,      Vector2IntHash, Vector2IntEqual> mines;
        std::unordered_map<Vector2Int, Station,   Vector2IntHash, Vector2IntEqual> stations;
        std::unordered_map<Vector2Int, Stack,     Vector2IntHash, Vector2IntEqual> stacks;

        Stack* adjacentStack = nullptr;
    };
    extern MineState state;
    
    struct Resources
    {
        Texture* mineTextures[PALETTE_SIZE];
        Texture* stationTextures[PALETTE_SIZE];
        Texture* containerTextures[PALETTE_SIZE];

        Texture* mineGhostTexture;
        Texture* stationGhostTexture;

        Textures::Sprite mineGhostSprite;
        Textures::Sprite stationGhostSprite;
    };
    extern Resources resources;

    void     Init();
    //Mine*    CreateMine     (i32 level, Grid::Cell* cell, i32 capacity, CargoType cargo, bool flipped);
    Station* CreateStation  (i32 level, Grid::Cell* cell, CargoType cargo, bool flipped);
    Stack*   CreateStack    (Vector2Int coordinate, CargoType cargo, i32 capacity, i32 count);
    void     Update         (i32 level);
    void     Draw           (i32 level);
    void     DrawStack      (Stack& stack);
    
    bool IsRailCompatible(Grid::Cell* cell);
    bool IsMineCompatible(Grid::Cell* cell, bool flipped);
}