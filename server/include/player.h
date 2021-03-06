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

    Player(const Session& session, const Room& room, const std::string& nickname);

    ~Player();

    void notify(const std::string& message);

    void chat(const std::string& message);

    void changeNickname(const std::string& nickname);

    void notifyList();

    void startGame(bool add_word = false);

    void play(std::shared_ptr<Game> game, ROLE role = INNOCENT);

    void leave();

    void readyForNextPhase();

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

#endif  // PLAYER_H_
