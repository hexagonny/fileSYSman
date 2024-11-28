#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <cstdlib>

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
    path sourceDir;
    do{
        cout<<"Enter your documents path: ";
        cin>>sourceDir;

        if(!exists(sourceDir) || !is_directory(sourceDir)){
            cerr<<"Error: Path does not exist or is not a directory.\n";
            sourceDir.clear();
        }
    }while(sourceDir.empty());
    cout<<"Directory Accepted: "<<sourceDir<<'\n';
    
    ofstream file(fileName);

    if(file.is_open()){
        file<<"sourceDirectory="<<sourceDir<<'\n';
        cout<<"File created: "<<fileName<<'\n';
    }
    else{
        cerr<<"Error: Failed to create file "<<fileName<<'\n';
    }
}
//  This function reads every single string and filters out the unnecessary characters.
path readConfig(const string& fileName)
{
    ifstream file(fileName);
    string line;
    path sourceDir;

    if (file.is_open()){
        cout<<"Successfuly opened: "<<fileName<<'\n';

        while(getline(file, line)){
            if(line.find("sourceDirectory=") == 0){
                string pathValue = line.substr(16);

                if(!pathValue.empty() && pathValue.front() == '"' && pathValue.back() == '"'){
                    pathValue = pathValue.substr(1, pathValue.size() - 2);
                }
                sourceDir = pathValue;
                break;
            }
        }
        
        file.close();
    }
    else{
        cerr<<"Error: Failed to open config file: "<<fileName<<'\n';
    }

    return sourceDir;
}
/*
  One of the key functions. It organizes cluttered documents into their respective folders.
  The algorithm is based on file extentions and their matching folder names.
*/
void moveDocumentFiles(const path& sourceDir, const unordered_map<string, path>& destMap)
{
    try{
        for (const auto& entry : directory_iterator(sourceDir)){
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
    } catch (const filesystem_error& e){
        cerr<<"Error: "<<e.what()<<'\n';
    }
    cout<<'\n';
}
//  This function displays the current destinations for checking and debugging.
void displayCurrentDir(const path& sourceDir, const unordered_map<string, path>& destMap)
{
    cout<<"Current source directory: "<<sourceDir.string()<<'\n';
    cout<<"Current destination directories:\n";
    for(const auto& [extention, destDir] : destMap){
        cout<<'\t'<<destDir.string()<<'\n';
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

    path sourceDirectory = readConfig(fileName);
    unordered_map<string, path> destinationMap = {
        {".pdf",  sourceDirectory / "PDF"},
        {".docx", sourceDirectory / "DOCX"},
        {".doc",  sourceDirectory / "DOCX"},
        {".pptx", sourceDirectory / "PPTX"},
        {".xlsx", sourceDirectory / "XLSX"},
    };

    cout<<line; cout<<endl;
    displayCurrentDir(sourceDirectory, destinationMap);

    system("pause");
    cout<<line; cout<<endl;
    
    moveDocumentFiles(sourceDirectory, destinationMap);

    system("pause");

    return 0;
}