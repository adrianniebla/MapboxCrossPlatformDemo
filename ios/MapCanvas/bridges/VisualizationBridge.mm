//
//  VisualizationBridge.mm
//  Canvas
//
//  Created by Adrian Niebla
//  Copyright © 2020 Adrian Niebla. All rights reserved.
//

#import "VisualizationBridge.h"
#import "Canvas.h"

@implementation VisualizationBridge

-(id) init : (int) width : (int) height {
    @autoreleasepool {
        canvas = new Canvas(width, height);
        return self;
    }
}
-(bool) renderFrame: (float) timeElapsed {
    @autoreleasepool {
        static_cast<Canvas*>(canvas)->redrawFrame();
        return true;
    }
}
-(void)dealloc {
    delete static_cast<Canvas*>(canvas);
}
// touch events
-(void) handlePan: (float) x : (float) y : (float) xDelta : (float) yDelta : (bool) hasEnded : (float) xVelOnDone : (float) yVelOnDone {
    @autoreleasepool {
        static_cast<Canvas*>(canvas)->onPan(x, y, xDelta, yDelta, hasEnded, xVelOnDone, yVelOnDone);
    }
}
-(void) handlePinch: (float) x : (float) y : (float) oldSpanX : (float) oldSpanY : (float) newSpanX : (float) newSpanY : (bool) isDone : (float) velocity {
    @autoreleasepool {
        static_cast<Canvas*>(canvas)->onZoom(x, y, oldSpanX, oldSpanY, newSpanX, newSpanY, isDone);
    }
}
-(void) setFrame: (float) width : (float) height {
    @autoreleasepool {
        static_cast<Canvas*>(canvas)->updateScreen(width, height);
    }
}

@end
