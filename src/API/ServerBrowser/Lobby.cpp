#include "API/ServerBrowser/Lobby.hpp"

DESERIALIZE_METHOD(ServerBrowser::API, Lobby,
    DESERIALIZE_VALUE_OPTIONAL(BeatsaverId, beatsaverId, String)
    DESERIALIZE_VALUE_OPTIONAL(CoverUrl, coverUrl, String)
    DESERIALIZE_VALUE_OPTIONAL(LevelName, levelName, String)
    DESERIALIZE_VALUE(Id, id, Int)
    DESERIALIZE_VALUE(ServerCode, serverCode, String)
    DESERIALIZE_VALUE(GameName, gameName, String)
    DESERIALIZE_VALUE(OwnerName, ownerName, String)
    DESERIALIZE_VALUE(PlayerCount, playerCount, Int)
    DESERIALIZE_VALUE(PlayerLimit, playerLimit, Int)
    DESERIALIZE_VALUE(IsModded, isModded, Bool)
    DESERIALIZE_VALUE(FirstSeen, firstSeen, String)
    DESERIALIZE_VALUE(LastUpdate, lastUpdate, String)
    DESERIALIZE_VALUE(LobbyState, lobbyState, Int)
    DESERIALIZE_VALUE_OPTIONAL(LevelId, levelId, String)
    DESERIALIZE_VALUE_OPTIONAL(SongName, songName, String)
    DESERIALIZE_VALUE_OPTIONAL(SongAuthor, songAuthor, String)
    DESERIALIZE_VALUE_OPTIONAL(Difficulty, difficulty, Int)
    DESERIALIZE_VALUE(Platform, platform, String)
    DESERIALIZE_VALUE(MasterServerHost, masterServerHost, String)
    DESERIALIZE_VALUE(MasterServerPort, masterServerPort, Int)
    DESERIALIZE_VALUE_OPTIONAL(EndedAt, endedAt, String)
    DESERIALIZE_VALUE(MpExVersion, mpExVersion, String)
    DESERIALIZE_CLASS(ModVersion, modVersion)
    DESERIALIZE_CLASS(GameVersion, gameVersion)
    DESERIALIZE_VALUE_OPTIONAL(ServerType, serverType, String)
    DESERIALIZE_VALUE_OPTIONAL(HostSecret, hostSecret, String)
    DESERIALIZE_VALUE_OPTIONAL(Endpoint, endpoint, String)
)