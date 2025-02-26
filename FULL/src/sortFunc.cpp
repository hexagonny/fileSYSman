#include "filesysman.h"

void moveFile(const path& entry, const path& destination)
{
    path destPath = destination / entry.filename();
    try{
        rename(entry, destPath);
        logger.logAction("Moved: " + entry.string());
        logger.logMoved("To: " + destPath.string());
    }
    catch(const filesystem_error &e){
        logger.logError("Error: Failed to move " + entry.string() + ": "
                        + string(e.what()));
    }
}
/*
  It moves common document files from different directories to the prefered documents folder.
  The algorithm is based on file extentions and multiple directories(eg. downloads folder).
*/
void moveToSourceDir(const vector<path>& initialPaths,
                     const path& sourceDir,
                     const unordered_map<string, path>& destMap)
{
    for (const auto& initialDir : initialPaths){
        if(!exists(initialDir) || !is_directory(initialDir)){
            logger.logWarning("Warning: Directory does not exist or is not a directory: "
                            + initialDir.string());
            continue;
        }

        for(const auto& entry : directory_iterator(initialDir)){
            if(!entry.is_regular_file()) continue;

            const auto& filePath = entry.path();
            string ext = entry.path().extension().string();

            if(destMap.find(ext) != destMap.end()){
                moveFile(filePath, sourceDir);
            }
        }
    }
    cout<<'\n';
}
/*
  It organizes cluttered documents into their respective folders.
  The algorithm is based on file extentions and their matching folder names.
*/
void sortByExtension(const path& sourceDir,
                     const unordered_map<string, path>& destMap)
{
    try{
        for(const auto& entry : directory_iterator(sourceDir)){
            if(!entry.is_regular_file()) continue;

            const auto& ext = entry.path().extension().string();

            if(destMap.find(ext) != destMap.end()){
                path destDir = destMap.at(ext);

                if(!exists(destDir)){
                    try{
                        create_directory(destDir);
                        logger.logCreated("New folder created: " + destDir.string());
                    }
                    catch(const filesystem_error &e){
                        logger.logError("Error: Failed to create folder: " + string(e.what()));
                        continue;
                    }
                }

                moveFile(entry.path(), destDir);
            }
        }
    }
    catch(const filesystem_error& e){
        logger.logError("Error: " + string(e.what()));
    }
}
void sortAlphabetically(const path& sourceDir)
{
    try{
        for(const auto& entry : directory_iterator(sourceDir)){
            if(!entry.is_regular_file()) continue;

            const auto& filePath = entry.path();
            string name = entry.path().filename().string();

            if(name.empty() || name[0] == '.'){
                continue;
            }

            if(is_directory(entry)){
                sortAlphabetically(entry.path());
            }
            else{
                char firstChar = toupper(name[0]);
                if(isalpha(firstChar)){
                    string alphaFolderName(1, firstChar);
                    path alphaFolder = sourceDir / alphaFolderName;

                    if(!exists(alphaFolder)){
                        try{
                            create_directory(alphaFolder);
                            logger.logCreated("New folder created: " + alphaFolder.string());
                        }
                        catch(const filesystem_error &e){
                            logger.logError("Error: Failed to create folder: " + string(e.what()));
                            continue;
                        }
                    }
                    moveFile(filePath, alphaFolder);
                }
                else{
                    logger.logWarning("Skipped: " + entry.path().string()
                                      + " (non-alphabetical start)");
                }
            }
        }
    }
    catch(const filesystem_error& e){
        logger.logError("Error: " + string(e.what()));
    }  
}