#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;

struct Nick{
    string uuid;
    string name;
};

class statlib
{
private:
    string pathstat;
    string pathwhitelist;
public:
// zmienne
    json whitelist;
    vector<Nick> gracze;
    string nc;
    string uuid;
    int playtimeticks;
// metody
    statlib();

    ~statlib();

    void lokalizacja();

    json whitelistToJson();

    vector<Nick> tablica(json whitelist);

    string podajNick();

    string nickToUuid(vector<Nick> gracze, string nc);

    int czasTick(string uuid);

    void wypiszWGodzinach(int playtimeticks, string nc);
};
