#include "player.h"

#include "session.h"
#include "room.h"
#include "game.h"


Player::Player(Session& session, Room& room) :
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

    if (!game_.expired()) {
        auto game = game_.lock();
        if (game->isNight()) {
            if (role_ == MAFIA) game->mafiaChat(chat_message);
            return;
        }
    }

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
    if (game_.expired()) return;
    if (dead_) return;
    auto game = game_.lock();

    if (!game->handleVoting(suspicious_, nickname)) return;

    if (suspicious_ == "") {
        game->notify(nickname_ + " voted for " + nickname + ".");
    } else {
        game->notify(nickname_ + " has changed vote from " + suspicious_ + " to " + nickname + ".");
    }

    suspicious_ = nickname;

    game->checkVoting(suspicious_);
}

void Player::vote(const bool mafia) {
    if (game_.expired()) return;
    if (dead_) return;
    if (voted_) return;

    auto game = game_.lock();
    game->handleVoting(mafia);
}

void Player::kill(const std::string& nickname) {
    if (game_.expired()) return;
    if (dead_) return;
    if (!isMafia()) return;

    auto game = game_.lock();
    if (!game->isNight()) return;

    game->handleKilling(nickname);
}

void Player::die() {
    if (game_.expired()) return;
    if (dead_) return;
    dead_ = true;

    auto game = game_.lock();
    game->notify(nickname_ + " died.");
}