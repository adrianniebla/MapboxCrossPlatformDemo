//
//  MapDelegateBridge.h
//  Canvas
//
//  Created by Adrian Niebla
//  Copyright © 2020 Adrian Niebla. All rights reserved.
//

#pragma once
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class IntermediateMapDelegate;

@interface MapDelegateBridge : NSObject {
    @private
    __weak IntermediateMapDelegate* mapDelegate;
}

-(id) init: (IntermediateMapDelegate*) delegate;
-(void) requestMapTile: (int) x : (int) y : (int) zoom;
-(void*) getReference;

@end

