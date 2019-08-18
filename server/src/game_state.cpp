#include "game_state.h"

#include <iostream>
#include <boost/bind.hpp>

#include "game.h"

GameState::GameState(const Game& game) :
    state_(STATE::NIGHT),
    game_(const_cast<Game&>(game)),
    phase_(0),
    timer_(boost::asio::deadline_timer(io_, boost::posix_time::seconds(kDayTime))),
    strand_(io_) {
}

GameState::~GameState() {
    io_.stop();
    timer_.cancel_one();
}

void GameState::changeNextState() {
    work_.reset();
    io_.stop();
    switch (state_) {
        case STATE::DAY: {
            state_ = STATE::FINAL_STATEMENT;
            game_.notify(game_.getSuspicious() + " can make the final statement.\n"
                "If you think that " + game_.getSuspicious() + " is mafia, send </mafia>. If not, send </innocent>.");
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
            game_.readyForNextPhase();
            game_.notify("Night has fallen.\n"
                "Players should debate the identities of the mafia and vote to eliminate a suspect.\n"
                "If you think the player is a suspect, send </vote nickname>\n"
                "If you want to check alive players, send </list>");
            timer_ = boost::asio::deadline_timer(io_, boost::posix_time::seconds(kDayTime));
            std::cout << "day" << std::endl;
            break;
        }
    }
    work_.reset(new boost::asio::io_service::work(io_));
    timer_.async_wait(boost::asio::bind_executor(strand_, boost::bind(
        &GameState::changeNextState,
        this,
        boost::asio::placeholders::error)));
    io_.restart();
    thread_ = boost::thread(boost::bind(&boost::asio::io_service::run, &io_));
}

void GameState::changeNextState(const boost::system::error_code& e) {
    if (e.value() == 0) {
        game_.notify("Time is over.");
        changeNextState();
    } else {
        std::cout << "error_code : " << e.message() << std::endl;
    }
}
