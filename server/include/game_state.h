#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <string>
#include <memory>

class Game;

class GameState {
public:
    enum STATE {
        DAY,
        FINAL_STATEMENT,
        NIGHT
    };

    GameState(Game& game);
    ~GameState();

    void changeNextState();

    bool isDay() { return state_ == DAY; }

    bool isFinalStatement() { return state_ == FINAL_STATEMENT; }

    bool isNight() { return state_ == NIGHT; }

private:
    STATE state_;

    Game& game_;
};
#endif