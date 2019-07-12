#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <memory>

class Session;
class Room;
class Game;

class Player{
public:
    enum ROLE { INOCCENT, MAFIA };

    Player(Session& session, Room& room);

    ~Player();

    void notify(const std::string& message);

    void chat(const std::string& message);

    void changeNickname(const std::string& nickname);

    void startGame();

    void play(std::shared_ptr<Game> game, ROLE role = INOCCENT);

    void voteTo(const std::string& nickname);

private:
    Session& session_;
    Room& room_;
    std::shared_ptr<Game> game_;
    std::string nickname_;
    ROLE role_;
    bool dead_;
    bool voted_;
};

#endif