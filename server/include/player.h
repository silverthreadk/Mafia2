#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>

class Session;
class Room;

class Player{
public:
    Player(Session& session, Room& room);

    ~Player();

    void chat(const std::string& message);

    void changeNickname(const std::string& nickname);



private:
    Session& session_;
    Room& room_;
    std::string nickname_;
};

#endif