#ifndef SESSION_H_
#define SESSION_H_

#include <deque>
#include <boost/asio.hpp>

#include "message.h"

typedef std::deque<Message> chat_message_queue;

class Room;

class Participant
{
public:
    virtual ~Participant() {}
    virtual void deliver(const Message& msg) = 0;
};

typedef std::shared_ptr<Participant> chat_participant_ptr;


class Session
    : public Participant,
    public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket socket, Room& room);

    void start();

    void deliver(const Message& msg);

private:
    void do_read_header();

    void do_read_body();

    void do_write();

    boost::asio::ip::tcp::socket socket_;
    Room& room_;
    Message read_msg_;
    chat_message_queue write_msgs_;
};


#endif