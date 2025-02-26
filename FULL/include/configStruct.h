#ifndef CONFIGSTRUCT_H
#define CONFIGSTRUCT_H

#include <vector>
#include <filesystem>
#include <unordered_map>

struct Config
{
    std::filesystem::path sourceDirectory;
    std::vector<std::filesystem::path> initialPaths;
};

#endif