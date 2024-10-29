#include "Trains.h"
#include "Rail.h"
#include "Curves.h"

#include <string>
#include <iostream>
#include <cassert>

namespace Trains
{
    State state;

	void Init()
	{

	}

	void Update(Grid::Grid& grid)
	{
		// TODO: Put this in the UI
		if (IsKeyPressed(KEY_SPACE))
		{
			UI::state.placeTrain = !UI::state.placeTrain;
		}

        /*===============================================
            CREATE TRAIN
        ===============================================*/
		if (UI::state.placeTrain)
		{
			Rail::uiState.selectedType = Rail::RailType::NONE;
            
            Vector2 mousePosition = GetMousePosition();
            Grid::Cell* cell = grid.GetCellAtWorldPosition(mousePosition);

            if (cell->railType != -1)
            {
                state.trainPlacementPosition = Vector2Add(cell->worldPosition, { CELL_SIZE / 2.0f, CELL_SIZE / 2.0f });

                if (IsMouseButtonPressed(0))
                {
                    Train train;
                    train.worldPosition = state.trainPlacementPosition;
                    train.currentCell = cell;
                    train.nextCell = nullptr;
                    train.speed = 0.0f;
                    train.acceleration = 10.0f;
                    train.maxSpeed = 170.0f;
                    train.rotation = 0.0f;

                    train.entryPosition  = train.worldPosition;
                    train.targetPosition = Rail::GetNextDestinationPoint(cell);
                    train.distanceToNextCell = Vector2Distance(train.worldPosition, train.targetPosition);
                    if (cell->railType != Rail::RailType::VERTICAL && cell->railType != Rail::RailType::HORIZONTAL)
                    {
                        train.distanceToNextCell *= 1.5f;
                    }
                    train.distanceTravelled = 0.0f;
                    train.nextCell = Rail::GetNextCell(&grid, cell);
                    train.color = { (u8)GetRandomValue(0, 255), (u8)GetRandomValue(0, 255), (u8)GetRandomValue(0, 255), 255 };
                    state.trains.push_back(train);
                }
            }
		}

        for (Train& train : state.trains)
        {
            // Calculate the center of the current cell
            Vector2 cellCenter = {
                train.currentCell->worldPosition.x + grid.cellSize / 2.0f,
                train.currentCell->worldPosition.y + grid.cellSize / 2.0f
            };

            // Update the distance travelled
            train.distanceTravelled += train.speed * GetFrameTime();
            float t = train.distanceTravelled / train.distanceToNextCell;

            // Offset the midpoint to create a control point for the curve
            Vector2 direction = Vector2Subtract(train.targetPosition, train.entryPosition);
            Vector2 perpendicular = { -direction.y, direction.x }; // Perpendicular vector
            perpendicular = Vector2Normalize(perpendicular);
            
            float offset = 1.0f; // Adjust this value to control the curvature
            Vector2 controlPoint = Vector2Add(cellCenter, Vector2Scale(perpendicular, offset));

            Vector2 nextPosition = BezierQuadratic(train.entryPosition, controlPoint, train.targetPosition, t);

            Vector2 facingDirection = Vector2Subtract(nextPosition, train.worldPosition);
            facingDirection = Vector2Scale(Vector2Normalize(facingDirection), 100.0f);
            train.worldPosition = nextPosition;

            train.rotation = atan2(facingDirection.y, facingDirection.x) * RAD2DEG;
            train.speed += train.acceleration * GetFrameTime();

            if (train.speed > train.maxSpeed) train.speed = train.maxSpeed;

            if (Vector2Distance(train.worldPosition, train.targetPosition) < 1.0f)
            {
                // We have reached the next cell
                if (train.nextCell->railType != -1) // Does this cell have rail
                {
                    train.currentCell   = train.nextCell;
                    train.worldPosition = train.targetPosition;
                    train.entryPosition = train.worldPosition;
                    // Get the next rail piece
                    
                    train.targetPosition = Rail::GetNextDestinationPoint(train.nextCell);
                    train.distanceToNextCell = Vector2Distance(train.worldPosition, train.targetPosition);
                    train.distanceTravelled = 0.0f;
                    train.nextCell = Rail::GetNextCell(&grid, train.nextCell);
                }
                else
                {
                    // We have reached the final destination
                    train.speed = 0.0f;
                }
            }
        }
	}

	void Draw(Grid::Grid& grid)
	{
		if (UI::state.placeTrain)
		{
			Vector2 mousePosition = GetMousePosition();
			Grid::Cell* cell = grid.GetCellAtWorldPosition(mousePosition);

			if (cell->railType != -1)
			{
                DrawCircleV(state.trainPlacementPosition, 8, YELLOW);
			}
		}
        for (Train& train : state.trains)
        {
            DrawPoly(train.worldPosition, 3, 12, train.rotation, train.color);
            //DrawCircleV(train.targetPosition, 5, GREEN);
            //DrawCircleV(train.entryPosition, 5, YELLOW);
            //DrawCircleV(train.controlPoint, 5, MAGENTA);
            // Current cell debug
            //Grid::Cell* cell = &grid.cells[train.currentCell->coordinate];
            //Rectangle rectangle =
            //{
            //    (float)cell->worldPosition.x,
            //    (float)cell->worldPosition.y ,
            //    (float)grid.cellSize,
            //    (float)grid.cellSize
            //};
            //DrawRectangleRec(rectangle, Fade(YELLOW, 0.5f));

            //// Next cell debug
            //if (train.nextCell)
            //{
            //    cell = &grid.cells[train.nextCell->coordinate];
            //    rectangle =
            //    {
            //        (float)cell->worldPosition.x,
            //        (float)cell->worldPosition.y ,
            //        (float)grid.cellSize,
            //        (float)grid.cellSize
            //    };
            //    DrawRectangleRec(rectangle, Fade(GREEN, 0.5f));
            //}

        }

	}
}
