#include "game.h"

#include <algorithm>
#include <random>

#include "session.h"
#include "player.h"

Game::Game(std::set<chat_participant_ptr>& players)
{
    for (auto player : players) {
        auto session = std::dynamic_pointer_cast<Session>(player);
        players_[session->getPlayer()->getNickname()] = session->getPlayer();
    }
}

Game::~Game()
{
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

    number_of_mafia_ = players_.size() / 3;

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

bool Game::handleVoting(const std::string& nickname)
{
    return true;
}