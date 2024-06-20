/*
 * File: /tomato/tomato/include/Reader.h
 * 
 * Created the 13 June 2024, 11:56 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#pragma once

#include <string>
#include <filesystem>
#include "Context.h"

namespace fs = std::filesystem;

namespace tomato {
    class LuauReader {
    public:
        static LuauContext getContextFromFile(const fs::path &filepath) noexcept(false);
        static LuauContext getContextFromText(const std::string &luauScript, const std::string &chunkName) noexcept(false);
    };
}