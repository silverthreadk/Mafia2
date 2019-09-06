#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

class Game;

class GameState {
 public:
    enum STATE {
        DAY,
        FINAL_STATEMENT,
        NIGHT
    };

    explicit GameState(const Game& game);

    ~GameState();

    void changeNextState();

    bool isDay() { return state_ == DAY; }

    bool isFinalStatement() { return state_ == FINAL_STATEMENT; }

    bool isNight() { return state_ == NIGHT; }

 private:
    void changeNextState(const boost::system::error_code& e);

    STATE state_;
    Game& game_;
    int phase_;

    boost::asio::io_service io_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;
    boost::asio::deadline_timer timer_;
    boost::thread thread_;
    boost::asio::io_context::strand strand_;

    const int kDayTime = 300;
    const int kFinalStatementTime = 20;
    const int kNightTime = 20;
};
#endif  // GAME_STATE_H_
