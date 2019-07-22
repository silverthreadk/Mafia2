#include "game_state.h"

#include "game.h"

GameState::GameState(Game& game) :
    state_(STATE::DAY),
    game_(game)
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
            game_.notify(game_.getSuspicious() + " can make the final statement.");
            game_.notify("If you think that " + game_.getSuspicious() + " is mafia, send </mafia>. If not, send </innocent>.");
            break;
        }
        case STATE::FINAL_STATEMENT: {
            state_ = STATE::NIGHT;
            game_.notify("Night started, be silent.");
            break;
        }
        case STATE::NIGHT: {
            state_ = STATE::DAY;
            break;
        }
    }
}