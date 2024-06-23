/*
 * File: /tomato/tomato/include/LuauReader.h
 * 
 * Created the 20 May 2024, 01:36 am by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#pragma once

#include <string>
#include <filesystem>
#include "LuauState.h"

namespace fs = std::filesystem;

class LuauReader {
public:
    static LuauState getContextFromFile(const fs::path &filepath) noexcept(false);
    static LuauState getContextFromText(const std::string &luauScript, const std::string &chunkName) noexcept(false);
};