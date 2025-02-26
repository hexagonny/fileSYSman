#include "hutils.h"

#include <iostream>
#include <iomanip>

using std::cout;
using std::cerr;
using std::string;
using std::setw;
using std::left;
using namespace hUtils;

Logger logger;

void Logger::logAction(const string& message)
{
    cout << message << '\n';
}

void Logger::logSuccess(const string& message)
{
    cout.flush();
    cout << Text::color(92) << message << '\n' << Text::defaultText(); 
}

void Logger::logMoved(const string& message)
{
    cout.flush();
    cout << Text::color(93) << message<<'\n' << Text::defaultText(); 
    filesMoved++;
}

void Logger::logRemoved(const string& message)
{
    cout.flush();
    cout << Text::color(93) << message << '\n' << Text::defaultText(); 
    foldersRemoved++;
}

void Logger::logCreated(const string& message)
{
    cout.flush();
    cout << Text::color(92) << message << '\n' << Text::defaultText(); 
    foldersCreated++;
}

void Logger::logError(const string& message)
{
    cerr.flush();
    cerr << Text::color(91) << message << '\n' << Text::defaultText(); 
    errors++;
}

void Logger::logWarning(const string& message)
{
    cerr.flush();
    cerr << Text::color(95) << message << '\n' << Text::defaultText(); 
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