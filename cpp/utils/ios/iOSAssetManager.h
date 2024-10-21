//
//  iOSAssetManager.hpp
//  MapCanvas
//
//  Created by Adrian Niebla on 20/10/24.
//

#pragma once
#include "OSUtils.h"
#include "AssetStorage.h"

class iOSAssetStorage : public AssetStorage {
public:
    iOSAssetStorage();
    ~iOSAssetStorage();
    std::string readFile(const std::string& filename) override;
};
