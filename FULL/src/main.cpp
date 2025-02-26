#include "filesysman.h"
#include "hutils.h"

#include <iostream>
#include <iomanip>

namespace fs = std::filesystem;

int main()
{
    hUtils::setConsoleWindowSize(); hUtils::Text::clearAll();
    hUtils::Text::toLine();

    const std::string fileName = "fileSYSman_Config.txt";

    std::cout << "Checking if config file exists...\n\n";
    hUtils::sleep(1000);
    if(!fs::exists(fileName)){
        logger.logError("Error: Config file does not exist!");
        hUtils::sleep(1000);
        hUtils::Text::toLine();
        std::cout << "\nYou don't have the config file for fileSYSman!\n"
                  << "Let's quickly configure your settings.\n\n";
        hUtils::pause();
        hUtils::Text::toLine(); std::cout << '\n';
        createTextConfig(fileName);
    }

    Config config = readConfig(fileName);
    
    if (config.sourceDirectory.empty()){
        logger.logError("Error: Source directory is missing in the config file.");
        std::cout << "\n";
        hUtils::pause();
        return 1;
    }
    std::unordered_map<std::string, fs::path> destinationMap = {
        {".pdf",  config.sourceDirectory / "PDF"},
        {".docx", config.sourceDirectory / "DOCX"},
        {".doc",  config.sourceDirectory / "DOCX"},
        {".pptx", config.sourceDirectory / "PPTX"},
        {".xlsx", config.sourceDirectory / "XLSX"},
    };

    hUtils::sleep(1000); hUtils::Text::clearAll();
    displayCurrentDir(config.initialPaths,
                      config.sourceDirectory,
                      destinationMap);

    hUtils::pause();
    hUtils::Text::toLine(); std::cout << '\n';

    int choice;
    std::cout << "1. Move files to your documents folder\n"
              << "2. Sort files in your documents folder\n"
              << "3. Remove sorted folders in your documents folder\n"
              << "   Enter any key to exit.\n\n"
              << std::setw(17) << std::left << "Choose action: ";
    std::cin >> choice;
    std::cin.clear(); fflush(stdin);
    hUtils::Text::clearLine(17);

    switch(choice){
        case 1:
            moveToSourceDir(config.initialPaths,
                            config.sourceDirectory,
                            destinationMap);
            break;
        case 2:
            int sortChoice;
            do{
                std::cout << "1. Sort by extension\n"
                          << "2. Sort alphabetiacally\n\n"
                          << std::setw(17) << std::left << "Choose action: ";
                std::cin >> sortChoice;
                std::cin.clear(); fflush(stdin);
            }while(sortChoice != 1 && sortChoice != 2);
            hUtils::Text::toLine(); std::cout <<'\n';

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
                std::cout << "1. Remove extension folders\n"
                          << "2. Remove alphabetical folders\n\n"
                          << "Choose action: ";
                std::cin >> removeChoice;
                std::cin.clear(); fflush(stdin);
            }while(removeChoice != 1 && removeChoice != 2);
            hUtils::Text::toLine(); std::cout <<'\n';

            switch(removeChoice){
                case 1:
                    removeExtensionSorting(config.sourceDirectory, destinationMap);
                case 2:
                    removeAlphabeticalSorting(config.sourceDirectory);
            }
            break;
    }
    logger.displaySummary();
    hUtils::pause();
    return 0;
}