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

    void mafiaChat(const std::string& message);

    std::string getAlivePlayers();

    void play(bool add_word);

    void leave(const std::string& nickname);

    void readyForNextPhase();

    bool handleVoting(const std::string& previous, const std::string& suspicious);

    void checkVoting(const std::string& suspicious);

    bool handleVoting(const bool mafia);

    void handleKilling(const std::string& target);

    bool isNight();

    const std::string& getSuspicious() { return suspicious_; }

private:
    void assignRoles();

    void assignWord();

    bool gameOver();

private:
    Room& room_;

    std::string word_;
    std::map<std::string, std::shared_ptr<Player> > players_;
    std::set<std::shared_ptr<Player> > mafia_;

    std::unique_ptr<GameState> state_;

    std::map<std::string, int> ballot_box_;

    int number_of_survivors_;
    int number_of_mafia_;

    std::string suspicious_;
    int number_of_vote_;
    int number_of_vote_on_mafia_;
};

#endif