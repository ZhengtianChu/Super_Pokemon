#pragma once
#include "DisplayableObject.h"
class MyObjectB :
    public DisplayableObject
{
public:
    MyObjectB(BaseEngine* pEngine)
        : DisplayableObject(pEngine) {
        m_iCurrentScreenX = 100; // Starting position on the screen
        m_iCurrentScreenY = 200;
        m_iDrawWidth = 100; // Width of drawing area
        m_iDrawHeight = 200; // Height of drawing area
    }
    void virtDraw();
    void virtDoUpdate(int iCurrentTime);
};


