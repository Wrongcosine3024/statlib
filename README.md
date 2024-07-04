Statlib is a simple application for reading and summarizing minecraft statistics for spigot servers.

Minecraft statistics for each player are stored under <server_dir>/world/stats/<player_uuid>.json
Statlib get player uuid from whitelist file **which must be supplied**, stores it, and if file exists - opens uuid.json under given directory.
Simple as that!

Program was tested only on minecraft versions 1.16 and 1.19, so it can detect wheter the play time statistic is called play_one_minute or play_time (see _Technical changes_ under [this link](https://www.minecraft.net/en-us/article/minecraft-snapshot-21w16a))

It's my **first serious** c++ project which I decided to publish on github. I'm aware that its probably bad written so any suggestions are welcome.
