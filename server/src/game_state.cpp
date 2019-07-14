#include "game_state.h"

GameState::GameState() :
    state_(STATE::DAY)
{
}

GameState::~GameState()
{
}

void GameState::changeNextState()
{
    switch (state_) {
        case STATE::DAY: {
            state_ = STATE::FINAL_STATEMENT;
            break;
        }
        case STATE::FINAL_STATEMENT: {
            state_ = STATE::NIGHT;
            break;
        }
        case STATE::NIGHT: {
            state_ = STATE::DAY;
            break;
        }
    }
}