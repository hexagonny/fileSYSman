#include "filesysman.h"

int main()
{
    clearAll();

    string line(60, '_');

    const string fileName = "fileSYSman_Config.txt";

    cout<<"Checking if config file exists...\n\n";
    sleep(1000);
    if(!exists(fileName)){
        logger.logError("Error: Config file does not exist!");
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
        logger.logError("Error: Source directory is missing in the config file.");
        cout<<"\n";
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

    sleep(1000); clearAll();
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
        <<setw(17)<<left<<"Choose action: ";
    cin>>choice;
    cin.clear(); fflush(stdin);
    clearLine(17);

    switch(choice){
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
                    <<setw(17)<<left<<"Choose action: ";
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
                    <<"2. Remove alphabetical folders\n\n"
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
    logger.displaySummary();
    system("pause");
    return 0;
}