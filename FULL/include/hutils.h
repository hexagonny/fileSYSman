#ifndef HUTILS_H
#define HUTILS_H

#pragma once


#ifdef _WIN32
    #include <windows.h>
#else
    #include <cstdlib>
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

#include <string>

namespace hUtils {

    // --- SYSTEM UTILITIES ---
    void setConsoleWindowSize();  //  Adjust console size.
    void pause();                 //  Cross-platform system pause.
    void sleep(int milliseconds); //  Sleep for a given duration.

    // --- TEXT UTILITIES ---
    namespace Text
    {
        void toLine            (char character = '-');     //  Print a line of repeated characters
        void toCentered        (std::string text,          //  Prints centered text.
                                int colorCode = 0,
                                int number = 0);      
        void toRight           (std::string text,          //  Moves text to the right.
                                int colorCode = 0);  
        void toLeft            (std::string text,          //  Moves text to the left.
                                int tab = 0,
                                int colorCode = 0,
                                int number = 0);
        std::string toLowerCase(std::string text);         //  Convert string to lowercase.

        std::string color      (int textColor = 0);        //  Get ANSI color codes.
        std::string defaultText();                         //  Reset text color.

        void clearAll();                                   //  Clears every output in the terminal.
        void clearLine         (int line);                 //  Clears an assigned line and below it.
    };
}

// --- LOGGER UTILITIES ---
struct Logger
{
    int filesMoved = 0, foldersRemoved = 0, foldersCreated = 0, errors = 0, warnings = 0;

    void logAction     (const std::string& message);
    void logSuccess    (const std::string& message);
    void logMoved      (const std::string& message); //  filesMoved
    void logRemoved    (const std::string& message); //  filesRemoved
    void logCreated    (const std::string& message); //  foldersCreated
    void logError      (const std::string& message); //  errors
    void logWarning    (const std::string& message); //  warnings
    void displaySummary();
};

extern Logger logger;

#endif