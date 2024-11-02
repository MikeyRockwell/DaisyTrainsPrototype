#include "UI.h"
#include "Game.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

namespace UI
{
    State state;
    
    void Init()
    {
        i32 xPos = (GetScreenWidth() / 2) - (COUNT * BUTTON_SIZE) / 2;
        
        for (i32 i = 0; i < COUNT; i++)
        {
            state.buttons[i].rectangle = { (float)xPos, 0, (float)BUTTON_SIZE, (float)BUTTON_SIZE};
            state.buttons[i].hovered = false;
            state.buttons[i].selected = false;
            state.buttons[i].type = (BuildType)i;

            switch (state.buttons[i].type)
            {
            case NONE:
            {
                state.buttons[i].sprite = Trains::resources.carSprite;
            } break;
            case RAIL:
            {
                state.buttons[i].sprite = Rail::uiState.railSpritesClockwise[0];
            } break;
            case TRAIN:
            {
                state.buttons[i].sprite = Trains::resources.trainSprite;
            } break;
            case MINE:
            {
                state.buttons[i].sprite = Textures::CreateSprite
                (
                    Mines::resources.mineTextures[PINK_CARGO],
                    { 0,0,CELL_SIZE, CELL_SIZE },
                    { CELL_SIZE / 2, CELL_SIZE / 2 },
                    0.0f,
                    1.0f,
                    WHITE
                );
                
            } break;
            case STATION:
            {
                state.buttons[i].sprite = Textures::CreateSprite
                (
                    Mines::resources.stationTextures[PINK_CARGO],
                    { 0,0,CELL_SIZE,CELL_SIZE },
                    { CELL_SIZE / 2, CELL_SIZE / 2 },
                    0.0f,
                    1.0f,
                    WHITE
                );
            } break;
            }

            xPos += BUTTON_SIZE;
        }
    }

    void Update()
    {
        Vector2 mouseScreenSpace = GetMousePosition();

        bool mouseOver = false;
        for (int i = 0; i < COUNT; i++)
        {
            state.buttons[i].hovered = CheckCollisionPointRec(mouseScreenSpace, state.buttons[i].rectangle);
            mouseOver = mouseOver || state.buttons[i].hovered;
            if (state.buttons[i].hovered && IsMouseButtonPressed(0))
            {
                state.buttons[i].selected = !state.buttons[i].selected;
                state.buildType = state.buttons[i].type;

                for (int j = 0; j < COUNT; j++)
                {
                    if (j != i)
                    {
                        state.buttons[j].selected = false;
                    }
                }
            }
        }
        state.mouseOverUI = mouseOver;
    }

    void DrawWorldSpace()
    {
        for (int i = 0; i < Game::LEVEL_COUNT; i++)
        {
            Game::Level& level = Game::state.levels[i];
            if (level.unlocked)
            {
                DrawText(level.helpText.c_str(), level.UIPosition.x, level.UIPosition.y, 20, level.helpTextColor);
            }
        }
    }

    void DrawScreenSpace()
    {
        // Draw the toolbar...
        for (int i = 0; i < COUNT; i++)
        {
            Button& button = state.buttons[i];
            Color color = button.selected ? PALETTE_ORANGE : button.hovered ? PALETTE_BLUE : PALETTE_LIGHT_GRAY;
            DrawRectangleRec(button.rectangle, color);
            DrawTexturePro
            (
                *button.sprite.texture,
                button.sprite.source,
                button.rectangle,
                { 0,0 },
                0.0f,
                WHITE
            );
            DrawRectangleLinesEx(button.rectangle, 1, BLACK);
            if (button.type == TRAIN)
            {
                std::string trainCount = std::to_string(Trains::state.trainsAvailable);
                DrawText(trainCount.c_str(), button.rectangle.x + 10, button.rectangle.y + 10, 35, PALETTE_WHITE);
            }
            if (button.type == RAIL)
            {
                std::string trackCount = std::to_string(Rail::railState.railAvailable);
                DrawText(trackCount.c_str(), button.rectangle.x + 10, button.rectangle.y + 10, 35, PALETTE_WHITE);
            }
            if (button.type == MINE)
            {
                std::string mineCount = std::to_string(Mines::state.minesAvailable);
                DrawText(mineCount.c_str(), button.rectangle.x + 10, button.rectangle.y + 10, 35, PALETTE_WHITE);
            }
            if (button.type == STATION)
            {
                std::string stationCount = std::to_string(Mines::state.stationsAvailable);
                DrawText(stationCount.c_str(), button.rectangle.x + 10, button.rectangle.y +10, 35, PALETTE_WHITE);
            }

        }

        // Draw the build type
        const char* buildType = "NONE";
        switch (state.buildType)
        {
        case NONE: buildType = "NONE"; break;
        case RAIL: buildType = "RAIL"; break;
        case TRAIN: buildType = "TRAIN"; break;
        case MINE: buildType = "MINE"; break;
        case STATION: buildType = "STATION"; break;
        }
        DrawText(buildType, 10, 100, 20, WHITE);
        const char* railTypeText = "NONE";
        switch (Rail::uiState.selectedType)
        {
        case Rail::RailType::NONE: railTypeText = "NONE"; break;
        case Rail::RailType::VERTICAL: railTypeText = "VERTICAL"; break;
        case Rail::RailType::HORIZONTAL: railTypeText = "HORIZONTAL"; break;
        case Rail::RailType::TOP_TO_LEFT: railTypeText = "TOP_TO_LEFT"; break;
        case Rail::RailType::TOP_TO_RIGHT: railTypeText = "TOP_TO_RIGHT"; break;
        case Rail::RailType::BOTTOM_TO_RIGHT: railTypeText = "BOTTOM_TO_RIGHT"; break;
        case Rail::RailType::BOTTOM_TO_LEFT: railTypeText = "BOTTOM_TO_LEFT"; break;
        }
        DrawText(railTypeText, 10, 120, 20, WHITE);
        const char* mouseOverUI = state.mouseOverUI ? "TRUE" : "FALSE";
        DrawText(mouseOverUI, 10, 140, 20, WHITE);
    }
}