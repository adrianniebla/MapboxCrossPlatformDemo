//
//  MapVisualizationBridge.h
//  Canvas
//
//  Created by Adrian Niebla
//  Copyright © 2020 Adrian Niebla. All rights reserved.
//

#pragma once
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "VisualizationBridge.h"
#import "MapDelegateBridge.h"

@interface MapVisualizationBridge : VisualizationBridge {}

-(id) init: (int) width : (int) height;
-(void) setMapDelegate: (MapDelegateBridge*) delegate;
-(void) setMapImageData: (const char*) data : (int) length : (int) x : (int) y : (int) zoom : (int) width : (int) height;

@end

