#ifndef GAME_H_
#define GAME_H_

#include <memory>
#include <string>
#include <set>

class Player;
class Participant;
typedef std::shared_ptr<Participant> chat_participant_ptr;

class Game :
    public std::enable_shared_from_this<Game> {
public:
    Game(std::set<chat_participant_ptr>& players_);

    ~Game();

    void play();

private:
    void assignRoles();

private:
    std::set<std::shared_ptr<Player> > players_;
    std::set<std::shared_ptr<Player> > mafia_;
    std::set<std::shared_ptr<Player> > innocents_;

    int number_of_mafia_;
};

#endif