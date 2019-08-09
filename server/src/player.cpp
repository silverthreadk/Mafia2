#include "player.h"

#include <cstdio>
#include <regex>
#include <boost/uuid/sha1.hpp>

#include "session.h"
#include "room.h"
#include "game.h"


static std::string convert(const std::string& p_arg) {
    boost::uuids::detail::sha1 sha1;
    sha1.process_bytes(p_arg.data(), p_arg.size());
    unsigned hash[5] = { 0 };
    sha1.get_digest(hash);

    char buf[9] = { 0 };

    std::sprintf(buf, "%08x", hash[4]);

    return std::string(buf+3);
}

static bool isValid(const std::string& name) {
    const std::regex pattern("([\xb0-\xc8]|[\xa1-\xfe]|[a-zA-Z0-9])+");
    return std::regex_match(name, pattern);
}

Player::Player(Session& session, Room& room, const std::string& nickname) :
    session_(session),
    room_(room),
    nickname_(convert(nickname)),
    role_(INNOCENT),
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
        if (dead_) return;
    }

    room_.deliver(Message(chat_message));
}

void Player::changeNickname(const std::string& nickname) {
    if (!game_.expired()) {
        notify("Allow to change the nickname only before the game is started.");
        return;
    }
    if (!isValid(nickname)) {
        notify("Nickname is invalid.");
        return;
    }

    nickname_ = nickname;
    notify("Nickname has been changed to " + nickname_ + ".");
}

void Player::notifyList() {
    if (game_.expired()) {
        return;
    }
    auto game = game_.lock();

    notify("Survivors : " + game->getAlivePlayers());
}

void Player::startGame() {
    if (!game_.expired()) {
        notify("Game is already running.");
        return;
    }
    room_.playGame();
}

void Player::readyForNextPhase() {
    suspicious_ = "";
    voted_ = false;
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

bool Player::die() {
    if (game_.expired()) return false;
    if (dead_) return false;
    dead_ = true;

    auto game = game_.lock();
    game->notify(nickname_ + " died.");

    return true;
}