#pragma once
#include "classes/Player.h"
namespace ClassGame {
    void GameStartUp();
    void RenderGame();
    void EndOfTurn();
    void EndOfGame(Player* winner);
    void StaleMate();
}
