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
    ifstream plik;
    plik.open("localization.conf");
    if(!plik.good()){
        ofstream plikcr;
        plikcr.open("localization.conf");
        if (!plikcr.good())
        {
            cerr<<"Nie mozna utworzyc pliku konfiguracyjnego";
            exit(1);
        }
        plikcr<<"whitelist_full_dir "<<"/path/to/whitelist/"<<endl;
        plikcr<<"stats_full_dir "<<"/path/to/world/stats/";
        plikcr.close();
        exit(1);
    }else{
        struct config
        {
            string property;
            string value;
        };
        vector<config> conf;
        string temp_var;
        while (plik>>temp_var)
        {
            config temp_config;
            temp_config.property = temp_var;
            plik>>temp_var;
            temp_config.value = temp_var;
            conf.push_back(temp_config);   
        }
        
        pathwhitelist = conf[0].value;
        pathstat = conf[1].value;
        string w;
        cout<<pathwhitelist<<" "<<pathstat<<endl;
    }
}

json statlib::whitelistToJson(){
    ifstream whitelist;
    string file;
    file = pathwhitelist+"whitelist.json";
    whitelist.open(file);
    if (!whitelist.good()) {
        std::cerr << "Nie mozna otworzyc pliku whitelisty." << endl;
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

// string statlib::podajNick(){
//     cout<<"Nick gracza: ";
//     string nc;
//     cin>>nc;
//     return nc;
// }

// string statlib::nickToUuid(vector<Nick> gracze, string nc){
//     string uuid = "";
//     for(int i = 0; i<gracze.size(); i++){
//         if(gracze[i].name == nc){
//             uuid = gracze[i].uuid;
//             break;
//         }
//     }
//     if (uuid == "")
//     {
//         cerr<<"Podana nazwa nie znajduje sie w pliku whitelist.json"<<endl;
//         exit(1);
//     }else{
//         cout<<uuid<<endl;
//         return uuid;
//     }
// }

int statlib::czasGodz(string uuid){
    string file = pathstat+uuid+".json";
    ifstream stats;
    stats.open(file);
    if(!stats.good()){
        cerr<<"Nie mozna otworzyc pliku statystyk! "<<file<<endl;
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
                cerr<<"Blad przy odczytaniu statystki";
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
                cerr<<"Blad przy odczytaniu statystki";
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
