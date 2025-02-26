#ifndef LOGMESSAGES_H
#define LOGMESSAGES_H

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

#include <iomanip>
#include <chrono>
#include <thread>
#include <stdexcept>

using namespace std;

//  Function to set the console text color using ANSI escape codes.
void setColor(int textColor);

//  Function to reset the console color
void resetColor();

//  This function exists for better output handling.
void sleep(int miliseconds);

//  Clears every output in the terminal.
void clearAll();

//  Entirely clears a specific line.
void clearLine(int line);

struct Logger
{
    int filesMoved = 0, foldersRemoved = 0,
        foldersCreated = 0, errors = 0, warnings = 0;

    void logAction(const string& message);
    void logSuccess(const string& message);
    void logMoved(const string& message);
    void logRemoved(const string& message);
    void logCreated(const string& message);
    void logError(const string& message);
    void logWarning(const string& message);
    void displaySummary();
};

extern Logger logger;

#endif