#ifndef SERVER_H_
#define SERVER_H_

#include <boost/asio.hpp>
#include "message.h"
#include "room.h"

class Server
{
public:
    Server(boost::asio::io_context& io_context,
        const boost::asio::ip::tcp::endpoint& endpoint);

private:
    void do_accept();

    boost::asio::ip::tcp::acceptor acceptor_;
    Room room_;
};

#endif