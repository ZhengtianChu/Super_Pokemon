#pragma once
#include "State.h"


class Scyzc3Engine;

class HelpState :
    public State
{
public:
    void Handle(Context* c);
    HelpState(Scyzc3Engine* pEngine);
    ~HelpState();
    void virtSetupBackgroundBuffer();
    void virtMouseDown(int iButton, int iX, int iY);
    int virtInitialiseObjects();
    void virtDrawStringsOnTop();
    void virtKeyDown(int ikeyDown);

    void copyAllBackgroundBuffer();

private:
    Scyzc3Engine* m_engine;
    SimpleImage image;

};

