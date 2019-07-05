#include "room.h"

#include "session.h"

void Room::join(chat_participant_ptr participant)
{
    participants_.insert(participant);
    for (auto msg : recent_msgs_)
        participant->deliver(msg);
}

void Room::leave(chat_participant_ptr participant)
{
    participants_.erase(participant);
}

void Room::deliver(const Message& msg)
{
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
        recent_msgs_.pop_front();

    for (auto participant : participants_)
        participant->deliver(msg);
}