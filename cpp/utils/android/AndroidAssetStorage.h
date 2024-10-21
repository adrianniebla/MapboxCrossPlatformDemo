//
// Created by Adrian Niebla on 20/10/24.
//

#pragma once
#include "OSUtils.h"
#include "AssetStorage.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

class AndroidAssetStorage : public AssetStorage {
public:
    AndroidAssetStorage(AAssetManager* manager);
    ~AndroidAssetStorage();
    std::string readFile(const std::string& filename) override;
private:
    AAssetManager* mAssetManager = nullptr;
};
