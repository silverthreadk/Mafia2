#include "player.h"

#include "session.h"
#include "room.h"
#include "game.h"


Player::Player(Session & session, Room & room) :
    session_(session),
    room_(room),
    role_(INOCCENT),
    dead_(true),
    suspicious_(""),
    voted_(false)
{
}

Player::~Player()
{
}

void Player::notify(const std::string& message) {
    session_.deliver(Message(message));
}

void Player::chat(const std::string& message) {
    std::string chat_message = nickname_ + " : " + message;

    room_.deliver(Message(chat_message));
}

void Player::changeNickname(const std::string& nickname) {
    nickname_ = nickname;
    notify("Nickname has been changed to " + nickname_ + ".");
}

void Player::startGame() {
    room_.playGame();
}

void Player::play(std::shared_ptr<Game> game, ROLE role) {
    game_ = game;
    role_ = role;
    dead_ = false;
}

void Player::voteFor(const std::string& nickname) {
    if (!game_) return;
    if (dead_) return;

    if (!game_->handleVoting(suspicious_, nickname)) return;

    if (suspicious_ == "") {
        game_->notify(nickname_ + " voted for " + nickname + ".");
    } else {
        game_->notify(nickname_ + " has changed vote from " + suspicious_ + " to " + nickname + ".");
    }

    suspicious_ = nickname;

    game_->checkVoting(suspicious_);
}

void Player::vote(const bool mafia) {
    if (!game_) return;
    if (dead_) return;
    if (voted_) return;

    game_->handleVoting(mafia);

}

void Player::die() {
    if (dead_) return;
    dead_ = true;

    game_->notify(nickname_ + " died.");
}