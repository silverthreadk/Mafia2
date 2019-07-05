#ifndef CLIENT_H_
#define CLIENT_H_

#include <deque>
#include <boost/asio.hpp>
#include "message.h"

typedef std::deque<Message> chat_message_queue;

class Client
{
public:
    Client(boost::asio::io_context& io_context,
        const boost::asio::ip::tcp::resolver::results_type& endpoints);

    void write(const Message& msg);

    void close();

private:
    void do_connect(const boost::asio::ip::tcp::resolver::results_type& endpoints);

    void do_read_header();

    void do_read_body();

    void do_write();

private:
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::socket socket_;
    Message read_msg_;
    chat_message_queue write_msgs_;
};

#endif