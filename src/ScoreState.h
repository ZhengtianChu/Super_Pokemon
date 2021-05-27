#pragma once
#include "State.h"
#include "Scyzc3FilterPoints.h"

using namespace std;

class Scyzc3Engine;

class ScoreState :
    public State
{
public:
    void Handle(Context* c);
    ScoreState(Scyzc3Engine* pEngine);
    ~ScoreState();
    void virtSetupBackgroundBuffer();
    void virtMouseDown(int iButton, int iX, int iY);
    int virtInitialiseObjects() { return 0; };
    void virtMouseWheel(int x, int y, int which, int timestamp);
    void virtDrawStringsOnTop();
    void virtKeyDown(int ikeyDown);
    void readScore();
    void virtMainLoopDoBeforeUpdate() override;
    void copyAllBackgroundBuffer();
    void virtMouseMoved(int iX, int iY);

private:
    Scyzc3Engine* m_engine;
    SimpleImage image;
    string nameList[5] = { "NULL","NULL" ,"NULL" ,"NULL" ,"NULL" };
    int scoreList[5] = { 0,0,0,0,0 };
    int m_iOffset;
    Scyzc3FilterPoints m_filterScaling;
    Scyzc3FilterPointsTranslation m_filterTranslation;

};

