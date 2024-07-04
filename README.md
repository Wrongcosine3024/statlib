## Table of contents
- [Overview](#overview)
  - [Very important](#very-important-)
- [Build](#build)
- [Branches](#branches)
- [Error handling](#error-handling)
- [License and third-party libraries](#license-and-third-party-libraries)
  - [JSON for modern C++](#JSON-for-modern-c)
## Overview
Statlib is a simple application for reading and summarizing minecraft statistics for spigot servers. Statlib has been written object oriented so it can be used in other projects as _statlib_ class.

Minecraft statistics for each player are stored under <server_dir>/world/stats/<player_uuid>.json
Statlib get player uuid from whitelist file **which must be supplied**, stores it, and if file exists - opens uuid.json under given directory.
Simple as that!

Program was tested only on minecraft versions 1.16 and 1.19 (spigot servers), so it can detect wheter the play time statistic is called play_one_minute or play_time (see _Technical changes_ under [this link](https://www.minecraft.net/en-us/article/minecraft-snapshot-21w16a))

### Very important ;)  
It's my **first serious** C++ project which I decided to publish on github. I'm aware that its probably bad written so any suggestions are welcome.
## Build
In order to build statlib I recommend using g++ compiler. You have to use -I flag followed by the directory of this repository (i.e. -I/home/user/Documents/statlib).

Example build command:  
```g++ -I./ main.cpp statlib.cpp -o statlib```
## Branches
There are four branches in this repository:
- main (current branch)
- files
- files-wo-cli
- ost

Initally statlib was console application designed to read playtime only (main branch) but I came up with an idea to use it as autonomous script:  
- under the "files" branch location of whitelist and statistics folder are still given via cli but output is saved to a file
- under the "files-wo-cli" branch program will check if there is a file called "localization.conf", if not it will create it. From now on, locations of whitelist and statistics directory will be given through that file
- the "ost" branch is the same as "files-wo-cli" but instead of reading only playtime, program additionally reads player deathcount, game exit count, and number of netherite ingots created by player
## Error handling
Statlib has implemented basic error handling, if it can't read statistic (because it changed name or sth) it will just print out error via console **and will continue execution**, for example: if there is a player on whitelist who never joined a server (so the server hasn't created statistics file for that specific player) program will not crash (because there is no file to read), it will just print error and try to read statistics for next player.
## License and third-party libraries
This project is released under [MIT License](https://github.com/Wrongcosine3024/statlib/blob/main/LICENSE).
### JSON for modern C++
It was possible to make this project by use of the _JSON for modern C++_ library written by **Niels Lohmann** (see [NOTICE](https://github.com/Wrongcosine3024/statlib/blob/main/NOTICE))
