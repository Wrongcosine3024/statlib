Statlib is a simple application for reading and summarizing minecraft statistics for servers.

Minecraft statistics for each player are stored under <server_dir>/world/stats/<player_uuid>.json
Statlib get player uuid from whitelist file, stores it, and if file exists - opens uuid.json under given directory.
