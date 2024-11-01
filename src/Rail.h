#pragma once

#include "Engine.h"

#include "Grid.h"
#include "UI.h"
#include "Trains.h"


namespace Rail
{
    const i32 RAIL_TYPES = 6;

    enum ConnectionPoint
    {
        NORTH,
        EAST,
        SOUTH,
        WEST
    };

    // The actual rail pieces built
    enum RailType
    {   
        NONE            = -1,
        VERTICAL        =  0,
        HORIZONTAL      =  1,
        TOP_TO_LEFT     =  2, 
        TOP_TO_RIGHT    =  3,
        BOTTOM_TO_RIGHT =  4,
        BOTTOM_TO_LEFT  =  5,
    };

    struct UIButton
    {
        RailType type;
        Rectangle bounds;
        Color color;
        bool hovered;
    };

    struct Rail
    {
        RailType type;
        Vector2 start;
        Vector2 end;
        Vector2Int coordinate;

        //std::vector<Trains::TrainTransform*> trainsOnRail;
    };

    struct RailState
    {
        i32 railAvailable = 10;
        std::unordered_map<Vector2Int, Rail, Vector2IntHash, Vector2IntEqual> coordinateToRailMap;
    };
    extern RailState railState;

    struct UIState
    {
        RailType selectedType = RailType::VERTICAL;
        UIButton buttons[RAIL_TYPES];
        Rail selectedRail;
        bool clockwise = true;

        Textures::Sprite railSpritesClockwise[RAIL_TYPES];
        Textures::Sprite railSpritesCounterClockwise[RAIL_TYPES];
    };
    extern UIState uiState;

    void Init  ();
    void Update(i32 level);
    void Draw  (i32 level);
    void DrawUI(i32 level);

    inline Vector2 GetCellConnectionPoint(Grid::Cell* cell, ConnectionPoint point)
    {
        if (point == NORTH)
        {
            return Vector2Add(cell->worldPosition, { CELL_SIZE / 2.0f, 0.0f });
        }
        if (point == EAST)
        {
            return Vector2Add(cell->worldPosition, { CELL_SIZE, CELL_SIZE / 2.0f });
        }
        if (point == SOUTH)
        {
            return Vector2Add(cell->worldPosition, { CELL_SIZE / 2.0f, CELL_SIZE });
        }
        if (point == WEST)
        {
            return Vector2Add(cell->worldPosition, { 0.0f, CELL_SIZE / 2.0f });
        }
        return { 0,0 };
    }

    inline void SetConnectionPoints(Grid::Cell* cell, RailType type)
    {   
        if (type == VERTICAL)
        {
            cell->connectionPoints.set(NORTH);
            cell->connectionPoints.set(SOUTH);
            cell->connectionPositions[0] = { GetCellConnectionPoint(cell, NORTH)};
            cell->connectionPositions[1] = { GetCellConnectionPoint(cell, SOUTH)};
        }
        if (type == HORIZONTAL)
        {
            cell->connectionPoints.set(EAST);
            cell->connectionPoints.set(WEST);
            cell->connectionPositions[0] = { GetCellConnectionPoint(cell, EAST) };
            cell->connectionPositions[1] = { GetCellConnectionPoint(cell, WEST) };
        }
        if (type == TOP_TO_LEFT)
        {
            cell->connectionPoints.set(NORTH);
            cell->connectionPoints.set(WEST);
            cell->connectionPositions[0] = { GetCellConnectionPoint(cell, NORTH) };
            cell->connectionPositions[1] = { GetCellConnectionPoint(cell, WEST) };
        }
        if (type == TOP_TO_RIGHT)
        {
            cell->connectionPoints.set(NORTH);
            cell->connectionPoints.set(EAST);
            cell->connectionPositions[0] = { GetCellConnectionPoint(cell, NORTH) };
            cell->connectionPositions[1] = { GetCellConnectionPoint(cell, EAST) };
        }
        if (type == BOTTOM_TO_LEFT)
        {
            cell->connectionPoints.set(SOUTH);
            cell->connectionPoints.set(WEST);
            cell->connectionPositions[0] = { GetCellConnectionPoint(cell, SOUTH) };
            cell->connectionPositions[1] = { GetCellConnectionPoint(cell, WEST) };
        }
        if (type == BOTTOM_TO_RIGHT)
        {
            cell->connectionPoints.set(SOUTH);
            cell->connectionPoints.set(EAST);
            cell->connectionPositions[0] = { GetCellConnectionPoint(cell, SOUTH) };
            cell->connectionPositions[1] = { GetCellConnectionPoint(cell, EAST) };
        }
        cell->railType = type;
    }


    inline Vector2 GetClosestConnectionPoint(Grid::Cell* cell, Vector2 worldPosition)
    {   
        std::vector<Vector2> points;
        points.reserve(4);
        
        // Add all the points
        if (cell->connectionPoints.any())
        {
            if (cell->connectionPoints.test(NORTH))
            {
                Vector2 northPoint = Vector2Add(cell->worldPosition, { CELL_SIZE / 2.0f, 0.0f });
                points.emplace_back(northPoint);
            }
            if (cell->connectionPoints.test(EAST))
            {
                Vector2 eastPoint = Vector2Add(cell->worldPosition, { CELL_SIZE, CELL_SIZE / 2.0f });
                points.emplace_back(eastPoint);
            }
            if (cell->connectionPoints.test(SOUTH))
            {
                Vector2 southPoint = Vector2Add(cell->worldPosition, { CELL_SIZE / 2.0f, CELL_SIZE});
                points.emplace_back(southPoint);
            }
            if (cell->connectionPoints.test(WEST))
            {
                Vector2 westPoint = Vector2Add(cell->worldPosition, { 0.0f, CELL_SIZE / 2.0f});
                points.emplace_back(westPoint);
            }
        }

        if (points.size() > 0)
        {
            std::sort(points.begin(), points.end(), [&worldPosition](const Vector2& a, const Vector2& b)
            {
                return Vector2Distance(a, worldPosition) < Vector2Distance(b, worldPosition);
            });

            return points[0];
        }

        return {0,0};
    }

    inline ConnectionPoint WorldToConnectionPoint(Grid::Cell* cell, Vector2 worldPosition)
    {
        Vector2 closestPoint = GetClosestConnectionPoint(cell, worldPosition);
        Vector2 northPoint = Vector2Add(cell->worldPosition, { CELL_SIZE / 2.0f, 0.0f });
        Vector2 eastPoint = Vector2Add(cell->worldPosition, { CELL_SIZE, CELL_SIZE / 2.0f });
        Vector2 southPoint = Vector2Add(cell->worldPosition, { CELL_SIZE / 2.0f, CELL_SIZE });
        Vector2 westPoint = Vector2Add(cell->worldPosition, { 0.0f, CELL_SIZE / 2.0f });

        if (closestPoint.x == northPoint.x && closestPoint.y == northPoint.y) return NORTH;
        if (closestPoint.x == eastPoint.x && closestPoint.y == eastPoint.y) return EAST;
        if (closestPoint.x == southPoint.x && closestPoint.y == southPoint.y) return SOUTH;
        if (closestPoint.x == westPoint.x && closestPoint.y == westPoint.y) return WEST;
    }

  /*  inline void AddTrainToRailState(Trains::TrainEngine& train)
    {
        railState.coordinateToRailMap[train.currentCell->coordinate].trainsOnRail.emplace_back(&train);
    }

    inline void RemoveTrainFromRailState(Trains::TrainEngine& train)
    {
        Rail& rail = railState.coordinateToRailMap[train.currentCell->coordinate];

        for (int i = 0; i < rail.trainsOnRail.size(); i++)
        {
            if (rail.trainsOnRail[i] == &train)
            {
                rail.trainsOnRail.erase(rail.trainsOnRail.begin() + i);
                return;
            }
        }
        std::cout << "Error: RemoveTrainFromRailState" << std::endl;

    }*/

    Vector2 GetNextDestinationPoint(Grid::Cell* cell);
    Grid::Cell* GetNextCell(Grid::Cell* cell);
}
