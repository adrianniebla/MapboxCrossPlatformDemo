//
// Created by Adrian Niebla
//

#pragma once
#include "Canvas.h"
#include "MapDelegate.h"
#include "MapTileRenderer.h"

struct TileRange {
    TileRange(int x1, int x2, int y1, int y2, int8_t z): minX{x1}, maxX{x2}, minY{y1}, maxY{y2}, zoom{z} {}
    int minX, maxX, minY, maxY, zoom;
};


class CanvasMap : public Canvas {
public:
    CanvasMap(int width, int height);
    Bounds getWorld() const { return Bounds(.0f, .0f, MapTile::WORLD_SIZE, MapTile::WORLD_SIZE); };
    void redrawFrame() override;
    void setMapDelegate(MapDelegate* mapDelegate);
    void setTileRenderer(std::unique_ptr<MapTileRenderer> renderer);
    void updateScreen(int width, int height) override;
    MapTileRenderer* getRenderer() const { return mRenderer.get(); };
    MapDelegate* mMapDelegate = nullptr;
private:
    std::unique_ptr<MapTileRenderer> mRenderer = nullptr;
    TileRange getRenderingRange();
    static int getLowestFilledZoomLevel(const std::vector<std::vector<MapTile*>>& tiles) ;
    void zoomOut();
};
