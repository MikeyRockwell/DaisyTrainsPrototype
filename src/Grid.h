#pragma once

#include "Engine.h"
#include "Camera.h"
#include <string>
#include <bitset>

namespace Grid
{
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
        bool buildable   = true;
    };

    struct Grid
    {
        i32 level = -1;

        Vector2 worldPosition = { 0,0 }; // This is where to draw the grid...
        i32 width  = 32;
        i32 height = 32;
        i32 cellSize = 32;
        std::unordered_map<Vector2Int, Cell, Vector2IntHash, Vector2IntEqual> cells;
        Textures::Sprite groundSprite;

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

        inline void Draw(GameCamera::Camera& camera)
        {
            for (auto& [coordinate, cell] : cells)
            {
                Rectangle rectangle = 
                {
                    (float)cell.worldPosition.x, 
                    (float)cell.worldPosition.y , 
                    (float)cellSize, 
                    (float)cellSize
                };

                DrawTexturePro
                (
                    *groundSprite.texture,
                    groundSprite.source,
                    rectangle,
                    { 0,0 },
                    0.0f,
                    WHITE
                );
                //DrawRectangleRec(rectangle, PALETTE_BLACK);
                //DrawRectangleLinesEx(rectangle, 0.5f / camera.rlCamera.zoom, PALETTE_LIGHT_GREEN);

                //Color color = cell->mouseOver ? SKYBLUE : GRAY;
                    
                /*if (cell->clockwise)
                {
                    DrawText("C", x * cellSize, y * cellSize, 10, BLACK);
                }
                else
                {
                    DrawText("CC", x * cellSize, y * cellSize, 10, BLACK);
                }*/

                // Coord debug
                //std::string strX = std::to_string(coordinate.x);
                //std::string strY = std::to_string(coordinate.y);
                //std::string coordinateText = strX + "," + strY;
                //DrawText(coordinateText.c_str(), x * cellSize, y * cellSize, 10, BLACK);
            }
        }
    };

    Grid Init(i32 level, i32 width, i32 height, i32 cellSize);
    void AddToGrid(Grid& grid, Vector2Int topLeftCoordinate, i32 width, i32 height);

    inline Vector2 GetWorldPosition(Vector2Int coordinate)
    {
        return { (float)(coordinate.x * CELL_SIZE), (float)(coordinate.y * CELL_SIZE) };
    }

    inline Vector2Int GetCoordinate(Vector2 worldPosition)
    {
        return { i32(floor(worldPosition.x / CELL_SIZE)), i32(floor(worldPosition.y / CELL_SIZE)) };
    }
}