#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <string>
#include <memory>
#include <thread>
#include <boost/asio.hpp>

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
    void changeNextState(const boost::system::error_code& e, STATE state, int phase);

    STATE state_;
    Game& game_;
    int phase_;

    std::shared_ptr<std::thread> thread_;
    boost::asio::io_service io_;
    boost::asio::deadline_timer timer_;

    const int kDayTime = 300;
    const int kFinalStatementTime = 20;
    const int kNightTime = 20;
};
#endif