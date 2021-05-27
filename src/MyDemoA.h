#pragma once
#include "BaseEngine.h"
#include "MyTileManager.h"

class MyDemoA :
    public BaseEngine
{
public:
    void virtSetupBackgroundBuffer() override;
    void virtMouseDown(int iButton, int iX, int iY);
    void virtKeyDown(int iKeyCode);

protected:
    MyTileManager tm; // Insert this line (for composition)!!!


public:
    int virtInitialiseObjects();
};

