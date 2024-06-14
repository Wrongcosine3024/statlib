#include "statlib.h"

statlib::statlib(){
    lokalizacja();
    whitelist = whitelistToJson();
    gracze = tablica(whitelist);
    //nc = podajNick();
    //uuid = nickToUuid(gracze,nc);
    //playtimeticks = czasTick(uuid);
    zapiszDoPliku(gracze);
}

statlib::~statlib(){
    whitelist = NULL;
    nc = "";
    uuid = "";
    playtimeticks = 0;
}

void statlib::lokalizacja(){
    string wh = "";
    string stat = "";
    cout<<"Podaj folder z plikiem whitelisty: ";
    cin>>wh;
    pathwhitelist = wh;
    cout<<"Podaj folder ze statystykami: ";
    cin>>stat;
    pathstat = stat;
}

json statlib::whitelistToJson(){
    ifstream whitelist;
    string file;
    file = pathwhitelist+"whitelist.json";
    whitelist.open(file);
    if (!whitelist.is_open()) {
        std::cerr << "Nie można otworzyć pliku JSON." << std::endl;
        exit(1);
    }
    json j;
    whitelist >> j;
    whitelist.close();
    return j;
}

vector<Nick> statlib::tablica(json j){
    vector<Nick> gracze;
    for (const auto& item : j)
        {
            Nick nick;
            nick.uuid = item["uuid"].get<string>();
            nick.name = item["name"].get<string>();
            nick.godziny = 0;
            gracze.push_back(nick);
        }
    return gracze;
}

string statlib::podajNick(){
    cout<<"Nick gracza: ";
    string nc;
    cin>>nc;
    return nc;
}

string statlib::nickToUuid(vector<Nick> gracze, string nc){
    string uuid = "";
    for(int i = 0; i<gracze.size(); i++){
        if(gracze[i].name == nc){
            uuid = gracze[i].uuid;
            break;
        }
    }
    if (uuid == "")
    {
        cout<<"Podana nazwa nie znajduje sie w pliku whitelist.json"<<endl;
        exit(1);
    }else{
        cout<<uuid<<endl;
        return uuid;
    }
}

int statlib::czasGodz(string uuid){
    string file = pathstat+uuid+".json";
    ifstream stats;
    stats.open(file);
    if(!stats.good()){
        cout<<"Nie mozna otworzyc pliku statystyk! "<<file<<endl;
        //exit(1);
        return 0;
    }else{
        json s;
        stats >> s;
        stats.close();
        int wersja = s["DataVersion"].get<int>();
        int playtimehours = 0;
        if (wersja<2711)
        {
            int playtimeticks = 0;
            try{
                playtimeticks = s["stats"]["minecraft:custom"]["minecraft:play_one_minute"].get<int>();
            }
            catch(...){
                cout<<"Blad przy odczytaniu statystki";
                exit(1);
            }
            playtimehours = playtimeticks/72000;
            return playtimehours;
        }else{
            int playtimeticks = 0;
            try{
                playtimeticks = s["stats"]["minecraft:custom"]["minecraft:play_time"].get<int>();
            }
            catch(...){
                cout<<"Blad przy odczytaniu statystki";
                exit(1);
            }
            playtimehours = playtimeticks/72000;
            return playtimehours;
        }
    }
    return 0;
}

// int statlib::wypiszWGodzinach(int playtimeticks, string nc){
//     int playtimehours = playtimeticks/72000;
//     //cout<<"Czas gry gracza "<<nc<<" to: "<<playtimehours<<"h"<<endl;
//     return playtimehours;
// }

void statlib::zapiszDoPliku(vector<Nick> gracze){
    ofstream plik;
    plik.open("stat.out", ios::out);
    for (int i = 0; i < gracze.size(); i++)
    {
        gracze[i].godziny = czasGodz(gracze[i].uuid);
        plik<<gracze[i].name<<" "<<gracze[i].uuid<<" "<<gracze[i].godziny<<endl;
    }

    // for (int i = 0; i < gracze.size(); i++)
    // {
    //     cout<<gracze[i].name<<" "<<gracze[i].godziny<<endl;
    // }
}
