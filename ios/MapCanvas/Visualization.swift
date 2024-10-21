//
//  CanvasVisualization.swift
//  Canvas
//
//  Created by Adrian Niebla
//  Copyright © 2020 Adrian Niebla. All rights reserved.
//

import UIKit
import GLKit

public class Visualization: UIView, UIGestureRecognizerDelegate {
    private let FPS: Double = 30.0
    private weak var glLayer: CAEAGLLayer? = nil
    private var context: EAGLContext?
    private var renderBuffer: GLuint = 0
    private var frameBuffer: GLuint = 0
    internal var frameSize: CGSize!
    internal var thread: Thread? = nil
    internal var canvasBridge: VisualizationBridge? = nil
    

    private var timeElapsed: Double = 0.0
    private var lastTimeStamp: Double = 0.0

    internal var hasPrevPinch = false
    internal var prevPinchSpan = CGPoint(x: 0.0, y: 0.0)
    internal var panRecognizer: UIPanGestureRecognizer?
    internal var pinchRecognizer: UIPinchGestureRecognizer?
    internal var singleTap: UITapGestureRecognizer?
    internal var doubleTap: UITapGestureRecognizer?
    internal var longPress: UILongPressGestureRecognizer?

    internal let queue = DispatchQueue(label: "CanvasBlockQueue", attributes: .concurrent)
    internal var blocks: [() -> Void] = []

    public override init(frame: CGRect) {
        super.init(frame: frame)
        self.initialize()
    }
    public required init?(coder: NSCoder) {
        super.init(coder: coder)
        self.initialize()
    }
    private func initialize() {
        self.autoresizingMask = [.flexibleWidth, .flexibleHeight]
        glLayer = self.layer as? CAEAGLLayer
        glLayer?.drawableProperties = [kEAGLDrawablePropertyRetainedBacking: false]
        self.frameSize = self.frame.size
        createGestureListeners()
        startDrawLoop()
    }

    override public func layoutSubviews() {
        super.layoutSubviews()
        glLayer?.frame = self.frame
        self.frameSize = self.frame.size
        let scale = UIScreen.main.nativeScale
        let width = Float(self.frameSize.width * scale)
        let height = Float(self.frameSize.height * scale)
        self.queue.async(flags: .barrier) { [weak self] in
            self?.blocks.append {
                self?.getVisualization()?.setFrame(width, height)
                glViewport(0, 0, GLsizei(width), GLsizei(height))
            }
        }
    }

    internal func startDrawLoop () {
        self.thread = Thread(block: { [weak self] in
            usleep(10000)
            self?.initializeOpenGL()
            self?.createVisualization()
            while !(self?.thread?.isCancelled ?? true) {
                self?.updateFrameTimer()
                self?.drawFrame()
                self?.queue.sync {
                    while (self?.blocks.count ?? 0) > 0 {
                        self?.blocks.remove(at: 0)()
                    }
                }
            }
        })
        self.thread?.start()
    }
    internal func drawFrame() {
        EAGLContext.setCurrent(self.context)
        glBindFramebuffer(GLenum(GL_FRAMEBUFFER), self.frameBuffer)
        glBindRenderbuffer(GLenum(GL_RENDERBUFFER), self.renderBuffer)
        if self.performUpdate(timeElapsed: self.timeElapsed) {
            glBindFramebuffer(GLenum(GL_DRAW_FRAMEBUFFER), self.frameBuffer)
            glBindFramebuffer(GLenum(GL_READ_FRAMEBUFFER), self.frameBuffer)
            self.context?.presentRenderbuffer(Int(GL_RENDERBUFFER))
            glGetError()
        }
        glBindFramebuffer(GLenum(GL_FRAMEBUFFER), 0)
        glBindRenderbuffer(GLenum(GL_RENDERBUFFER), 0)
        glFlush()
    }

    private func performUpdate(timeElapsed: Double) -> Bool{
        return timeElapsed == 0.0 ? false : self.getVisualization()?.renderFrame(Float(timeElapsed)) ?? false
    }


    internal func getVisualization() -> VisualizationBridge? {
        return canvasBridge
    }

    internal func createVisualization() {
        canvasBridge = VisualizationBridge(Int32(self.frameSize.width), Int32(self.frameSize.height));
    }

    private func updateFrameTimer() {
        var currentTimeStamp: Double = CACurrentMediaTime()
        let waitSeconds: UInt32 = UInt32((max(0.0, (1.0 / FPS) - (currentTimeStamp - lastTimeStamp))) * 1000000)
        usleep(max(0, waitSeconds))
        currentTimeStamp = CACurrentMediaTime()
        timeElapsed = currentTimeStamp - lastTimeStamp
        lastTimeStamp = currentTimeStamp
    }

    private func initializeOpenGL() {
        self.context = EAGLContext(api: EAGLRenderingAPI.openGLES3)
        guard let initialContext = self.context else {
            print("EAGLContext could not be created")
            exit(1)
        }
        guard EAGLContext.setCurrent(self.context) else {
            print("Failed to set current context")
            exit(1)
        }
        print("context has been set \(self)")
        let nativeScale: CGFloat = UIScreen.main.nativeScale

        self.glLayer?.contentsScale = UIScreen.main.nativeScale

        let pixelWidth: GLsizei = GLsizei(self.frameSize.width * nativeScale)
        let pixelHeight: GLsizei = GLsizei(self.frameSize.height * nativeScale)

        // Setup the render buffer
        glGenRenderbuffers(1, &self.renderBuffer)
        glBindRenderbuffer(GLenum(GL_RENDERBUFFER), self.renderBuffer)

        glRenderbufferStorage(GLenum(GL_RENDERBUFFER), GLenum(GL_DEPTH_COMPONENT16), pixelWidth, pixelHeight)
        usleep(10000)

        initialContext.renderbufferStorage(Int(GL_RENDERBUFFER), from: self.glLayer)

        CATransaction.flush()

        // setup the frame buffer
        glGenFramebuffers(1, &self.frameBuffer)
        glBindFramebuffer(GLenum(GL_FRAMEBUFFER), self.frameBuffer)
        glFramebufferRenderbuffer(GLenum(GL_FRAMEBUFFER), GLenum(GL_COLOR_ATTACHMENT0), GLenum(GL_RENDERBUFFER), self.renderBuffer)

        glViewport(0, 0, GLsizei(pixelWidth), GLsizei(pixelHeight))
    }

    public override class var layerClass: AnyClass {
        return CAEAGLLayer.self
    }
}
