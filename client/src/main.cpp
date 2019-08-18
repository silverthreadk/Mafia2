#include <iostream>

#include "client.h"

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: Client <host> <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;

        boost::asio::ip::tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        Client c(io_context, endpoints);

        std::thread t([&io_context]() { io_context.run(); });

        char line[Message::kMaxBodyLength + 1];
        while (std::cin.getline(line, Message::kMaxBodyLength + 1)) {
            Message msg;
            msg.body_length(std::strlen(line));
            std::memcpy(msg.body(), line, msg.body_length());
            msg.encode_header();
            c.write(msg);
        }

        c.close();
        t.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
