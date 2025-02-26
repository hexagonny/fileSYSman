#ifndef REMOVEFUNC_H
#define REMOVEFUNC_H

#include "configStruct.h"

void removeFolder(const std::filesystem::path& destination);

void removeExtensionSorting(const std::filesystem::path& sourceDir,
                            const std::unordered_map<std::string, std::filesystem::path>& destMap);

void removeAlphabeticalSorting(const std::filesystem::path& sourceDir);

#endif