#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;

struct Nick{
    string uuid;
    string name;
    int godziny;
    int deathcount;
    int gameexit;
    int netheriteszt;
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
// metody
    statlib(); // constructor

    ~statlib(); // destructor

    void lokalizacja(); // gets whitelist and statistics directory from "localization.conf", if file does not exists creates it

    json whitelistToJson(); // loads file to an json object (from nlohmann/json)

    vector<Nick> tablica(json whitelist); // creates Nick type array from json object

    // string podajNick();

    // string nickToUuid(vector<Nick> gracze, string nc);

    Nick stat(string uuid); // reads statistics and saves them to the array

    // void wypiszWGodzinach(int playtimeticks, string nc);

    void zapiszDoPliku(vector<Nick> gracze); // saves output to a file
};
