//
//  MapDelegateBridge.m
//  Canvas
//
//  Created by Adrian Niebla
//  Copyright © 2020 Adrian Niebla. All rights reserved.
//


#import <Foundation/Foundation.h>
#import "MapDelegateBridge.h"
#import "MapDelegate.h"
#import <UIKit/UIKit.h>

#if __has_include("MapCanvas/MapCanvas-Swift.h")
#include <MapCanvas/MapCanvas-Swift.h>
#else
#include <MapCanvas-Swift.h>
#endif

class IOSMapDelegate: public MapDelegate {
public:
    IOSMapDelegate(MapDelegateBridge* bridge): bridge{bridge} {}
    void requestMapTile(int x, int y, int zoom) override {
        [bridge requestMapTile:x :y :zoom];
    };
private:
    MapDelegateBridge* bridge;
};

@interface MapDelegateBridge() <NSObject>
@property (nonatomic) IOSMapDelegate* delegate;
@end

@implementation MapDelegateBridge

-(id) init: (IntermediateMapDelegate*) delegate {
    self->mapDelegate = delegate;
    self->_delegate = new IOSMapDelegate(self);
    return self;
}
-(void) requestMapTile: (int) x : (int) y : (int) zoom {
    @autoreleasepool {
        [self->mapDelegate requestMapTileWithX:x y:y zoom:zoom];
    }
}

-(void*) getReference {
    return static_cast<void*>(_delegate);
}

@end
