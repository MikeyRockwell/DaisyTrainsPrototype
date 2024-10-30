#include "Engine.h"

#include "Game.h"
#include "Grid.h"
#include "Rail.h"
#include "Trains.h"
#include "Mines.h"

#include <iostream>

const i32 screenWidth = 1600;
const i32 screenHeight = 960;

int main()
{
	std::cout << "Hello, World!" << std::endl;

	InitWindow(screenWidth, screenHeight, "Connex");

    SetTargetFPS(60);

	Grid::Grid grid = Grid::Init(screenWidth / CELL_SIZE, screenHeight / CELL_SIZE, CELL_SIZE);
    Rail::Init(grid);
	Mines::Init(grid);
	Game::Init();
	Trains::Init();

	while(!WindowShouldClose())
	{
        Game::Update(); // Update the camera, and the mouse position

		grid.Update();
        Rail::Update(grid);
		Trains::Update(grid);
		Mines::Update(grid);

		BeginDrawing();
        BeginMode2D(Game::state.camera.rlCamera);
		ClearBackground(PALETTE_WHITE);

		grid.Draw(Game::state.camera);
		Rail::Draw(grid);
        Trains::Draw(grid);
        Mines::Draw();

        EndMode2D();

        //Rail::DrawUI(grid);
		Game::Draw();

		EndDrawing();
	}

	Textures::Clean();
	CloseWindow();

	return 0;
}