//
//  CanvasVisualization+Extension.swift
//  Canvas
//
//  Created by Adrian Niebla
//  Copyright © 2020 Adrian Niebla. All rights reserved.
//

extension Visualization {
    internal func createGestureListeners() {
        panRecognizer = UIPanGestureRecognizer(target: self, action: #selector(panFunction))
        pinchRecognizer = UIPinchGestureRecognizer(target: self, action: #selector(pinchFunction))

        addGestureRecognizer(panRecognizer!)
        addGestureRecognizer(pinchRecognizer!)


        panRecognizer?.cancelsTouchesInView = false
        pinchRecognizer?.cancelsTouchesInView = false

        panRecognizer?.delegate = self
        pinchRecognizer?.delegate = self
    }

    @objc private func panFunction(sender: UIPanGestureRecognizer) {
        let translation = sender.translation(in: self)
        let velocity = sender.velocity(in: self)
        let location = sender.location(in: self)

        let xDelta = Float(translation.x * contentScaleFactor)
        let yDelta = Float(translation.y * contentScaleFactor)
        let x = Float(location.x * contentScaleFactor)
        let y = Float(location.y * contentScaleFactor)
        let xVelOnDone = Float(velocity.x * contentScaleFactor)
        let yVelOnDone = Float(velocity.y * contentScaleFactor)
        let hasEnded = sender.state == UIGestureRecognizer.State.ended
        sender.setTranslation(CGPoint(x: 0.0, y: 0.0), in: self)
        self.blocks.append {
            self.getVisualization()?.handlePan(x, y, xDelta, yDelta, hasEnded, xVelOnDone, yVelOnDone)
        }
    }
    @objc private func pinchFunction(sender: UIPinchGestureRecognizer) {
        let location = sender.location(in: self)
        let hasEnded = sender.state == UIGestureRecognizer.State.ended

        var prevXSpan = prevPinchSpan.x
        var prevYSpan = prevPinchSpan.y
        var currentXSpan = CGFloat(0.0)
        var currentYSpan = CGFloat(0.0)

        if sender.numberOfTouches != 1 {
            let t1 = sender.location(ofTouch: 0, in: self)
            let t2 = sender.location(ofTouch: 1, in: self)
            currentXSpan = abs(t2.x - t1.x)
            currentYSpan = abs(t2.y - t1.y)
        } else {
            currentXSpan = prevXSpan
            currentYSpan = prevYSpan
        }
        if !hasPrevPinch {
            prevXSpan = currentXSpan / sender.scale
            prevYSpan = currentYSpan / sender.scale
        }

        let x = location.x * contentScaleFactor
        let y = location.y * contentScaleFactor
        let velocity = sender.velocity
        self.blocks.append {
            self.getVisualization()?.handlePinch(Float(x), Float(y), Float(prevXSpan), Float(prevYSpan),
                                                 Float(currentXSpan), Float(currentYSpan), hasEnded, Float(velocity))
        }
        prevPinchSpan = CGPoint(x: currentXSpan, y: currentYSpan)
        hasPrevPinch = !hasEnded
        sender.scale = 1.0
    }
}
