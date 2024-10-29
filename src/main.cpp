#include "Engine.h"

#include "Grid.h"
#include "Rail.h"
#include "Trains.h"

#include <iostream>

const i32 screenWidth = 1600;
const i32 screenHeight = 960;

int main()
{
	std::cout << "Hello, World!" << std::endl;

	InitWindow(screenWidth, screenHeight, "Connex");

	Grid::Grid grid = Grid::Init(screenWidth / CELL_SIZE, screenHeight / CELL_SIZE, CELL_SIZE);
    Rail::Init(grid);

	while(!WindowShouldClose())
	{
		grid.Update();
        Rail::Update(grid);
		Trains::Update(grid);

		BeginDrawing();
		ClearBackground(RAYWHITE);

		// Grid drawing..

		grid.Draw();
		Rail::DrawUI(grid);
        Trains::Draw(grid);

		EndDrawing();
	}

	Textures::Clean();
	CloseWindow();

	return 0;
}