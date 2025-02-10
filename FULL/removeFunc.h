#ifndef REMOVEFUNC_H
#define REMOVEFUNC_H

#include "configStruct.h"
#include "logMessages.h"

void removeFolder(const path& destination);

void removeExtensionSorting(const path& sourceDir, auto& destMap);

void removeAlphabeticalSorting(const path& sourceDir);

#endif