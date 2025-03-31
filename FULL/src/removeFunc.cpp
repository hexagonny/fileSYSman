#include "filesysman.h"
#include "hutils.h"

namespace fs = std::filesystem;

void removeFolder(const fs::path& destination)
{
    try{
        remove_all(destination);
        hUtils::log.Removed("Successfuly deleted: " + destination.string());
    }
    catch(const fs::filesystem_error &e){
        hUtils::log.Error("Error: Deleting folder: " + std::string(e.what()));
    }
}
void removeExtensionSorting(const fs::path& sourceDir,
                            const std::unordered_map<std::string, fs::path>& destMap)
{
    try{
        for(const auto &entry : destMap){
            const fs::path &destDir = entry.second;
            if(!exists(destDir) || !is_directory(destDir)){
                hUtils::log.Warning("Warning: Directory does not exist or is not a directory: "
                                  + destDir.string());
                continue;
            }
            for(const auto& file : fs::directory_iterator(destDir)){
                if(file.is_regular_file()){
                    const auto& filePath = file.path();
                    moveFile(filePath, sourceDir);
                }
            }
            removeFolder(destDir);
        }
    }
    catch(const fs::filesystem_error& e){
        hUtils::log.Error("Error: " + std::string(e.what()));
    }
}
void removeAlphabeticalSorting(const fs::path& sourceDir)
{
    try{
        for(const auto& entry : fs::directory_iterator(sourceDir)){
            const fs::path& destDir = entry.path();
            if(!is_directory(entry)){
                hUtils::log.Warning("Warning: Directory does not exist or is not a directory: "
                                  + destDir.string());
                continue;
            }
            std::string destName = destDir.filename().string();
            if(destName.size() == 1 && isupper(destName[0])){
                for(const auto& file : fs::directory_iterator(entry)){
                    if(file.is_regular_file()){
                        const auto& filePath = file.path();
                        moveFile(filePath, sourceDir);
                    }
                }
            }
            removeFolder(destDir);
        }
    }
    catch(const fs::filesystem_error& e){
        hUtils::log.Error("Error: " + std::string(e.what()));
    }
}