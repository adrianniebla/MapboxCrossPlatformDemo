//
//  MapVisualizationBridge.m
//  Canvas
//
//  Created by Adrian Niebla
//  Copyright © 2020 Adrian Niebla. All rights reserved.
//

#import "MapVisualizationBridge.h"
#import "CanvasMap.h"
#import "GlMapTileRenderer.h"
#import "iOSAssetManager.h"

@implementation MapVisualizationBridge

-(id) init: (int) width : (int) height {
    @autoreleasepool {
        auto canvasPtr = new CanvasMap(width, height);
        canvas = canvasPtr;
        canvasPtr->setStorageManager(std::make_unique<iOSAssetStorage>());
        canvasPtr->setTileRenderer(std::make_unique<GlMapTileRenderer>(canvasPtr->getStorage()));
        return self;
    }
}

-(void) setMapDelegate: (MapDelegateBridge*) delegate {
    @autoreleasepool {
        auto mapDelegate = static_cast<MapDelegate*>(delegate.getReference);
        static_cast<CanvasMap*>(canvas)->setMapDelegate(mapDelegate);
    }
}

-(void) setMapImageData: (const char*) data : (int) length : (int) x : (int) y : (int) zoom : (int) width : (int) height {
    @autoreleasepool {
        auto map = static_cast<CanvasMap*>(canvas);
        map->getRenderer()->setData(x, y, zoom, data, length, width, height);
    }
}

@end
