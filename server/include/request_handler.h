#ifndef REQUEST_HANDLER_H_
#define REQUEST_HANDLER_H_

#include <string>
#include <memory>
#include <vector>

class Message;
class Player;

class RequestHandler {
 public:
    static void handleRequest(const Message& message, std::shared_ptr<Player> player);

    static std::vector<std::string> parse(const Message& message);
};

#endif  // REQUEST_HANDLER_H_
