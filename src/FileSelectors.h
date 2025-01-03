#pragma once

#include <optional>
#include <string>
#include <vector>

#include <nfd.hpp>

std::optional<std::string> chooseProjectFile();

std::optional<std::string> chooseOutputCSV();

std::optional<std::string> chooseParentDirectory();

std::vector<std::string> chooseImageFiles();


