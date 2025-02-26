#ifndef CONFIGSTRUCT_H
#define CONFIGSTRUCT_H

#include <vector>
#include <filesystem>
#include <unordered_map>

using namespace std::filesystem;

struct Config
{
    path sourceDirectory;
    std::vector<path> initialPaths;
};

#endif