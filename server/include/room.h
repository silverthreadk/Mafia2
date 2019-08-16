#ifndef ROOM_H_
#define ROOM_H_

#include <deque>
#include <set>
#include <memory>
#include <boost/asio.hpp>

#include "message.h"

typedef std::deque<Message> chat_message_queue;

class Game;
class Participant;
typedef std::shared_ptr<Participant> chat_participant_ptr;

class Room
{
public:
    Room();
    ~Room();

    void join(chat_participant_ptr participant);

    void leave(chat_participant_ptr participant);

    bool exist(chat_participant_ptr participant);

    void deliver(const Message& msg);

    bool addNickname(const std::string& nickname);

    bool changeNickname(const std::string& src, const std::string& dest);

    void deleteNickname(const std::string& nickname);

    void playGame(bool add_word);

    void gameOver();

private:
    std::set<chat_participant_ptr> participants_;
    std::set<std::string> nickname_info_;
    enum { max_recent_msgs = 100 };
    chat_message_queue recent_msgs_;
    std::shared_ptr<Game> game_;
};

#endif