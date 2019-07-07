#include "player.h"

#include "Session.h"
#include "Room.h"


Player::Player(Session & session, Room & room) :
    session_(session),
    room_(room)
{
}

Player::~Player()
{
}

void Player::chat(const std::string& message) {
    std::string chat_message = nickname_ + " : " + message;

    room_.deliver(Message(chat_message));
}

void Player::changeNickname(const std::string& nickname) {
    nickname_ = nickname;
    session_.deliver(Message("Nickname has changed to " + nickname_ + "."));
}