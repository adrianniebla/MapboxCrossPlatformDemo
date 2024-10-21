//
//  MapVisualization.swift
//  Canvas
//
//  Created by Adrian Niebla
//  Copyright © 2020 Adrian Niebla. All rights reserved.
//

public class MapVisualization: Visualization {
    var mapBridge: MapVisualizationBridge?
    private var mapDelegate: MapDelegate? = nil
    private var intermediateMapDelegate: IntermediateMapDelegate? = nil
    public override init(frame: CGRect) {
        super.init(frame: frame)
    }
    public required init?(coder: NSCoder) {
        super.init(coder: coder)
    }
    override internal func getVisualization() -> VisualizationBridge? {
        return mapBridge
    }

    override internal func createVisualization() {
        mapBridge = MapVisualizationBridge(Int32(self.frameSize.width), Int32(self.frameSize.height))
    }

    /// Pass the MapImageDelegate implementation that will return the map tile images when requested.
    /// - Parameter delegate: An implementation of the MapImageDelegate protocol
    public func setMapImageDelegate(delegate: MapDelegate) {
        self.queue.async(flags: .barrier) { [weak self] in
            self?.blocks.append {
                self?.mapDelegate = delegate
                self?.intermediateMapDelegate = IntermediateMapDelegate(delegate: delegate)
                self?.mapBridge?.setMapDelegate(self?.intermediateMapDelegate?.getDelegateBridge())
            }
        }
    }

    /// Fulfill a map tile request for image oif the tile to render.
    /// - Parameters:
    ///   - x: position of tile in x
    ///   - y: position of tile in y
    ///   - zoom: zoom of tiles
    ///   - img: Image of the tile from the API calls
    public func fulfillMapRequest(x: Int, y: Int, zoom: Int, img: UIImage) {
        queue.async(flags: .barrier) { [weak self] in
            self?.blocks.append {
                // Must add autoreleasepool, the context?.draw in the pixelData function will leak memory if not.
                autoreleasepool { () -> Void in
                    if let pixelData = img.pixelData() {
                        self?.mapBridge?.setMapImageData(pixelData, Int32(pixelData.count), Int32(x), Int32(y),
                                                         Int32(zoom), Int32(img.size.width * img.scale),
                                                         Int32(img.size.height * img.scale))
                    }
                }

            }
        }
    }
}
extension UIImage {
   func pixelData() -> [Int8]? {
        let size = self.size
        let dataSize = size.width * size.height * 4
        var pixelData = [Int8](repeating: 0, count: Int(dataSize))
        let colorSpace = CGColorSpaceCreateDeviceRGB()
        let context = CGContext(data: &pixelData,
                               width: Int(size.width),
                               height: Int(size.height),
                               bitsPerComponent: 8,
                               bytesPerRow: 4 * Int(size.width),
                               space: colorSpace,
                               bitmapInfo: CGImageAlphaInfo.noneSkipLast.rawValue)
       guard let cgImage = self.cgImage else { return nil }
       context?.draw(cgImage, in: CGRect(x: 0, y: 0, width: size.width, height: size.height))
       return pixelData
   }
}
