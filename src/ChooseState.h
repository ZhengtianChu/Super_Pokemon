#pragma once
#include "State.h"

class Scyzc3Engine;

class ChooseState :
    public State
{
public:
    void Handle(Context* c);
    ChooseState(Scyzc3Engine* pEngine);
    ~ChooseState();
    void virtSetupBackgroundBuffer();
    void virtMouseDown(int iButton, int iX, int iY);
    int virtInitialiseObjects();
    void virtDrawStringsOnTop() override;
    void virtKeyDown(int ikeyDown);
    void virtMainLoopDoBeforeUpdate();
    void copyAllBackgroundBuffer();

private:
    Scyzc3Engine* m_engine;
    SimpleImage image;
    SimpleImage mapImage;
    SimpleImage pImage1;
    SimpleImage pImage2;
    int flag1 = 0, flag2 = 0, flag3 = 1, flag4 = 0;
};

