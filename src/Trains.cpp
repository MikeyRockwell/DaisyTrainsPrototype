#include "Curves.h"
#include "Game.h"
#include "Rail.h"
#include "Trains.h"

#include <string>
#include <iostream>
#include <cassert>
#include "Mines.h"

namespace Trains
{
    State state;

	void Init()
	{
        Texture* texture = Textures::Load("res/sprites/train.png");
        i32 width = texture->width;
        i32 height = texture->height;
        state.trainSprite = Textures::CreateSprite
        (
            texture,
            { 0,0, (float)width, (float)height },
            { (float)width / 2, (float)height / 2 },
            0.0f,
            1.0f,
            WHITE
        );
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
            
            Vector2 mousePosition = Game::state.mouseWorldPosition;
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
                    train.maxSpeed = 370.0f;
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


                    state.trains[state.trainCount++] = train;

                    // ADD TRAIN TO RAIL STATE
                    Rail::AddTrainToRailState(state.trains[state.trainCount-1]);
                }
            }
		}

        for (int i = 0; i < state.trainCount; i++)
        {
            Train& train = state.trains[i];
            // LOADING TRAIN
            if (train.currentCell->hasMine && !train.loaded)
            {
                Mines::Mine* mine = &Mines::state.mines[train.currentCell->coordinate];
                if (mine->count > 0)
                {
                    i32 amount = 1;
                    mine->count -= amount;
                    train.loaded = true;
                    train.cargoColor = mine->color;
                    train.cargoCount = amount;
                }
            }

            // UNLOADING TRAIN
            if (train.currentCell->hasStation && train.loaded)
            {
                Mines::Station* station = &Mines::state.stations[train.currentCell->coordinate];
                Game::state.currency += train.cargoCount;
                
                train.loaded = false;
                train.cargoCount = 0;
                station->color = train.cargoColor;
            }

            // Calculate the center of the current cell
            Vector2 cellCenter = {
                train.currentCell->worldPosition.x + grid.cellSize / 2.0f,
                train.currentCell->worldPosition.y + grid.cellSize / 2.0f
            };

            // Update the distance travelled
            train.distanceTravelled += train.speed * GetFrameTime();
            train.tValue = train.distanceTravelled / train.distanceToNextCell;
            if (train.tValue > 1.0f)
            {
                train.tValue = 1.0f;
            }

            // Offset the midpoint to create a control point for the curve
            Vector2 direction = Vector2Subtract(train.targetPosition, train.entryPosition);
            Vector2 perpendicular = { -direction.y, direction.x }; // Perpendicular vector
            perpendicular = Vector2Normalize(perpendicular);
            
            float offset = 1.0f; // Adjust this value to control the curvature
            Vector2 controlPoint = Vector2Add(cellCenter, Vector2Scale(perpendicular, offset));

            //Vector2 nextPosition = BezierQuadratic(train.entryPosition, controlPoint, train.targetPosition, t);
            Vector2 nextPosition = GetSplinePointBezierQuad(train.entryPosition, controlPoint, train.targetPosition, train.tValue);

            Vector2 facingDirection = Vector2Subtract(nextPosition, train.worldPosition);
            facingDirection = Vector2Scale(Vector2Normalize(facingDirection), 100.0f);
            train.worldPosition = nextPosition;

            train.rotation = atan2(facingDirection.y, facingDirection.x) * RAD2DEG;
            train.speed += train.acceleration * GetFrameTime();

            Rail::Rail& currentRail = Rail::railState.coordinateToRailMap[train.currentCell->coordinate];
            if (currentRail.trainsOnRail.size() > 1)
            {
                i32 trainIndex = -1;

                for (int j = 0; j < currentRail.trainsOnRail.size(); j++)
                {
                    if (currentRail.trainsOnRail[j] == &train)
                    {
                        trainIndex = j;
                        break;
                    }
                }

                if (trainIndex != -1 && trainIndex != 0)
                {
                    Train* trainInFront = currentRail.trainsOnRail[trainIndex - 1];
                    if (train.tValue < trainInFront->tValue)
                    {
                        train.speed = Clamp(train.speed, 0.0f, trainInFront->speed);
                        if (Vector2Distance(train.worldPosition, trainInFront->worldPosition) < 10.0f)
                        {
                            train.tValue = trainInFront->tValue - 0.333f;
                        }
                    }
                }
            }

            train.tValue = Clamp(train.tValue, 0.0f, 1.0f);

            if (train.speed > train.maxSpeed) train.speed = train.maxSpeed;

            if (train.tValue == 1.0f) // We have reached the next cell
            {
                if (train.nextCell->railType != -1) // Does this cell have rail
                {
                    // REMOVE TRAIN FROM CURRENT RAIL STATE
                    Rail::RemoveTrainFromRailState(train);

                    train.currentCell   = train.nextCell;
                    Rail::AddTrainToRailState(train);

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
            Color color = train.loaded ? train.cargoColor : WHITE;
            Rectangle destination =
            {
                train.worldPosition.x,
                train.worldPosition.y,
                state.trainSprite.texture->width,
                state.trainSprite.texture->height
            };
            DrawTexturePro
            (
                *state.trainSprite.texture,
                state.trainSprite.source,
                destination,
                { state.trainSprite.origin.x, state.trainSprite.origin.y },
                train.rotation,
                color
            );

            //DrawPoly(train.worldPosition, 3, 12, train.rotation, color);
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
