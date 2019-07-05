#ifndef ROOM_H_
#define ROOM_H_

#include <deque>
#include <set>
#include <boost/asio.hpp>

#include "message.h"

typedef std::deque<Message> chat_message_queue;

class Participant;
typedef std::shared_ptr<Participant> chat_participant_ptr;

class Room
{
public:
    void join(chat_participant_ptr participant);

    void leave(chat_participant_ptr participant);

    void deliver(const Message& msg);

private:
    std::set<chat_participant_ptr> participants_;
    enum { max_recent_msgs = 100 };
    chat_message_queue recent_msgs_;
};

#endif