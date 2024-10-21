//
//  iOSMapDelegate.hpp
//  MapCanvas
//
//  Created by Adrian Niebla on 20/10/24.
//

#pragma once
#include "MapDelegate.h"
#include "OSUtils.h"

class iOSMapDelegate : public MapDelegate {
public:
    iOSMapDelegate();
    ~iOSMapDelegate() override;
    void requestMapTile(int x, int y, int zoom) override;
};
