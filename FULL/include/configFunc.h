#ifndef CONFIGFUNC_H
#define CONFIGFUNC_H

#include <fstream>
#include <string>

#include "configStruct.h"
#include "logMessages.h"

//  This function creates a new configuration file if the user does not have it.
void createTextConfig(const string &fileName);
//  This function removes'\t' in every single line of string.
string trim(const string &str);
//  This function reads every line of string and filters out the unnecessary characters.
Config readConfig(const string& fileName);
//  This function displays the current destinations for checking and debugging.
void displayCurrentDir(const vector<path>& initialPaths,
                       const path& sourceDir,
                       const unordered_map<string, path>& destMap);

#endif