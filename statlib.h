#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;

struct Nick{
    string uuid; 
    string name; // player nick
};

class statlib
{
private:
    string pathstat; // statistics directory
    string pathwhitelist; // whitelist directory
public:
// zmienne/variables
    json whitelist;
    vector<Nick> gracze; // array of players created from whitelist object
    string nc; // nick
    string uuid;
    int playtimeticks; // playtime in ticks
// metody/methods
    statlib(); // constructor

    ~statlib(); // destructor

    void lokalizacja(); // prompts for whitelist.json and stats directory

    json whitelistToJson(); // loads file to an json object (from nlohmann/json)

    vector<Nick> tablica(json whitelist); // creates Nick type array from json object

    string podajNick(); // prompts for nick

    string nickToUuid(vector<Nick> gracze, string nc); // returns uuid for given nick from players array

    int czasTick(string uuid); // returns playtime in ticks for given uuid

    void wypiszWGodzinach(int playtimeticks, string nc); // prints out playtime in hours (instead of ticks)
};
