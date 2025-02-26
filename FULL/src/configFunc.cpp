#include "filesysman.h"

//  This function creates a new configuration file if the user does not have it.
void createTextConfig(const string &fileName)
{
    vector<string> dirLabels = {"prefered documents", "downloads", "desktop"};
    vector<path> paths(3);

    cout<<"If you already have your existing documents folder is decently sorted.\n"
        <<"I suggest you to create a new secondary documents folder.\n\n";
    sleep(6000);

    for(size_t i = 0; i < dirLabels.size(); ++i ){
        do{
            cout<<"Enter your "<<dirLabels[i]<<" path: ";
            cin>>paths[i];

            if(!exists(paths[i]) || !is_directory(paths[i])){
                logger.logError("Error: Path does not exists or is not a directory!");
                paths[i].clear();
            }
        }while(paths[i].empty());
    }

    logger.logSuccess("Directory Accepted:");
    for(const auto &path : paths){
        cout<<'\t'<<path<<'\n';
    }
    sleep(2000);
    
    ofstream file(fileName);

    if(file.is_open()){
        file<<"sourceDirectory="<<paths[0]<<'\n';
        file<<"initialDirectories="<<paths[1]<<","<<paths[2];
        logger.logSuccess("File created: " + fileName);
    }
    else{
        logger.logError("Error: Failed to create file " + fileName);
    }
}
//  This function removes'\t' in every single line of string.
string trim(const string &str)
{
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");

    if(first == string::npos || last == string::npos){
        return "";
    }

    return str.substr(first, (last - first + 1));
}
//  This function reads every single string and filters out the unnecessary characters.
Config readConfig(const string& fileName)
{
    ifstream file(fileName); Config config;
    if(!file.is_open()){
        logger.logError("Error: Failed to open config file: " + fileName);
        return config;
    }
    logger.logSuccess("Successfully opened: " + fileName);
    sleep(2000);

    string line;
    while(getline(file, line)){
        if(line.find("sourceDirectory=") == 0){
            string sourceDir = line.substr(string("sourceDirectory=").size());

            sourceDir = trim(sourceDir);

            if(!sourceDir.empty() && sourceDir.front() == '"' && sourceDir.back() == '"'){
                sourceDir = sourceDir.substr(1, sourceDir.size() - 2);
            }

            config.sourceDirectory = sourceDir;
        }
        else if(line.find("initialDirectories=") == 0){
            string initialDirs = line.substr(string("initialDirectories=").size());
            
            initialDirs = trim(initialDirs);

            stringstream ss(initialDirs);
            string path;
            while(getline(ss, path, ',')){
                path = trim(path);

                if(!path.empty() && path.front() == '"' && path.back() == '"'){
                    path = path.substr(1, path.size() - 2);
                }
                config.initialPaths.push_back(path);
            }
        }
    }
    
    file.close();
    return config;
}
//  This function displays the current destinations for checking and debugging.
void displayCurrentDir(const vector<path>& initialPaths,
                       const path& sourceDir,
                       const unordered_map<string, path>& destMap)
{
    cout<<"Current Configuration:\n\n";
    if(!initialPaths.empty()){
        cout<<"Initial directories:\n";
        for(const auto &initialDir : initialPaths){
            cout<<'\t'<<initialDir.string()<<'\n';
        }
    }
    else{
        cout<<"Initial Directories: None specified.\n";
    }
    cout<<"Source directory: "<<sourceDir.string()<<'\n';
    
    cout<<"Destination directories(for extensions):\n";
    for(const auto& [extension, destDir] : destMap){
        cout<<'\t'<<destDir.string()<<" -> "
            <<extension<<'\n';
    }
    cout<<'\n';
}