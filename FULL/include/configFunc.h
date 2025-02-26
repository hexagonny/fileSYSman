#ifndef CONFIGFUNC_H
#define CONFIGFUNC_H

#include <string>

#include "configStruct.h"

//  This function creates a new configuration file if the user does not have it.
void createTextConfig(const std::string &fileName);
//  This function removes'\t' in every single line of string.
std::string trim(const std::string &str);
//  This function reads every line of string and filters out the unnecessary characters.
Config readConfig(const std::string& fileName);
//  This function displays the current destinations for checking and debugging.
void displayCurrentDir(const std::vector<std::filesystem::path>& initialPaths,
                       const std::filesystem::path& sourceDir,
                       const std::unordered_map<std::string, std::filesystem::path>& destMap);

#endif