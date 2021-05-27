#pragma once
#include "header.h"
#include "BaseEngine.h"
#include "Scyzc3TileManager.h"
#include "SDL_mixer.h"
#include "SDL.h"
#include "Bgm.h"

class State;

class Scyzc3Engine :
    public BaseEngine
{
public:
    Scyzc3Engine();
    ~Scyzc3Engine();
    void virtSetupBackgroundBuffer() override;
    void virtMouseDown(int iButton, int iX, int iY) override;
    int virtInitialiseObjects() override;
    void virtDrawStringsOnTop() override;
    void StateSwitch(int stateID);
    void virtKeyDown(int iKeyCode) override;
    void setMapId(int id) { mapId = id; };
    int getMapId() { return mapId; };
    int getLoad() { return mLoad; };
    void setLoad(int load) { mLoad = load; };
    void setRunTime(double time) { runTime = time; };
    double getRunTime() { return runTime; };
    void virtMainLoopDoBeforeUpdate() override;
    void copyAllBackgroundBuffer() override;
    int getWinId() { return winPlayerId; };
    void setWinId(int id) { winPlayerId = id; };
    int getWinScore() { return winScore; };
    void setWinScore(int s) { winScore = s; }
    void virtMouseWheel(int x, int y, int which, int timestamp) override;
    void virtMouseMoved(int iX, int iY);
    void setName1(std::string s) { name1 = s; };
    std::string getName1() { return name1; };
    void setName2(std::string s) { name2 = s; };
    std::string getName2() { return name2; };
    int getLevel() { return level; };
    void setLevel(int l) { level = l; };
    //void playMusic();
    Bgm* getBgm() { return bgm; };

private:
    State* currentState;
    State* states[8];
    int mapId = 0;
    int mLoad = 0;
    double runTime = 0;
    int m_iOffset = 0;
    int winPlayerId = 0;
    int winScore = 0;
    std::string name1 = "pikaqu";
    std::string name2 = "Charmander";
    int level = 0;
    Bgm* bgm;
};

