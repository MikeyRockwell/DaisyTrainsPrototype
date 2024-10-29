#pragma once

#include "Engine.h"
#include <unordered_map>
#include <string>
#include <bitset>

namespace Grid
{
    struct Cell
    {
        i32 railType = -1;
        std::bitset<4> connectionPoints;

        Vector2 connectionPositions[2] = { {-1,-1},{-1,-1} };

        Vector2Int coordinate;
        Vector2 worldPosition;

        bool mouseOver = false;
        bool selected  = false;
        bool clockwise = true;
    };

    struct Grid
    {
        i32 width  = 32;
        i32 height = 32;
        i32 cellSize = 32;
        std::unordered_map
            <Vector2Int, Cell, Vector2IntHash, Vector2IntEqual> cells;

        inline Cell* GetCellAtWorldPosition(Vector2 worldPosition)
        {
            Vector2Int gridPosition = {i32(floor(worldPosition.x / cellSize)), i32(floor(worldPosition.y / cellSize))};
            return &cells[gridPosition];
        }

        inline Cell* CoordinateToCell(Vector2Int coordinate)
        {
            return &cells[coordinate];
        }

        inline Vector2 GetWorldPosition(Vector2Int coordinate)
        {
            return {(float)(coordinate.x * cellSize), (float)(coordinate.y * cellSize) };
        }

        inline void Update()
        {
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x <width; x++)
                {
                    Vector2Int coordinate = Vector2Int{x,y};
                    cells[coordinate].mouseOver = false;
                }
            }
            Cell* cell = GetCellAtWorldPosition(GetMousePosition());
            cell->mouseOver = true;

            if (IsMouseButtonPressed(0))
            {
                cell->selected = !cell->selected;
            }
        }

        inline void Draw()
        {
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    Vector2Int coordinate = Vector2Int{x,y};
                    Cell* cell = &cells[coordinate];
                    Rectangle rectangle = 
                    {
                        (float)cell->worldPosition.x, 
                        (float)cell->worldPosition.y , 
                        (float)cellSize, 
                        (float)cellSize
                    };

                    //Color color = cell->mouseOver ? SKYBLUE : GRAY;
                    DrawRectangleRec(rectangle, GRAY);
                    DrawRectangleLinesEx(rectangle, 0.5f, BLACK);
                    
                    if (cell->clockwise)
                    {
                        DrawText("C", x * cellSize, y * cellSize, 10, BLACK);
                    }
                    else
                    {
                        DrawText("CC", x * cellSize, y * cellSize, 10, BLACK);
                    }

            


                    // Coord debug
                    //std::string strX = std::to_string(coordinate.x);
                    //std::string strY = std::to_string(coordinate.y);
                    //std::string coordinateText = strX + "," + strY;
                    //DrawText(coordinateText.c_str(), x * cellSize, y * cellSize, 10, BLACK);
                }
            }
        }
    };

    inline Grid Init(i32 width, i32 height, i32 cellSize)
    {
        Grid grid;
        grid.width = width;
        grid.height = height;
        grid.cellSize = cellSize;

        for (int y = 0; y < grid.height; y++)
        {
            for (int x = 0; x <grid.width; x++)
            {
                Vector2Int coordinate = {x,y};
                Cell cell = 
                {
                    .coordinate = coordinate,
                    .worldPosition = grid.GetWorldPosition(coordinate)
                };
                grid.cells[coordinate] = cell;
            }
        }

        return grid;
    }
}