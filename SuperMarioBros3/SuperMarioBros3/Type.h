#pragma once

#include <string>
#include <d3dx9math.h>

using Point = D3DXVECTOR2;
using Point3D = D3DXVECTOR3;
using Matrix = D3DXMATRIX;

using Texture = LPDIRECT3DTEXTURE9;

// Primary type
struct String : public std::string
{
    String(const std::string& str) : std::string(str) {};
    String(const char* chars) : std::string(chars) {};

    auto toArray() {
        return this->c_str();
    }
};