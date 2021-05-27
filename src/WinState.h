#pragma once
#include "State.h"

class Scyzc3Engine;

using namespace std;

class WinState :
    public State
{
public:
    void Handle(Context* c);
    WinState(Scyzc3Engine* pEngine);
    void virtSetupBackgroundBuffer();
    void virtMouseDown(int iButton, int iX, int iY) {};
    int virtInitialiseObjects() { return 0; };
    void virtDrawStringsOnTop();
    void virtKeyDown(int ikeyDown);
    void writeScore();
    void copyAllBackgroundBuffer();

private:
    Scyzc3Engine* m_engine;
    SimpleImage image;
    string username;
};