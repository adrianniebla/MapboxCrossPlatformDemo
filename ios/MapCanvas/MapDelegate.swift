//
//  MapDelegate.swift
//  Canvas
//
//  Created by Adrian Niebla
//  Copyright © 2020 Adrian Niebla. All rights reserved.
//

import Foundation

@objc public protocol MapDelegate {
    func requestMapTile(x: Int, y: Int, zoom: Int) -> Void
}
