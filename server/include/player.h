#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <memory>

class Session;
class Room;
class Game;

class Player{
public:
    enum ROLE { INNOCENT, MAFIA };

    Player(Session& session, Room& room);

    ~Player();

    void notify(const std::string& message);

    void chat(const std::string& message);

    void changeNickname(const std::string& nickname);

    void startGame();

    void play(std::shared_ptr<Game> game, ROLE role = INNOCENT);

    void voteFor(const std::string& nickname);

    void vote(const bool mafia);

    bool die();

    void kill(const std::string& nickname);

    bool isMafia() { return role_ == MAFIA; }

    bool isAlive() { return !dead_; }

    std::string getNickname() { return nickname_; }

private:
    Session& session_;
    Room& room_;
    std::weak_ptr<Game> game_;
    std::string nickname_;
    ROLE role_;
    bool dead_;
    std::string suspicious_;
    bool voted_;
};

#endif