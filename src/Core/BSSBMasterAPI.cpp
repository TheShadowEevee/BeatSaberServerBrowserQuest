#include "main.hpp"
#include "Core/BSSBMasterAPI.hpp"

#include "Utils/WebUtils.hpp"

#include "Core/Exceptions.hpp"

#define BASE_URL std::string("https://bssb.app/api/v1")
#define FILE_DOWNLOAD_TIMEOUT 64

namespace ServerBrowser::Core {
    std::string BSSBMasterAPI::exception;

    std::optional<ServerBrowser::Core::ServerBrowserResult> BSSBMasterAPI::Browse(std::string platform, int filterFull, int vanilla) {
        std::string browsePath = BASE_URL + "/browse?platform=" + platform;
        if (filterFull > 0) browsePath += "&filterFull=" + std::to_string(filterFull);
        if (vanilla > 0) browsePath += "&vanilla=" + std::to_string(vanilla);
        std::optional<rapidjson::Document> json;
        json = WebUtils::GetJSON(browsePath);
        if (!json.has_value())
            return std::nullopt;
        try {
            ServerBrowser::Core::ServerBrowserResult page;
            page.Deserialize(json.value().GetObject());
            if (!exception.empty()) exception.clear();
            return page;
        }
        catch (const std::exception& e) {
            getLogger().error("%s", e.what());
            exception = e.what();
            return std::nullopt;
        }
    }

    //std::vector<uint8_t> BSSBMasterAPI::GetCoverImage(const BeatSaver::Beatmap& beatmap) {
    //    std::string data;
    //    WebUtils::Get(beatmap.GetVersions().front().GetCoverURL(), FILE_DOWNLOAD_TIMEOUT, data);
    //    std::vector<uint8_t> bytes(data.begin(), data.end());
    //    return bytes;
    //}

    void BSSBMasterAPI::BrowseAsync(std::function<void(std::optional<ServerBrowser::Core::ServerBrowserResult>)> finished, std::string platform, int filterFull, int vanilla) {
        exception.clear();
        std::string browsePath = BASE_URL + "/browse?platform=" + platform;
        if (filterFull > 0) browsePath += "&filterFull=" + std::to_string(filterFull);
        if (vanilla > 0) browsePath += "&vanilla=" + std::to_string(vanilla);
        WebUtils::GetJSONAsync(browsePath,
            [finished](long httpCode, bool error, rapidjson::Document& document) {
                if (error) {
                    finished(std::nullopt);
                }
                else {
                    try {
                        ServerBrowser::Core::ServerBrowserResult page;
                        page.Deserialize(document.GetObject());
                        finished(page);
                    }
                    catch (const std::exception& e) {
                        getLogger().error("%s", e.what());
                        exception = e.what();
                        finished(std::nullopt);
                        // Convert the document into a string and log/write to file for debug purposes
                        rapidjson::StringBuffer buffer;
                        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                        document.Accept(writer);
                        writefile("/sdcard/ModData/ServerBrowser_Browse.json", buffer.GetString());
                    }
                }
            }
        );
    }

    //void DownloadBeatmapAsync(const BeatSaver::Beatmap& beatmap, std::function<void(bool)> finished, std::function<void(float)> progressUpdate) {
    //    WebUtils::GetAsync(beatmap.GetVersions().front().GetDownloadURL(), FILE_DOWNLOAD_TIMEOUT,
    //        [beatmap, finished](long httpCode, std::string data) {
    //            auto targetFolder = RuntimeSongLoader::API::GetCustomLevelsPath() + FileUtils::FixIlegalName(beatmap.GetId() + " (" + beatmap.GetMetadata().GetSongName() + " - " + beatmap.GetMetadata().GetLevelAuthorName() + ")");
    //            int args = 2;
    //            int statusCode = zip_stream_extract(data.data(), data.length(), targetFolder.c_str(), +[](const char* name, void* arg) -> int {
    //                return 0;
    //                }, &args);
    //            finished(statusCode);
    //        }, progressUpdate
    //    );
    //}

    void BSSBMasterAPI::GetCoverImageAsync(const ServerBrowser::Core::HostedGameData& lobby, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate) {
        auto coverURL = lobby.get_CoverUrl();
        if (coverURL.has_value()) {
            std::string URL = coverURL.value();
            URL.erase(remove(URL.begin(), URL.end(), '\\'), URL.end());
            WebUtils::GetAsync(URL, FILE_DOWNLOAD_TIMEOUT,
                [lobby, finished](long httpCode, std::string data) {
                    std::vector<uint8_t> bytes(data.begin(), data.end());
                    finished(bytes);
                }, progressUpdate
            );
        }
    }
}