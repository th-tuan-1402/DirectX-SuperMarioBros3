#pragma once

#include <string>
#include "Ultis.h"

// Primary type
struct String : public std::string
{
    String(const std::string& str) : std::string(str) {};
    String(const char* chars) : std::string(chars) {};

    auto toArray() {
        return this->c_str();
    }

    operator LPCWSTR() {
        return ToLPCWSTR(this->toArray());
    }
};