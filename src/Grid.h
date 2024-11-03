#pragma once

#include "Engine.h"
#include "Camera.h"
#include <string>
#include <bitset>

namespace Grid
{
    const i32 GROUND_SPRITES = 5;
    const i32 OBSTACLE_SPRITES = 3;

    struct Cell
    {
        i32 level = -1;

        bool initialized = false;
        i32 railType = -1;
        std::bitset<4> connectionPoints;

        Vector2 connectionPositions[2] = { {-1,-1},{-1,-1} };

        Vector2Int coordinate;
        Vector2 worldPosition;

        bool mouseOver   = false;
        bool selected    = false;
        bool clockwise   = true;
        bool hasMine     = false;
        bool hasStation  = false;
        bool hasObstacle = false;
        bool hasCrossing = false;
        bool buildable   = true;
        
        i32 groundSpriteIndex = 0;
        i32 obstacleSpriteIndex = 0;
    };

    struct Grid
    {
        i32 level = -1;

        Vector2 worldPosition = { 0,0 }; // This is where to draw the grid...
        i32 width  = 32;
        i32 height = 32;
        i32 cellSize = 32;
        std::unordered_map<Vector2Int, Cell, Vector2IntHash, Vector2IntEqual> cells;
        Textures::Sprite groundSprite[GROUND_SPRITES];
        Textures::Sprite obstacleSprites[OBSTACLE_SPRITES];

        inline Cell* GetCellAtWorldPosition(Vector2 worldPosition)
        {
            Vector2Int gridPosition = {i32(floor(worldPosition.x / cellSize)), i32(floor(worldPosition.y / cellSize))};
            if (cells.find(gridPosition) == cells.end())
            {
                return &cells[{-1, -1}];
                
            }
            return &cells[gridPosition];
        }

        inline Cell* CoordinateToCell(Vector2Int coordinate)
        {
            return &cells[coordinate];
        }


        inline void Update()
        {
            for (auto& [coordinate, cell] : cells)
            {
                cell.mouseOver = false;
            }

            Cell* cell = GetCellAtWorldPosition(GetMousePosition());
            cell->mouseOver = true;

            if (IsMouseButtonPressed(0))
            {
                cell->selected = !cell->selected;
            }
        }
    };

    Grid Init(i32 level, i32 width, i32 height, i32 cellSize);
    void AddToGrid(Grid& grid, Vector2Int topLeftCoordinate, i32 width, i32 height);
    void Draw(Grid& grid, GameCamera::Camera& camera);

    inline Vector2 GetWorldPosition(Vector2Int coordinate)
    {
        return { (float)(coordinate.x * CELL_SIZE), (float)(coordinate.y * CELL_SIZE) };
    }

    inline Vector2Int GetCoordinate(Vector2 worldPosition)
    {
        return { i32(floor(worldPosition.x / CELL_SIZE)), i32(floor(worldPosition.y / CELL_SIZE)) };
    }
}