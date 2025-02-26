#include "filesysman.h"

using namespace std;

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

void clearAll()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void clearLine(int line)
{
    cout<<"\033["<<line<<";1H";
    cout<<"\033[J";
}

Logger logger;

void Logger::logAction(const string& message)
{
    cout<<message<<'\n';
}

void Logger::logSuccess(const string& message)
{
    setColor(92); cout.flush();
    cout<<message<<'\n'; resetColor();
}

void Logger::logMoved(const string& message)
{
    setColor(93); cout.flush();
    cout<<message<<'\n'; resetColor();
    filesMoved++;
}

void Logger::logRemoved(const string& message)
{
    setColor(93); cout.flush();
    cout<<message<<'\n'; resetColor();
    foldersRemoved++;
}

void Logger::logCreated(const string& message)
{
    setColor(92); cout.flush();
    cout<<message<<'\n'; resetColor();
    foldersCreated++;
}

void Logger::logError(const string& message)
{
    setColor(91); cerr.flush();
    cerr<<message<<'\n'; resetColor();
    errors++;
}

void Logger::logWarning(const string& message)
{
    setColor(95); cerr.flush();
    cerr<<message<<'\n'; resetColor();
    warnings++;
}

void Logger::displaySummary()
{
    cout<<"\nSummary:\n\n"
        <<setw(17)<<left<<"Files Moved: "    <<filesMoved<<'\n'
        <<setw(17)<<left<<"Folders Removed: "<<foldersRemoved<<'\n'
        <<setw(17)<<left<<"Folders Created: "<<foldersCreated<<'\n'
        <<setw(17)<<left<<"Warnings: "       <<warnings<<'\n'
        <<setw(17)<<left<<"Errors: "         <<errors<<"\n\n";
}