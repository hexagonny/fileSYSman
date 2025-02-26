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

struct Config
{
    path sourceDirectory;
    vector<path> initialPaths;
};
//  Function to set the console text color using ANSI escape codes.
void setColor(int textColor)
{
    cout<<"\033["<<textColor<<"m";
}
//  Function to reset the console color
void resetColor() { cout<<"\033[0m"; }
//  This function exists for better output handling.
void sleep(int miliseconds)
{
    this_thread::sleep_for(chrono::milliseconds(miliseconds));
}
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
                setColor(91); cerr<<"Error: ";
                resetColor(); cerr<<"Path does not exists or is noot a directory.\n";
                paths[i].clear();
            }
        }while(paths[i].empty());
    }

    setColor(92); cout<<"Directory Accepted:\n";
    resetColor();
        for(const auto &path : paths){
            cout<<'\t'<<path<<'\n';
        }
    sleep(2000);
    
    ofstream file(fileName);

    if(file.is_open()){
        file<<"sourceDirectory="<<paths[0]<<'\n';
        file<<"initialDirectories="<<paths[1]<<","<<paths[2];
        setColor(92); cout<<"File created: ";
        resetColor(); cout<<fileName<<'\n';
    }
    else{
        setColor(91); cerr<<"Error: ";
        resetColor(); cerr<<"Failed to create file "<<fileName<<'\n';
    }
}
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
        setColor(91); cerr<<"Error: ";
        resetColor(); cerr<<"Failed to open config file: "<<fileName<<'\n';
        return config;
    }
    setColor(92); cout<<"Successfuly opened: ";
    resetColor(); cout<<fileName<<'\n';
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
void moveFile(const path& entry, const path& destination)
{
    path destPath = destination / entry.filename();
    try{
        rename(entry, destPath);
        setColor(93); cout<<"Moved: ";
        resetColor(); cout<<entry.string()<<'\n';
        setColor(93); cout<<"To: ";
        resetColor(); cout<<destPath.string()<<'\n';
    }
    catch(const filesystem_error &e){
        setColor(91); cerr<<"Error: ";
        resetColor(); cerr<<"Failed to move "<<entry.string()<<": "<<e.what()<<'\n';
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
            setColor(95); cerr<<"Warning: ";
            resetColor(); cerr<<"Directory does not exist or is not a directory: "<<initialDir.string()<<'\n';
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
                        setColor(92); cout<<"New folder created: ";
                        resetColor(); cout<<destDir.string()<<'\n';
                    }
                    catch(const filesystem_error &e){
                        setColor(91); cerr<<"Error: ";
                        resetColor(); cout<<"Failed to create folder: "<< e.what()<<'\n';
                        continue;
                    }
                }

                moveFile(entry.path(), destDir);
            }
        }
    }
    catch(const filesystem_error& e){
        setColor(91); cerr<<"Error: ";
        resetColor(); cerr<<e.what()<<'\n';
    }
    cout<<'\n';
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
                            setColor(92); cout<<"New folder created: ";
                            resetColor(); cout<<alphaFolder.string()<<'\n';
                        }
                        catch(const filesystem_error &e){
                            setColor(91); cerr<<"Error: ";
                            resetColor(); cerr<<"Failed to create folder: "<< e.what()<<'\n';
                            continue;
                        }
                    }
                    moveFile(filePath, alphaFolder);
                }
                else{
                    setColor(95); cout<<"Skipped: ";
                    resetColor(); cout<<entry.path().string()<<" (non-alphabetical start)\n";
                }
            }
        }
    }
    catch(const filesystem_error& e){
        setColor(91); cerr<<"Error: ";
        resetColor(); cerr<<e.what()<<'\n';
    }
    cout<<'\n';   
}
void removeFolder(const path& destination)
{
    try{
        remove_all(destination);
        setColor(93); cout<<"Deleted: ";
        resetColor(); cout<<"Successfuly deleted: "<<destination.string()<<'\n';
    }
    catch(const filesystem_error &e){
        setColor(91); cerr<<"Error: ";
        resetColor(); cerr<<"Deleting folder: "<<e.what()<<'\n';
    }
}
void removeExtensionSorting(const path& sourceDir,
                            unordered_map<string, path> &destMap)
{
    try{
        for(const auto &entry : destMap){
            const path &destDir = entry.second;
            if(!exists(destDir) || !is_directory(destDir)){
                setColor(95); cerr<<"Warning: ";
                resetColor(); cerr<<"Directory does not exist or is not a directory: "<<destDir.string()<<'\n';
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
        setColor(91); cerr<<"Error: ";
        resetColor(); cerr<<e.what()<<'\n';
    }
}
void removeAlphabeticalSorting(const path& sourceDir)
{
    try{
        for(const auto& entry : directory_iterator(sourceDir)){
            const path& destDir = entry.path();
            if(is_directory(entry)){
                string destName = destDir.filename().string();
                if(destName.size() == 1 && isupper(destName[0])){
                    for(const auto& file : directory_iterator(entry)){
                        if(file.is_regular_file()){
                            moveFile(file.path(), sourceDir);
                        }
                    }
                }
                removeFolder(destDir);
            }
        }
    }
    catch(const filesystem_error& e){
        setColor(91); cerr<<"Error: ";
        resetColor(); cerr<<e.what()<<'\n';
    }
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
int main()
{
    //  To draw a straight line for better output handling.
    string line(60, '_');

    const string fileName = "fileSYSman_Config.txt";

    cout<<'\n';
    if(!exists(fileName)){
        setColor(91); cerr<<"Error: ";
        resetColor(); cerr<<"Config file does not exists.\n";
        sleep(1000);
        cout<<line; cout<<"\n\n"
            <<"You don't have the config file for fileSYSman!\n"
            <<"Let's quickly configure your settings.\n\n";
        system("pause");
        cout<<line; cout<<"\n\n";
        createTextConfig(fileName);
    }

    Config config = readConfig(fileName);
    
    if (config.sourceDirectory.empty()){
        setColor(91); cerr<<"Error: ";
        resetColor(); cerr<<"Source directory is missing in the config file.\n\n";
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

    cout<<line; cout<<"\n\n";
    displayCurrentDir(config.initialPaths,
                      config.sourceDirectory,
                      destinationMap);

    system("pause");
    cout<<line<<"\n\n";

    int choice;
    cout<<"1. Move files to your documents folder\n"
        <<"2. Sort files in your documents folder\n"
        <<"3. Remove sorted folders in your documents folder\n"
        <<"   Enter any key to exit.\n\n"
        <<"Choose action: ";
    cin>>choice;
    cout<<line<<"\n\n";

    switch(choice){
        char choice;
        case 1:
            moveToSourceDir(config.initialPaths,
                            config.sourceDirectory,
                            destinationMap);
            break;
        case 2:
            int sortChoice;
            do{
                cout<<"1. Sort by extension\n"
                    <<"2. Sort alphabetiacally\n\n"
                    <<"Choose action: ";
                cin>>sortChoice;
                cin.clear(); fflush(stdin);
            }while(sortChoice != 1 && sortChoice != 2);
            cout<<line<<"\n\n";

            switch(sortChoice){
                case 1: 
                    sortByExtension(config.sourceDirectory, destinationMap);
                    break;
                case 2:
                    sortAlphabetically(config.sourceDirectory);
                    break;
            }
            break;
        case 3:
            int removeChoice;
            do{
                cout<<"1. Remove extension folders\n"
                    <<"2. Remove alphabetical fodlers\n\n"
                    <<"Choose action: ";
                cin>>removeChoice;
                cin.clear(); fflush(stdin);
            }while(removeChoice != 1 && removeChoice != 2);
            cout<<line<<"\n\n";

            switch(removeChoice){
                case 1:
                    removeExtensionSorting(config.sourceDirectory, destinationMap);
                case 2:
                    removeAlphabeticalSorting(config.sourceDirectory);
            }
            break;
    }
    system("pause");
    return 0;
}