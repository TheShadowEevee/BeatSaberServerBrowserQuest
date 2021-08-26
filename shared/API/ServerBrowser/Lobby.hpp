#pragma once
#include "../TypeMacros.hpp"
#include "SemVer.hpp"

DECLARE_JSON_CLASS(ServerBrowser::API, Lobby, 
    GETTER_VALUE_OPTIONAL(std::string, BeatsaverId);
    GETTER_VALUE_OPTIONAL(std::string, CoverUrl);
    GETTER_VALUE_OPTIONAL(std::string, LevelName);
    GETTER_VALUE(int, Id);
    GETTER_VALUE(std::string, ServerCode);
    GETTER_VALUE(std::string, GameName);
    GETTER_VALUE(std::string, OwnerName);
    GETTER_VALUE(int, PlayerCount);
    GETTER_VALUE(int, PlayerLimit);
    GETTER_VALUE(bool, IsModded);
    GETTER_VALUE(std::string, FirstSeen);
    GETTER_VALUE(std::string, LastUpdate);
    GETTER_VALUE(int, LobbyState);
    GETTER_VALUE_OPTIONAL(std::string, LevelId);
    GETTER_VALUE_OPTIONAL(std::string, SongName);
    GETTER_VALUE_OPTIONAL(std::string, SongAuthor);
    GETTER_VALUE_OPTIONAL(int, Difficulty);
    GETTER_VALUE(std::string, Platform);
    GETTER_VALUE(std::string, MasterServerHost);
    GETTER_VALUE(int, MasterServerPort);
    GETTER_VALUE_OPTIONAL(std::string, EndedAt);
    GETTER_VALUE(std::string, MpExVersion);
    GETTER_VALUE(ServerBrowser::API::SemVer, ModVersion);
    GETTER_VALUE(ServerBrowser::API::SemVer, GameVersion);
    GETTER_VALUE_OPTIONAL(std::string, ServerType);
    GETTER_VALUE_OPTIONAL(std::string, HostSecret);
    GETTER_VALUE_OPTIONAL(std::string, Endpoint);
)
