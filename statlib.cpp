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
            cerr<<"Could not create configuration file";
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
    }
}

json statlib::whitelistToJson(){
    ifstream whitelist;
    string file;
    file = pathwhitelist+"whitelist.json";
    whitelist.open(file);
    if (!whitelist.good()) {
        std::cerr << "Could not open whitelist file" << endl;
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

Nick statlib::stat(string uuid){
    Nick stat;
    stat = {uuid,"",0,0,0,0};
    string file = pathstat+uuid+".json";
    ifstream stats;
    stats.open(file);
    if(!stats.good()){
        cerr<<"Could not open statistics file "<<file<<endl;
        //exit(1);
        return stat;
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
                cerr<<"Error while reading playtime statistic for "<<uuid<<endl;
            }
            try
            {
                stat.deathcount = s["stats"]["minecraft:custom"]["minecraft:deaths"].get<int>();
            }
            catch(...)
            {
                cerr<<"Error while reading deathcount for "<<uuid<<endl;
            }
            try
            {
                stat.gameexit = s["stats"]["minecraft:custom"]["minecraft:leave_game"].get<int>();
            }
            catch(...)
            {
                cerr<<"Error while reading game exit count for "<<uuid<<endl;
            }
            try
            {
                stat.netheriteszt = s["stats"]["minecraft:crafted"]["minecraft:netherite_ingot"].get<int>();
            }
            catch(...)
            {
                cerr<<"Error while reading netherite ingot count for "<<uuid<<endl;
            }
            playtimehours = playtimeticks/72000;
            stat.godziny = playtimehours;
            return stat;
        }else{
            int playtimeticks = 0;
            try{
                playtimeticks = s["stats"]["minecraft:custom"]["minecraft:play_time"].get<int>();
            }
            catch(...){
                cerr<<"Error while reading playtime statistic for "<<uuid<<endl;
            }
            try
            {
                stat.deathcount = s["stats"]["minecraft:custom"]["minecraft:deaths"].get<int>();
            }
            catch(...)
            {
                cerr<<"Error while reading deathcount for "<<uuid<<endl;
            }
            try
            {
                stat.gameexit = s["stats"]["minecraft:custom"]["minecraft:leave_game"].get<int>();
            }
            catch(...)
            {
                cerr<<"Error while reading game exit count for "<<uuid<<endl;
            }
            try
            {
                stat.netheriteszt = s["stats"]["minecraft:crafted"]["minecraft:netherite_ingot"].get<int>();
            }
            catch(...)
            {
                cerr<<"Error while reading netherite ingot count for "<<uuid<<endl;
            }
            playtimehours = playtimeticks/72000;
            stat.godziny = playtimehours;
            return stat;
        }
    }
    return stat;
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
        gracze[i].godziny = stat(gracze[i].uuid).godziny;
        gracze[i].deathcount = stat(gracze[i].uuid).deathcount;
        gracze[i].gameexit = stat(gracze[i].uuid).gameexit;
        gracze[i].netheriteszt = stat(gracze[i].uuid).netheriteszt;
        plik<<gracze[i].name<<" "<<gracze[i].uuid<<" "<<gracze[i].godziny<<" "<<gracze[i].deathcount<<" "<<gracze[i].gameexit<<" "<<gracze[i].netheriteszt<<endl;
    }

    // for (int i = 0; i < gracze.size(); i++)
    // {
    //     cout<<gracze[i].name<<" "<<gracze[i].godziny<<endl;
    // }
}
