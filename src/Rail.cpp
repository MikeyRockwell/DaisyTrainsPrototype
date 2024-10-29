#include "Rail.h"
#include "Trains.h"

#include <iostream>

namespace Rail
{
    RailState railState;
    UIState uiState;

    const i32 UI_BLOCK_SIZE  = 64;

    void Init(Grid::Grid& grid)
    {
        for (int i = 0; i < RAIL_TYPES; i++)
        {
            i32 screenCenter = GetScreenWidth() / 2;
            i32 x = screenCenter - (UI_BLOCK_SIZE * RAIL_TYPES / 2) + UI_BLOCK_SIZE * i;
            i32 y = GetScreenHeight() - UI_BLOCK_SIZE;
            uiState.buttons[i].bounds = { (float)x, (float)y, (float)UI_BLOCK_SIZE, (float)UI_BLOCK_SIZE };
            uiState.buttons[i].color = DARKGRAY;
            uiState.buttons[i].hovered = false;
            //state.buttons[i].clicked = false;
        }

        // Load the rail sprites - Horizontally packed
        Texture* texture = Textures::Load("res/sprites/tracks_directional.png");

        // VERTICAL
        Rectangle source = { 0, 0, CELL_SIZE, CELL_SIZE };
        Vector2 origin = { CELL_SIZE / 2, CELL_SIZE / 2 };
        uiState.railSpritesClockwise[VERTICAL] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // HORIZONTAL
        source = { CELL_SIZE, 0, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesClockwise[HORIZONTAL] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // TOP TO LEFT
        source = { CELL_SIZE * 2, 0, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesClockwise[TOP_TO_LEFT] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // TOP TO RIGHT
        source = { CELL_SIZE * 3, 0, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesClockwise[TOP_TO_RIGHT] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // BOTTOM TO RIGHT
        source = { CELL_SIZE * 4, 0, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesClockwise[BOTTOM_TO_RIGHT] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // BOTTOM TO LEFT
        source = { CELL_SIZE * 5, 0, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesClockwise[BOTTOM_TO_LEFT] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);

        // Counter clockwise
        // VERTICAL
        source = { 0, CELL_SIZE, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesCounterClockwise[VERTICAL] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // HORIZONTAL
        source = { CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesCounterClockwise[HORIZONTAL] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // TOP TO LEFT
        source = { CELL_SIZE * 2, CELL_SIZE, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesCounterClockwise[TOP_TO_LEFT] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // TOP TO RIGHT
        source = { CELL_SIZE * 3, CELL_SIZE, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesCounterClockwise[TOP_TO_RIGHT] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // BOTTOM TO RIGHT
        source = { CELL_SIZE * 4, CELL_SIZE, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesCounterClockwise[BOTTOM_TO_RIGHT] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
        // BOTTOM TO LEFT
        source = { CELL_SIZE * 5, CELL_SIZE, CELL_SIZE, CELL_SIZE };
        uiState.railSpritesCounterClockwise[BOTTOM_TO_LEFT] = Textures::CreateSprite(texture, source, origin, 0.0f, 1.0f, WHITE);
    }

    void Update(Grid::Grid& grid)
    {
        Vector2 mousePosition = GetMousePosition();
        Grid::Cell* cell = grid.GetCellAtWorldPosition(mousePosition);
        
        Vector2 cellCenter = { cell->worldPosition.x + grid.cellSize / 2, cell->worldPosition.y + grid.cellSize / 2 };
        
        // Current rail segment type
        RailType flags = RailType::NONE;

        /*=========================================================
            RAIL MOUSE OVER GRID
        =========================================================*/

        // CALCULATE THE UI RAIL SEGMENT
        if (uiState.selectedType == RailType::VERTICAL)
        {
            flags = RailType::VERTICAL;
            uiState.selectedRail.start = { cell->worldPosition.x + grid.cellSize / 2, cell->worldPosition.y };
            uiState.selectedRail.end = { cell->worldPosition.x + grid.cellSize / 2, cell->worldPosition.y + grid.cellSize };
        }
        if (uiState.selectedType == RailType::HORIZONTAL)
        {
            flags = RailType::HORIZONTAL;
            uiState.selectedRail.start = { cell->worldPosition.x, cell->worldPosition.y + grid.cellSize / 2 };
            uiState.selectedRail.end = { cell->worldPosition.x + grid.cellSize, cell->worldPosition.y + grid.cellSize / 2 };
        }
        if (uiState.selectedType == RailType::TOP_TO_LEFT)
        {
            flags = RailType::TOP_TO_LEFT;
            uiState.selectedRail.start = { cell->worldPosition.x + grid.cellSize / 2, cell->worldPosition.y };
            uiState.selectedRail.end = { cell->worldPosition.x, cell->worldPosition.y + grid.cellSize / 2 };
        }
        if (uiState.selectedType == RailType::TOP_TO_RIGHT)
        {
            flags = RailType::TOP_TO_RIGHT;
            uiState.selectedRail.start = { cell->worldPosition.x + grid.cellSize / 2, cell->worldPosition.y };
            uiState.selectedRail.end = { cell->worldPosition.x + grid.cellSize, cell->worldPosition.y + grid.cellSize / 2 };
        }
        if (uiState.selectedType == RailType::BOTTOM_TO_RIGHT)
        {
            flags = RailType::BOTTOM_TO_RIGHT;
            uiState.selectedRail.start = { cell->worldPosition.x + grid.cellSize / 2, cell->worldPosition.y + grid.cellSize };
            uiState.selectedRail.end = { cell->worldPosition.x + grid.cellSize, cell->worldPosition.y + grid.cellSize / 2 };
        }
        if (uiState.selectedType == RailType::BOTTOM_TO_LEFT)
        {
            flags = RailType::BOTTOM_TO_LEFT;
            uiState.selectedRail.start = { cell->worldPosition.x + grid.cellSize / 2, cell->worldPosition.y + grid.cellSize };
            uiState.selectedRail.end = { cell->worldPosition.x, cell->worldPosition.y + grid.cellSize / 2 };
        }

        /*=========================================================
            CREATE RAIL SEGMENT
        =========================================================*/
        if (IsMouseButtonDown(0))
        {
            if (uiState.selectedType != RailType::NONE && cell->railType == -1)
            {
                Rail rail;
                rail.type = uiState.selectedType;
                rail.start = uiState.selectedRail.start;
                rail.end   = uiState.selectedRail.end;
                rail.coordinate = cell->coordinate;
                cell->clockwise = uiState.clockwise;
                railState.rails.push_back(rail);
                SetConnectionPoints(cell, flags);
            }
        }

        /*=========================================================
            SCROLL TO CHANGE RAIL SEGMENT
        =========================================================*/
        if (GetMouseWheelMove() > 0)
        {
            uiState.selectedType = (RailType)((uiState.selectedType + 1) % RAIL_TYPES);
        }
        if (GetMouseWheelMove() < 0)
        {
            uiState.selectedType = (RailType)((uiState.selectedType - 1) % RAIL_TYPES);
        }
        if (uiState.selectedType < 0) uiState.selectedType = (RailType)(RAIL_TYPES - 1);
        if (uiState.selectedType > RAIL_TYPES - 1) uiState.selectedType = (RailType)0;

        /*=========================================================
            DELETE RAIL SEGMENT
        =========================================================*/
        if (IsMouseButtonDown(1))
        {
            if (cell->railType != -1)
            {
                cell->railType = -1;
                cell->connectionPoints.reset();
                cell->connectionPositions[0] = { -1,-1 };
                cell->connectionPositions[1] = { -1,-1 };
                // Remove the rail from the rail state
                for (int i = 0; i < railState.rails.size(); i++)
                {
                    if (railState.rails[i].coordinate.x == cell->coordinate.x && railState.rails[i].coordinate.y == cell->coordinate.y)
                    {
                        railState.rails.erase(railState.rails.begin() + i);
                    }
                }
            }
        }

        /*========================================================
            FLIP RAIL SEGMENT DIRECTION
        =========================================================*/
        if (IsMouseButtonPressed(2))
        {
            if (cell->railType != -1)
            {
                cell->clockwise = !cell->clockwise;
            }
            uiState.clockwise = !uiState.clockwise;
        }


        /*=========================================================
            UPDATE UI BUTTONS
        =========================================================*/
        for (int i = 0; i < RAIL_TYPES; i++)
        {
            if (CheckCollisionPointRec(mousePosition, uiState.buttons[i].bounds))
            {
                uiState.buttons[i].hovered = true;
                uiState.buttons[i].color = RAYWHITE;
                if (IsMouseButtonPressed(0))
                {
                    UI::state.placeTrain = false;
                    if (uiState.selectedType == i)
                    {
                        uiState.selectedType = RailType::NONE;
                    }
                    else
                    {
                        uiState.selectedType = (RailType)i;
                    }
                }
            }
            else
            {
                uiState.buttons[i].hovered = false;
            }
        }
    }

    void DrawUI(Grid::Grid& grid)
    {   
        // DRAW THE RAIL SEGMENT AT THE MOUSE POSITION
        Vector2 mousePosition = GetMousePosition();
        Grid::Cell* cell = grid.GetCellAtWorldPosition(mousePosition);

        // DRAW THE RAIL GHOST
        if (uiState.selectedType != RailType::NONE)
        {
            Color ghostColor = cell->railType != -1 ? RED : SKYBLUE;
            Rectangle bounds =
            {
                cell->worldPosition.x,
                cell->worldPosition.y,
                CELL_SIZE,
                CELL_SIZE
            };
            if (uiState.clockwise)
            {
                DrawTexturePro
                (
                    *uiState.railSpritesClockwise[uiState.selectedType].texture,
                    uiState.railSpritesClockwise[uiState.selectedType].source,
                    bounds,
                    { 0,0 },
                    0.0f,
                    ghostColor
                );
            }
            else
            {
                DrawTexturePro
                (
                    *uiState.railSpritesCounterClockwise[uiState.selectedType].texture,
                    uiState.railSpritesCounterClockwise[uiState.selectedType].source,
                    bounds,
                    { 0,0 },
                    0.0f,
                    ghostColor
                );
            }
           
            DrawRectanglePro(bounds, {0,0}, 0, Fade(ghostColor, 0.5f));
        }

        // DRAW THE RAILS
        for (int i = 0; i < railState.rails.size(); i++)
        {
            Rail* rail = &railState.rails[i];
            Grid::Cell* cell = grid.CoordinateToCell(rail->coordinate);
            Rectangle destination =
            {
                cell->worldPosition.x,
                cell->worldPosition.y,
                CELL_SIZE,
                CELL_SIZE
            };

            if (cell->clockwise)
            {
                DrawTexturePro
                (
                    *uiState.railSpritesClockwise[rail->type].texture,
                    uiState.railSpritesClockwise[rail->type].source,
                    destination,
                    { 0,0 },
                    0.0f,
                    uiState.railSpritesClockwise[rail->type].tint
                );
            }
            else
            {
                DrawTexturePro
                (
                    *uiState.railSpritesCounterClockwise[rail->type].texture,
                    uiState.railSpritesCounterClockwise[rail->type].source,
                    destination,
                    { 0,0 },
                    0.0f,
                    uiState.railSpritesCounterClockwise[rail->type].tint
                );
            }
        }

        if (cell->connectionPositions[0].x != -1)
        {
            DrawCircleV(cell->connectionPositions[0], 5, RED);
        }
        if (cell->connectionPositions[1].x != -1)
        {
            DrawCircleV(cell->connectionPositions[1], 5, RED);
        }
        
        // DRAW THE UI BUTTONS
        for (int i = 0; i < RAIL_TYPES; i++)
        {
            UIButton& button = uiState.buttons[i];
            Color color = DARKGRAY;
            if (button.hovered && i != uiState.selectedType)
            {
                color = ORANGE;
            }
            else if (i == uiState.selectedType)
            {
                color = YELLOW;
            }

            DrawRectangleRec(button.bounds, color);
            DrawRectangleLinesEx(button.bounds, 1, BLACK);
            
            DrawTexturePro
            (
                *uiState.railSpritesClockwise[i].texture,
                uiState.railSpritesClockwise[i].source,
                button.bounds,
                {0,0},
                uiState.railSpritesClockwise[i].rotation,
                uiState.railSpritesClockwise[i].tint
            );
        }

        // DRAW CLOCKWISE INDICATOR DEBUG
        const char* clockwise = uiState.clockwise ? "C" : "CC";
        DrawText(clockwise, 10, 10, 20, uiState.clockwise ? GREEN : RED);
    }

    Vector2 GetNextDestinationPoint(Grid::Cell* cell)
    {
        RailType railPiece = (RailType)cell->railType;

        float halfCellSize = CELL_SIZE / 2.0f;

        Vector2 cellCenter = { cell->worldPosition.x + halfCellSize, cell->worldPosition.y + halfCellSize };
        Vector2 top   = { cellCenter.x, cellCenter.y - halfCellSize };
        Vector2 bot   = { cellCenter.x, cellCenter.y + halfCellSize };
        Vector2 left  = { cellCenter.x - halfCellSize, cellCenter.y };
        Vector2 right = { cellCenter.x + halfCellSize, cellCenter.y };

        // These coordinates are relative to the cell top left corner
        if (cell->clockwise)
        {
            // Clockwise means up and to the left....?
            if (railPiece == VERTICAL)        return top;
            if (railPiece == HORIZONTAL)      return left;
            if (railPiece == TOP_TO_LEFT)     return left;
            if (railPiece == TOP_TO_RIGHT)    return top;
            if (railPiece == BOTTOM_TO_RIGHT) return right;
            if (railPiece == BOTTOM_TO_LEFT)  return bot;
        }
        else
        {
            // Counter clockwise means down and to the right....?
            if (railPiece == VERTICAL)        return bot;
            if (railPiece == HORIZONTAL)      return right;
            if (railPiece == TOP_TO_LEFT)     return top;
            if (railPiece == TOP_TO_RIGHT)    return right;
            if (railPiece == BOTTOM_TO_RIGHT) return bot;
            if (railPiece == BOTTOM_TO_LEFT)  return left;
        }
        
        std::cout << "Error: GetNextDestinationPoint" << std::endl;
    }
    Grid::Cell* GetNextCell(Grid::Grid* grid, Grid::Cell* cell)
    {
        RailType railPiece = (RailType)cell->railType;

        if (cell->clockwise)
        {
            if (railPiece == VERTICAL)        return grid->CoordinateToCell({cell->coordinate.x, cell->coordinate.y - 1});
            if (railPiece == HORIZONTAL)      return grid->CoordinateToCell({cell->coordinate.x - 1, cell->coordinate.y});
            if (railPiece == TOP_TO_LEFT)     return grid->CoordinateToCell({cell->coordinate.x - 1, cell->coordinate.y});
            if (railPiece == TOP_TO_RIGHT)    return grid->CoordinateToCell({cell->coordinate.x, cell->coordinate.y - 1});
            if (railPiece == BOTTOM_TO_RIGHT) return grid->CoordinateToCell({cell->coordinate.x + 1, cell->coordinate.y});
            if (railPiece == BOTTOM_TO_LEFT)  return grid->CoordinateToCell({ cell->coordinate.x, cell->coordinate.y + 1 });
        }
        else
        {
            if (railPiece == VERTICAL)        return grid->CoordinateToCell({cell->coordinate.x, cell->coordinate.y + 1});
            if (railPiece == HORIZONTAL)      return grid->CoordinateToCell({cell->coordinate.x + 1, cell->coordinate.y});
            if (railPiece == TOP_TO_LEFT)     return grid->CoordinateToCell({cell->coordinate.x, cell->coordinate.y - 1});
            if (railPiece == TOP_TO_RIGHT)    return grid->CoordinateToCell({cell->coordinate.x + 1, cell->coordinate.y});
            if (railPiece == BOTTOM_TO_RIGHT) return grid->CoordinateToCell({cell->coordinate.x, cell->coordinate.y + 1});
            if (railPiece == BOTTOM_TO_LEFT)  return grid->CoordinateToCell({ cell->coordinate.x - 1, cell->coordinate.y });
        }
        
        return nullptr;

    }
}