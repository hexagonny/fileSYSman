#ifndef SORTFUNC_H
#define SORTFUNC_H

#include "configStruct.h"
#include "logMessages.h"

void moveFile(const path& entry, const path& destination);
/*
  It moves common document files from different directories to the prefered documents folder.
  The algorithm is based on file extentions and multiple directories(eg. downloads folder).
*/
void moveToSourceDir(const vector<path>& initialPaths,
                     const path& sourceDir,
                     const unordered_map<string, path>& destMap);
/*
  It organizes cluttered documents into their respective folders.
  The algorithm is based on file extentions and their matching folder names.
*/
void sortByExtension(const path& sourceDir,
                     const unordered_map<string, path>& destMap);

void sortAlphabetically(const path& sourceDir);

#endif