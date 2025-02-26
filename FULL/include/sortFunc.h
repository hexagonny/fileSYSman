#ifndef SORTFUNC_H
#define SORTFUNC_H

#include "configStruct.h"

void moveFile(const std::filesystem::path& entry, const std::filesystem::path& destination);
/*
  It moves common document files from different directories to the prefered documents folder.
  The algorithm is based on file extentions and multiple directories(eg. downloads folder).
*/
void moveToSourceDir(const std::vector<std::filesystem::path>& initialPaths,
                     const std::filesystem::path& sourceDir,
                     const std::unordered_map<std::string, std::filesystem::path>& destMap);
/*
  It organizes cluttered documents into their respective folders.
  The algorithm is based on file extentions and their matching folder names.
*/
void sortByExtension(const std::filesystem::path& sourceDir,
                     const std::unordered_map<std::string, std::filesystem::path>& destMap);

void sortAlphabetically(const std::filesystem::path& sourceDir);

#endif