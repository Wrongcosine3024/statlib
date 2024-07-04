#include "statlib.h"

statlib::statlib(){
    lokalizacja();
    whitelist = whitelistToJson();
    gracze = tablica(whitelist);
    nc = podajNick();
    uuid = nickToUuid(gracze,nc);
    playtimeticks = czasTick(uuid);
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
    cout<<"Whitelist directory: ";
    cin>>wh;
    pathwhitelist = wh;
    cout<<"Statistics directory: ";
    cin>>stat;
    pathstat = stat;
}

json statlib::whitelistToJson(){
    ifstream whitelist;
    string file;
    file = pathwhitelist+"whitelist.json";
    whitelist.open(file);
    if (!whitelist.is_open()) {
        std::cerr << "Could not open JSON file." << std::endl;
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
    for(int i = 0; i<=gracze.size(); i++){
        if(gracze[i].name == nc){
            uuid = gracze[i].uuid;
            break;
        }
    }
    if (uuid == "")
    {
        cout<<"Given nick does not exists in whitelist.json"<<endl;
        exit(1);
    }else{
        cout<<uuid<<endl;
        return uuid;
    }
}

int statlib::czasTick(string uuid){
    string file = pathstat+uuid+".json";
    ifstream stats;
    stats.open(file);
    if(!stats.good()){
        cout<<"Could not open statistics file "<<file;
        exit(1);
    }
    json s;
    stats >> s;
    stats.close();
    int wersja = s["DataVersion"].get<int>();
    if (wersja<2711)
    {
        int playtimeticks = 0;
        try{
            playtimeticks = s["stats"]["minecraft:custom"]["minecraft:play_one_minute"].get<int>();
        }
        catch(...){
            cout<<"Error while reading statistic";
            exit(1);
        }
        return playtimeticks;
    }else{
        int playtimeticks = 0;
        try{
            playtimeticks = s["stats"]["minecraft:custom"]["minecraft:play_time"].get<int>();
        }
        catch(...){
            cout<<"Error while reading statistic";
            exit(1);
        }
        return playtimeticks;
    }
    return 0;
}

void statlib::wypiszWGodzinach(int playtimeticks, string nc){
    int playtimehours = playtimeticks/72000;
    cout<<"Playtime of player "<<nc<<" is: "<<playtimehours<<"h"<<endl;
}
