#pragma once
#include "TileManager.h"
class MyTileManager :
    public TileManager
{
public: 
    MyTileManager()
        : TileManager(20, 20, 15, 15)
    {
    }

    virtual void virtDrawTileAt(
        BaseEngine* pEngine,
        DrawingSurface* pSurface,
        int iMapX, int iMapY,
        int iStartPositionScreenX, int iStartPositionScreenY) const override;
};

