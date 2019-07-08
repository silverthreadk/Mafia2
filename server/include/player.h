#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <memory>

class Session;
class Room;
class Game;

class Player{
public:
    Player(Session& session, Room& room);

    ~Player();

    void notify(const std::string& message);

    void chat(const std::string& message);

    void changeNickname(const std::string& nickname);

    void startGame();

    void play(std::shared_ptr<Game> game);

private:
    Session& session_;
    Room& room_;
    std::shared_ptr<Game> game_;
    std::string nickname_;
};

#endif