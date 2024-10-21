//
// Created by Adrian Niebla
//
#pragma once

class MapDelegate {
public:
    virtual void requestMapTile(int x, int y, int zoom) = 0;
    virtual ~MapDelegate() = default;
};
