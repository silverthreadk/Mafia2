#ifndef GAME_H_
#define GAME_H_

#include <memory>
#include <string>
#include <map>
#include <set>

class Player;
class Room;
class GameState;
class Participant;
typedef std::shared_ptr<Participant> chat_participant_ptr;

class Game :
    public std::enable_shared_from_this<Game> {
public:
    Game(std::set<chat_participant_ptr>& players_, Room& room);

    ~Game();

    void notify(const std::string& message);

    void play();

    bool handleVoting(const std::string& previous, const std::string& suspicious);

private:
    void assignRoles();

private:
    Room& room_;

    std::map<std::string, std::shared_ptr<Player> > players_;
    std::set<std::shared_ptr<Player> > mafia_;
    std::set<std::shared_ptr<Player> > innocents_;

    std::unique_ptr<GameState> state_;

    std::map<std::string, int> ballot_box_;

    int number_of_survivors_;
    int number_of_mafia_;
};

#endif