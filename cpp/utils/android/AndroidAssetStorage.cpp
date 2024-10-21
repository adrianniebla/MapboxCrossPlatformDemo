//
// Created by Adrian Niebla on 20/10/24.
//

#include "AndroidAssetStorage.h"

AndroidAssetStorage::AndroidAssetStorage(AAssetManager *manager) : AssetStorage() {
    this->mAssetManager = manager;
}

AndroidAssetStorage::~AndroidAssetStorage() {

}

std::string AndroidAssetStorage::readFile(const std::string &filename) {
    AAsset* asset = AAssetManager_open(mAssetManager, filename.c_str(), AASSET_MODE_BUFFER);
    auto length = AAsset_getLength(asset);
    char* content = new GLchar[length + 1];
    AAsset_read(asset, content, length);
    content[length] = '\0';
    AAsset_close(asset);
    return content;
}
