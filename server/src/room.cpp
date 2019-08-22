#include "room.h"

#include "session.h"
#include "game.h"

Room::Room() :
    game_(nullptr) {
}

Room::~Room() {
}

void Room::join(chat_participant_ptr participant) {
    participants_.insert(participant);
    for (auto msg : recent_msgs_)
        participant->deliver(msg);
    if (participants_.size() == 3) {
        deliver(Message("If you want to play a game, send </start> or </start word>"));
    }
}

void Room::leave(chat_participant_ptr participant) {
    participants_.erase(participant);
}

bool Room::exist(chat_participant_ptr participant) {
    return participants_.find(participant) != participants_.end();
}

void Room::deliver(const Message& msg) {
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
        recent_msgs_.pop_front();

    for (auto participant : participants_)
        participant->deliver(msg);
}

std::string Room::getPlayerNicknames() {
    std::string player_nicknames = "";
    for (auto nickname : nickname_info_) {
        player_nicknames += nickname + " / ";
    }

    return player_nicknames;
}

bool Room::addNickname(const std::string& nickname) {
    return nickname_info_.insert(nickname).second;
}

bool Room::changeNickname(const std::string& src, const std::string& dest) {
    if (nickname_info_.insert(dest).second) {
        nickname_info_.erase(src);
        return true;
    }

    return false;
}

void Room::deleteNickname(const std::string& nickname) {
    nickname_info_.erase(nickname);
}

bool Room::playGame(bool add_word) {
    if (participants_.size() < 3) return false;
    game_ = std::make_shared<Game>(participants_, *this);
    game_->play(add_word);

    return true;
}

void Room::gameOver() {
    game_ = nullptr;
}
