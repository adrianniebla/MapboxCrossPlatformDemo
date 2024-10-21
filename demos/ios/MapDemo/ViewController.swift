//
//  ViewController.swift
//  MapDemo
//
//  Created by Adrian Niebla on 20/10/24.
//

import Foundation
import MapCanvas

class MapCallback : MapDelegate {
    let mView: MapVisualization
    let apiKey = ""
    init (view: MapVisualization) {
        self.mView = view
        assert(!apiKey.isEmpty, "Missing apikey")
    }
    func requestMapTile(x: Int, y: Int, zoom: Int) {
        let imgUrl = URL(string: "https://api.mapbox.com/v4/mapbox.satellite/\(zoom)/\(x)/\(y).jpg90?access_token=\(apiKey)")!
        URLSession.shared.dataTask(with: imgUrl) { [weak self] (data, response, err) in
            if let validData = data, let img = UIImage(data: validData) {
                self?.mView.fulfillMapRequest(x: x, y: y, zoom: zoom, img: img)
            }
        }.resume()
    }
}

class ViewController: UIViewController {
    @IBOutlet weak var map: MapVisualization!
    override func viewDidLoad() {
        super.viewDidLoad()
        map.setMapImageDelegate(delegate: MapCallback(view: map))
        // Do any additional setup after loading the view.
    }
}

