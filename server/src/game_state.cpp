#include "game_state.h"

#include <boost/bind.hpp>
#include <iostream>

#include "game.h"

GameState::GameState(Game& game) :
    state_(STATE::NIGHT),
    game_(game),
    phase_(0),
    timer_(boost::asio::deadline_timer(io_, boost::posix_time::seconds(kDayTime)))
{
}

GameState::~GameState()
{
    io_.stop();
    timer_.cancel_one();
}

void GameState::changeNextState()
{
    switch (state_) {
        case STATE::DAY: {
            state_ = STATE::FINAL_STATEMENT;
            game_.notify(game_.getSuspicious() + " can make the final statement.");
            game_.notify("If you think that " + game_.getSuspicious() + " is mafia, send </mafia>. If not, send </innocent>.");
            timer_ = boost::asio::deadline_timer(io_, boost::posix_time::seconds(kFinalStatementTime));
            std::cout << "final statement" << std::endl;
            break;
        }
        case STATE::FINAL_STATEMENT: {
            state_ = STATE::NIGHT;
            game_.notify("Night started, be silent.");
            game_.mafiaChat("Mafia are allowed to chat to decide on whom to kill. If you decided to kill, send </kill nickname>");
            timer_ = boost::asio::deadline_timer(io_, boost::posix_time::seconds(kNightTime));
            std::cout << "night" << std::endl;
            break;
        }
        case STATE::NIGHT: {
            state_ = STATE::DAY;
            ++phase_;
            timer_ = boost::asio::deadline_timer(io_, boost::posix_time::seconds(kDayTime));
            std::cout << "day" << std::endl;
            break;
        }
    }

    timer_.async_wait(boost::bind(
        &GameState::changeNextState,
        this,
        boost::asio::placeholders::error,
        state_,
        phase_));
    thread_ = std::make_shared<std::thread>([&] {io_.run(); });
    thread_->detach();
}

void GameState::changeNextState(const boost::system::error_code& e, STATE state, int phase)
{
    if (e.value() == 0 && phase_ == phase && state_ == state) {
        game_.notify("Time is over.");
        changeNextState();
    } else {
        std::cout << "error_code : " << e.message() << std::endl;
    }
}