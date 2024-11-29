#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <cstdlib>
#include <vector>

#include <chrono>
#include <thread>

using namespace std;
using namespace std::filesystem;

#ifdef _WIN32
    // Windows-specific code
    void setConsoleWindowSize(int width, int height) {
        std::string command = "mode con: cols=" + std::to_string(width) + " lines=" + std::to_string(height);
        system(command.c_str());
    }
#elif __unix__
    // Linux/MacOS-specific code
    void setConsoleWindowSize(int width, int height) {
        std::string command = "resize -s " + std::to_string(height) + " " + std::to_string(width);
        system(command.c_str());
    }
#endif

struct Config
{
    path sourceDirectory;
    vector<path> initialPaths;
};
//  This function exists for better output handling.
void sleep(int miliseconds){
    this_thread::sleep_for(chrono::milliseconds(miliseconds));
}
//  This function checks whether the user has a configuration file or not.
bool fileExists(const string &fileName)
{
    return exists(fileName);
}
//  This function creates a new configuration file if the user does not have it.
void createTextConfig(const string &fileName)
{
    vector<string> dirLabels = {"documents", "downloads", "desktop"};
    vector<path> paths(3);

    for(size_t i = 0; i < dirLabels.size(); ++i ){
        do{
            cout<<"Enter your "<<dirLabels[i]<<" path: ";
            cin >> paths[i];

            if(!exists(paths[i]) || !is_directory(paths[i])){
                cerr<<"Error: Path does not exists or is noot a directory.\n";
                paths[i].clear();
            }
        }while(paths[i].empty());
    }

    cout<<"Directory Accepted:\n";
    for(const auto &path : paths){
        cout<<'\t'<<path<<'\n';
    }
    
    ofstream file(fileName);

    if(file.is_open()){
        file<<"sourceDirectory="<<paths[0]<<'\n';
        file<<"initialDirectories="<<paths[1]<<","<<paths[2];
        cout<<"File created: "<<fileName<<'\n';
    }
    else{
        cerr<<"Error: Failed to create file "<<fileName<<'\n';
    }
}
string trim(const string &str){
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
    ifstream file(fileName);
    Config config;
    if(!file.is_open()){
        cerr<<"Error: Failed to open config file: "<<fileName<<'\n';
        return config;
    }
    cout<<"Successfuly opened: "<<fileName<<'\n';

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
            string directories = line.substr(string("initialDirectories=").size());
            
            directories = trim(directories);

            stringstream ss(directories);
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
/*
  One of the key functions. It organizes cluttered documents into their respective folders.
  The algorithm is based on file extentions and their matching folder names.
*/
void sortDocumentFiles(const path& sourceDir, const unordered_map<string, path>& destMap)
{
    try{
        for(const auto& entry : directory_iterator(sourceDir)){
            if(entry.is_regular_file()){
                const auto& filePath = entry.path();
                const auto& ext = filePath.extension().string();

                if(destMap.find(ext) != destMap.end()){
                    path destDir = destMap.at(ext);

                    if(!exists(destDir)){
                        create_directories(destDir);
                        cout<<"New folder created: "<<destDir.string()<<'\n';
                    }

                    path destPath = destDir / filePath.filename();

                    rename(filePath, destPath);

                    cout<<"Moved: "<<filePath.string()<<'\n'
                        <<"To: "<<destPath.string()<<'\n';
                }
            }
        }
    }
    catch(const filesystem_error& e){
        cerr<<"Error: "<<e.what()<<'\n';
    }
    cout<<'\n';
}
/*
  One of the key functions. It moves common document files from different directories to the prefered documents folder.
  The algorithm is based on file extentions and multiple directories(eg. downloads folder).
*/
void moveDocumentFiles(const vector<path>& initialPaths,
                       const path& sourceDir,
                       const unordered_map<string, path>& destMap)
{
    for (const auto& initialDir : initialPaths){
        if(!exists(initialDir) || !is_directory(initialDir)){
            cerr<<"Warning: Initial directory does not exist or is not a directory: "<<initialDir.string()<<'\n';
            continue;
        }

        for(const auto& entry : directory_iterator(initialDir)){
            if(entry.is_regular_file()){
                string ext = entry.path().extension().string();

                if(destMap.find(ext) != destMap.end()){
                    try{
                        path destination = sourceDir / entry.path().filename();
                        
                        rename(entry.path(), destination);
                        cout<<"Moved: "<<entry.path().string()<<'\n'
                            <<"To: "<<destination.string()<<'\n';
                    }
                    catch(const filesystem_error &e){
                        cerr<<"Error moving file: "<<entry.path().string()<<": "<<e.what()<<'\n';
                    }
                }
            }
        }
    }
    cout<<'\n';
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
    cout<<"Destination directories:\n";
    for(const auto& [extention, destDir] : destMap){
        cout<<'\t'<<destDir.string()<<" -> "
            <<extention<<'\n';
    }
    cout<<'\n';
}
int main()
{
    //  To draw a straight line for better output handling.
    int length = 60;
    string line(length, '-');

    //  To constantly open in a specific window size for better output handling.
    setConsoleWindowSize(length, 30);

    const string fileName = "fileSYSman_Config.txt";

    if(!fileExists(fileName)){
        cerr<<"Error: Config file does not exists.\n";
        sleep(2000);
        cout<<line; cout<<endl
            <<"You don't have the config file for fileSYSman!\n"
            <<"Let's quickly configure your settings.\n\n";
        system("pause");
        cout<<line; cout<<endl;
        createTextConfig(fileName);
    }

    Config config = readConfig(fileName);

    if (config.sourceDirectory.empty()){
        cerr<<"Error: Source directory is missing in the config file.\n\n";
        system("pause");
        return 1;
    }

    unordered_map<string, path> destinationMap = {
        {".pdf",  config.sourceDirectory / "PDF"},
        {".docx", config.sourceDirectory / "DOCX"},
        {".doc",  config.sourceDirectory / "DOCX"},
        {".pptx", config.sourceDirectory / "PPTX"},
        {".xlsx", config.sourceDirectory / "XLSX"},
    };

    cout<<line; cout<<endl;
    displayCurrentDir(config.initialPaths,
                      config.sourceDirectory,
                      destinationMap);

    system("pause");
    cout<<line; cout<<endl; 

    int choice;
    do{
        cout<<"1. Move files to your documents folder\n"
            <<"2. Sort files in your documents folder\n"
            <<"   Enter any key to exit.\n\n"
            <<"Choose action: ";
        cin>>choice;

        switch(choice){
            case 1: moveDocumentFiles(config.initialPaths,
                                      config.sourceDirectory,
                                      destinationMap);
                    break;
            case 2: sortDocumentFiles(config.sourceDirectory, destinationMap);
                    break;
        }
        system("pause");
        cout<<line; cout<<endl; 
    }while(choice == 1 || choice == 2);

    return 0;
}