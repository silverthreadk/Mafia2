#include "request_handler.h"

#include <string>
#include <vector>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

#include "message.h"
#include "player.h"

RequestHandler::RequestHandler()
{
}

void RequestHandler::handleRequest(const Message& msg, std::shared_ptr<Player> player)
{
    const std::vector<std::string> command = RequestHandler::parse(msg);

    if (command.size() == 1) {
        if (command[0] == "/START") {
            player->startGame();
        }
    } else if (command.size() >= 2) {
        if (command[0] == "/CHAT") {
            player->chat(command[1]);
        } else if (command[0] == "/NICK") {
            player->changeNickname(command[1]);
        }
    }
}

std::vector<std::string> RequestHandler::parse(const Message& msg)
{
    std::string message(msg.body(), msg.body_length());

    if (message.at(0) != '/') {
        std::vector<std::string> command;
        command.push_back("/CHAT");
        command.push_back(message);

        return command;
    }

    std::vector<std::string> command;
    boost::split(command, message, boost::is_any_of(" \t"), boost::token_compress_on);

    boost::to_upper(command[0]);

    return command;
}