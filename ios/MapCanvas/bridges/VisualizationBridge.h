//
//  CanvasVisualizationBridge.h
//  Canvas
//
//  Created by Adrian Niebla
//  Copyright © 2020 Adrian Niebla. All rights reserved.
//

#pragma once
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>



@interface VisualizationBridge : NSObject {
    @protected void *canvas;
}

-(id) init : (int) width : (int) height;
-(bool) renderFrame: (float) timeElapsed;
// touch events
-(void) handlePan: (float) x : (float) y : (float) xDelta : (float) yDelta : (bool) hasEnded : (float) xVelOnDone : (float) yVelOnDone;
-(void) handlePinch: (float) x : (float) y : (float) oldSpanX : (float) oldSpanY : (float) newSpanX : (float) newSpanY : (bool) isDone : (float) velocity;
-(void) setFrame: (float) width : (float) height;

@end

