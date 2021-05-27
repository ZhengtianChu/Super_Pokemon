#pragma once
#include "TileManager.h"
class Scyzc3TileManager :
    public TileManager
{
public:
    Scyzc3TileManager()
        : TileManager(26, 26)
    {
        grass = ImageManager::loadImage("images/grass.png", true);
        floor = ImageManager::loadImage("images/floor.png", true);
        water = ImageManager::loadImage("images/water.png", true);
        wall1 = ImageManager::loadImage("images/wall1.png", true);
        tree = ImageManager::loadImage("images/tree.png", true);
        door = ImageManager::loadImage("images/door.png", true);
        door2 = ImageManager::loadImage("images/door2.png", true);
        key = ImageManager::loadImage("images/key.png", true);
        heart = ImageManager::loadImage("images/heart.png", true);
        sword = ImageManager::loadImage("images/sword.png", true);
        sand = ImageManager::loadImage("images/sand.jpg", true);
    }

    ~Scyzc3TileManager(void) { printf("delete tile"); };

    virtual void virtDrawTileAt(
        BaseEngine* pEngine,
        DrawingSurface* pSurface,
        int iMapX, int iMapY,
        int iStartPositionScreenX, int iStartPositionScreenY) const override;

   

private:
    SimpleImage grass;
    SimpleImage floor;
    SimpleImage water;
    SimpleImage door;
    SimpleImage door2;
    SimpleImage wall1;
    SimpleImage tree;
    SimpleImage key;
    SimpleImage heart;
    SimpleImage sword;
    SimpleImage sand;
};

