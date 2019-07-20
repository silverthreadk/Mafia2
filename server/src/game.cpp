#include "game.h"

#include <algorithm>
#include <random>

#include "session.h"
#include "room.h"
#include "player.h"
#include "game_state.h"

Game::Game(std::set<chat_participant_ptr>& players, Room& room) :
    room_(room),
    state_(std::make_unique<GameState>()),
    suspicious_(""),
    number_of_vote_(0),
    number_of_vote_on_mafia_(0)
{
    for (auto player : players) {
        auto session = std::dynamic_pointer_cast<Session>(player);
        players_[session->getPlayer()->getNickname()] = session->getPlayer();
    }
}

Game::~Game()
{
}

void Game::notify(const std::string& message) {
    room_.deliver(Message(message));
}

void Game::play()
{
    assignRoles();
}

void Game::assignRoles()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<std::shared_ptr<Player> > player_list;
    std::transform(players_.begin(), players_.end(), back_inserter(player_list), [](std::pair<std::string, std::shared_ptr<Player> > const & pair) {
        return pair.second;
    });
    std::shuffle(player_list.begin(), player_list.end(), g);

    number_of_survivors_ = players_.size();
    number_of_mafia_ = number_of_survivors_ / 3;

    for (int i = 0; i < number_of_mafia_; ++i) {
        mafia_.insert(player_list[i]);
        player_list[i]->play(shared_from_this(), Player::ROLE::MAFIA);
        player_list[i]->notify("you are mafia!");
    }

    for (int i = number_of_mafia_; i < player_list.size(); ++i) {
        innocents_.insert(player_list[i]);
        player_list[i]->play(shared_from_this());
        player_list[i]->notify("you are innocent!");
    }
}

bool Game::handleVoting(const std::string& previous, const std::string& suspicious)
{
    if (!state_->isDay()) return false;

    auto it = players_.find(suspicious);
    if (it == players_.end()) return false;

    ballot_box_[suspicious] += 1;

    it = players_.find(previous);
    if (it != players_.end()) {
        ballot_box_[previous] -= 1;
    }

    return true;
}

void Game::checkVoting(const std::string& suspicious)
{
    if (ballot_box_[suspicious] <= number_of_survivors_ / 2) return;

    state_->changeNextState();
    suspicious_ = suspicious;
    notify(suspicious + " can make the final statement.");
    notify("If you think that " + suspicious + " is mafia, send </mafia>. If not, send </innocent>.");
}

bool Game::handleVoting(const bool mafia)
{
    if (!state_->isFinalStatement()) return false;
    ++number_of_vote_;

    if (mafia) {
        ++number_of_vote_on_mafia_;

        if (number_of_vote_on_mafia_ > number_of_survivors_ / 2) {
            players_[suspicious_]->die();
            --number_of_survivors_;
            if (players_[suspicious_]->isMafia()) {
                notify(suspicious_ + " is mafia.");
            } else {
                notify(suspicious_ + " is not mafia.");
            }
            state_->changeNextState();
            notify("Night started, be silent.");
            return true;
        }
    }

    if (number_of_vote_ - number_of_vote_on_mafia_ > number_of_survivors_ / 2) {
        state_->changeNextState();
        notify("Night started, be silent.");
    }

    return true;
}
