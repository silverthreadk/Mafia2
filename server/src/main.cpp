#include <iostream>

#include "server.h"

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: Server <port> [<port> ...]\n";
            return 1;
        }

        boost::asio::io_context io_context;

        std::list<Server> servers;
        for (int i = 1; i < argc; ++i) {
            boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), std::atoi(argv[i]));
            servers.emplace_back(io_context, endpoint);
        }

        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
