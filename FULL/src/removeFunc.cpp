#include "filesysman.h"

void removeFolder(const path& destination)
{
    try{
        remove_all(destination);
        logger.logRemoved("Successfuly deleted: " + destination.string());
    }
    catch(const filesystem_error &e){
        logger.logError("Error: Deleting folder: " + string(e.what()));
    }
}
void removeExtensionSorting(const path& sourceDir,
                            const unordered_map<string, path>& destMap)
{
    try{
        for(const auto &entry : destMap){
            const path &destDir = entry.second;
            if(!exists(destDir) || !is_directory(destDir)){
                logger.logWarning("Warning: Directory does not exist or is not a directory: "
                                  + destDir.string());
                continue;
            }
            for(const auto& file : directory_iterator(destDir)){
                if(file.is_regular_file()){
                    const auto& filePath = file.path();
                    moveFile(filePath, sourceDir);
                }
            }
            removeFolder(destDir);
        }
    }
    catch(const filesystem_error& e){
        logger.logError("Error: " + string(e.what()));
    }
}
void removeAlphabeticalSorting(const path& sourceDir)
{
    try{
        for(const auto& entry : directory_iterator(sourceDir)){
            const path& destDir = entry.path();
            if(!is_directory(entry)){
                logger.logWarning("Warning: Directory does not exist or is not a directory: "
                                  + destDir.string());
                continue;
            }
            string destName = destDir.filename().string();
            if(destName.size() == 1 && isupper(destName[0])){
                for(const auto& file : directory_iterator(entry)){
                    if(file.is_regular_file()){
                        const auto& filePath = file.path();
                        moveFile(filePath, sourceDir);
                    }
                }
            }
            removeFolder(destDir);
        }
    }
    catch(const filesystem_error& e){
        logger.logError("Error: " + string(e.what()));
    }
}