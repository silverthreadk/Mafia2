#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <string>

class GameState {
public:
    enum STATE {
        DAY,
        FINAL_STATEMENT,
        NIGHT
    };

    GameState();
    ~GameState();

    void changeNextState();

    bool isDay() { return state_ == DAY; }

    bool isFinalStatement() { return state_ == FINAL_STATEMENT; }

    bool isNight() { return state_ == NIGHT; }

private:
    STATE state_;
};
#endif