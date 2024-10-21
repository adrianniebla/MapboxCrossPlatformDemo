//
//  IntermediateMapImageDelegate.swift
//  Canvas
//
//  Created by Adrian Niebla
//  Copyright © 2020 Adrian Niebla. All rights reserved.
//

import Foundation

@objc public class IntermediateMapDelegate: NSObject, MapDelegate {
    private let delegate: MapDelegate
    private var delegateBridge: MapDelegateBridge!
    public init(delegate: MapDelegate) {
        self.delegate = delegate
        super.init()
        self.delegateBridge = MapDelegateBridge(self)
    }
    @objc public func requestMapTile(x: Int, y: Int, zoom: Int) {
        DispatchQueue.global().async { [weak self] in
            self?.delegate.requestMapTile(x: x, y: y, zoom: zoom)
        }
    }
    public func getDelegateBridge() -> MapDelegateBridge {
        return delegateBridge
    }
}
