#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <unordered_map>

using namespace std;
using namespace std::filesystem;

//  Please read the code documentation first for the requirements of compiling this code.

void sortDocumentFiles(path& sourceDir, unordered_map<string, path>& destMap)
{
    try{
        for(const auto& entry : directory_iterator(sourceDir)){
            if(entry.is_regular_file()){
                const auto& filePath = entry.path();
                const auto& ext = filePath.extension().string();

                if(destMap.find(ext) != destMap.end()){
                    path destDir = destMap.at(ext);

                    if(!exists(destDir)){
                        try{
                            create_directory(destDir);
                            cout<<"New folder created: "<<destDir.string()<<'\n';
                        }
                        catch(const filesystem_error &e){
                            cerr<<"Error: Failed to create folder: "<< e.what()<<'\n';
                            continue;
                        }
                    }
                    path destPath = destDir / filePath.filename();
                    try{
                        rename(filePath, destPath);
                        cout<<"Moved: "<<filePath.string()<<'\n'
                            <<"To: "<<destPath.string()<<'\n';
                    }
                    catch(const filesystem_error &e){
                        cerr<<"Error: Failed to move "<<filePath.string()<<": "<<e.what()<<'\n';
                    }
                }
            }
        }
    }
    catch(const filesystem_error& e){
        cerr<<"Error: "<<e.what()<<'\n';
    }
}
int main()
{
    path sourceDir;
    do{
        cout<<"Enter your documents path: ";
        cin>>sourceDir;

        if(!exists(sourceDir) || !is_directory(sourceDir)){
            cerr<<"Error: Path does not exists or is not a directory.\n";
            sourceDir.clear();
        }
    }while(sourceDir.empty());

    unordered_map<string, path> destMap = {
        {".pdf",  sourceDir / "PDF"},
        {".docx", sourceDir / "DOCX"},
        {".doc",  sourceDir / "DOCX"},
        {".pptx", sourceDir / "PPTX"},
        {".xlsx", sourceDir / "XLSX"},
    };

    cout<<"Directory Accepted: "<<sourceDir<<"\n";

    sortDocumentFiles(sourceDir, destMap);
    cout<<"Moving files is completed.\n\n";
    system("pause");
    return 0;
}