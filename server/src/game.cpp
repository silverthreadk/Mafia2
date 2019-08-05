#include "game.h"

#include <algorithm>
#include <random>

#include "session.h"
#include "room.h"
#include "player.h"
#include "game_state.h"

Game::Game(std::set<chat_participant_ptr>& players, Room& room) :
    room_(room),
    state_(std::make_unique<GameState>(*this)),
    suspicious_(""),
    number_of_survivors_(0),
    number_of_mafia_(0),
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

void Game::mafiaChat(const std::string& message) {
    for (auto mafia : mafia_) {
        if (!mafia->isAlive()) continue;
        mafia->notify(message);
    }
}

std::string Game::getAlivePlayers() {
    std::string alive_players = "";
    for (auto player : players_) {
        if (!player.second->isAlive()) continue;
        alive_players += player.second->getNickname() + " / ";
    }
    return alive_players;
}

void Game::play()
{
    notify("At the start of the game, each player is secretly assigned a role affiliated with one of these teams.\n"
        "The game has two alternating phases : one, during which the mafia may covertly murder an innocent,\n"
        "and two, in which surviving players debate the identities of the mafia and vote to eliminate a suspect.\n"
        "The game continues until all of the mafia have been eliminated or until the mafia outnumber the innocents.\n"
        "Good luck!");
    assignRoles();
    state_->changeNextState();
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
        player_list[i]->notify("You are mafia!");
    }

    for (int i = number_of_mafia_; i < player_list.size(); ++i) {
        innocents_.insert(player_list[i]);
        player_list[i]->play(shared_from_this());
        player_list[i]->notify("You are innocent!");
    }
}

void Game::readyForNextPhase() {
    ballot_box_.clear();
    suspicious_ = "";
    number_of_vote_ = 0;
    number_of_vote_on_mafia_ = 0;

    for (auto player : players_) {
        player.second->readyForNextPhase();
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

    suspicious_ = suspicious;
    state_->changeNextState();
}

bool Game::handleVoting(const bool mafia)
{
    if (!state_->isFinalStatement()) return false;
    ++number_of_vote_;

    notify("Number of vote : " + std::to_string(number_of_vote_));

    if (mafia) {
        ++number_of_vote_on_mafia_;

        if (number_of_vote_on_mafia_ > number_of_survivors_ / 2) {
            players_[suspicious_]->die();
            --number_of_survivors_;
            if (players_[suspicious_]->isMafia()) {
                --number_of_mafia_;
                notify(suspicious_ + " is mafia.");
            } else {
                notify(suspicious_ + " is not mafia.");
            }
            if (gameOver()) {
                room_.gameOver();
                return true;
            }

            state_->changeNextState();
            return true;
        }
    }

    if (number_of_vote_ - number_of_vote_on_mafia_ > number_of_survivors_ / 2) {
        state_->changeNextState();
    }

    return true;
}

void Game::handleKilling(const std::string& target) {
    if (!players_[target]->die()) return;
    if (players_[target]->isMafia()) --number_of_mafia_;
    --number_of_survivors_;
    if (gameOver()) {
        room_.gameOver();
        return;
    }

    state_->changeNextState();
}

bool Game::gameOver() {
    if (number_of_mafia_ == 0) {
        notify("Innocents win!");
        return true;
    } else if (number_of_survivors_ - number_of_mafia_ <= number_of_mafia_) {
        notify("Mafia win!");
        return true;
    }
    return false;
}

bool Game::isNight() { return state_->isNight(); }

