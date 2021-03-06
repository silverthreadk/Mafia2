#ifndef SESSION_H_
#define SESSION_H_

#include <memory>
#include <deque>
#include <boost/asio.hpp>

#include "message.h"

typedef std::deque<Message> chat_message_queue;

class Room;
class Player;

class Participant {
 public:
    virtual ~Participant() {}
    virtual void deliver(const Message& msg) = 0;
};

typedef std::shared_ptr<Participant> chat_participant_ptr;


class Session
    : public Participant,
    public std::enable_shared_from_this<Session> {
 public:
    Session(boost::asio::ip::tcp::socket socket, const Room& room);

    void start();

    void deliver(const Message& msg);

    std::shared_ptr<Player> getPlayer() { return player_; }

 private:
    void do_read_header();

    void do_read_body();

    void do_write();

    boost::asio::ip::tcp::socket socket_;
    Room& room_;
    std::shared_ptr<Player> player_;
    Message read_msg_;
    chat_message_queue write_msgs_;
};


#endif  // SESSION_H_
