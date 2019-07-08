#include "player.h"

#include "session.h"
#include "room.h"
#include "game.h"


Player::Player(Session & session, Room & room) :
    session_(session),
    room_(room)
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
    notify("Nickname has changed to " + nickname_ + ".");
}

void Player::startGame() {
    room_.playGame();
}

void Player::play(std::shared_ptr<Game> game) {
    game_ = game;
}