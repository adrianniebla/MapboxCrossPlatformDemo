//
// Created by Adrian Niebla on 20/10/24.
//

#pragma once
#include <string>

class AssetStorage {
public:
    AssetStorage() = default;
    virtual ~AssetStorage() = default;
    virtual std::string readFile(const std::string& filename) = 0;
};