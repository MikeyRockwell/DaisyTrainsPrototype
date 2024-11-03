#include "Engine.h"
#include "Audio.h"
#include "Game.h"
#include "Grid.h"
#include "Rail.h"
#include "Trains.h"
#include "Mines.h"

#include <iostream>

//#define PLATFORM_WEB

#ifdef PLATFORM_WEB
#include <emscripten.h>
#endif

#ifdef PLATFORM_WEB
const i32 screenWidth = 1600;
const i32 screenHeight = 900;
#else
const i32 screenWidth = 2560;
const i32 screenHeight = 1440;
#endif


void UpdateAndDraw();

int main()
{
	std::cout << "Hello, World!" << std::endl;

	InitWindow(screenWidth, screenHeight, "Connex");

    SetTargetFPS(60);
	
    Audio::Init();
	Game::Init();
    Rail::Init();
	Mines::Init();
	Trains::Init();
	UI::Init();

    Game::AddLevel(0);

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(UpdateAndDraw, 0, 1);
#else
	while(!WindowShouldClose())
	{
        UpdateAndDraw();
	}
#endif

    Audio::Clean();
	Textures::Clean();
	CloseWindow();

	return 0;
}

void UpdateAndDraw()
{
    Audio::Update();
	Game::Update(); // Update the camera, and the mouse position
	Rail::Update(0);
	Trains::Update();
	Mines::Update(0);
	UI::Update();

	Game::CheckLevelStates();

	BeginDrawing();
	BeginMode2D(Game::state.camera.rlCamera);
	ClearBackground(PALETTE_BLACK);

	Grid::Draw(Game::state.grid, Game::state.camera);
	Rail::Draw(0);
	Trains::Draw();
	Mines::Draw(0);
	Game::Draw();

	UI::DrawWorldSpace();

	EndMode2D();
	//DrawFPS(20, 20);
	UI::DrawScreenSpace();

	EndDrawing();
}