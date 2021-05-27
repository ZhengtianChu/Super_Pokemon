#pragma once
#include "State.h"
#include "Scyzc3TileManager.h"
#include <iostream>
#include "Bgm.h"

using namespace std;
class player;
class rival;
class MovingPokemon;
class MovingPokemon2;
class State;
class bullet;
class Scyzc3Engine;
class Scyzc3Object;

class RunningState :
    public State
{
public:
    void Handle(Context* c);
    RunningState(Scyzc3Engine* pEngine);
    ~RunningState();
    void virtSetupBackgroundBuffer();
    void virtMouseDown(int iButton, int iX, int iY);
    int virtInitialiseObjects();
    void virtDrawStringsOnTop();
    void loadMap(char* filename);
    void saveGame(char* filename, int reset);
    vector<string> GetMapData();
    vector<string> GetCurrentView();
    void UpdateView();
    Scyzc3TileManager* GetTileManager() { return &tm; };
    int GetTileHeight() { return tileHeight; };
    int GetTileWidth() { return tileWidth; };
    int getTileValue(int x, int y);
    void virtKeyDown(int ikeyDown) override;
    float GetDistance(int x1, int y1, int x2, int y2);
    void copyAllBackgroundBuffer();

    friend ofstream& operator<<(ofstream& output, player*& obj);
    friend ofstream& operator<<(ofstream& output, rival*& obj);
    friend ofstream& operator<<(ofstream& output, MovingPokemon*& obj);
    friend ofstream& operator<<(ofstream& output, MovingPokemon2*& obj);


protected:
    player* player1;
    rival* player2;
    MovingPokemon* pokemon;
    MovingPokemon2* pokemon2;
    bullet* firstBullet;
    bullet* secondBullet;
    bullet* thirdBullet;
    bullet* firstBullet2;
    bullet* secondBullet2;
    bullet* thirdBullet2;
    string name1;
    string name2;


private:
    Scyzc3Engine* m_engine;
    Scyzc3TileManager tm;
    vector<string> currentView;
    vector<string> mapData;
    int tileHeight = 0;
    int tileWidth = 0;
    int hp1 = 100, hp2 = 100, hp3 = 300, hp4 = 300;
    int s1 = 0, s2 = 0;
    int p1X = 100, p1Y = 200, p2X = 1000, p2Y = 200, p3X = 1000, p3Y = 500, p4X = 500, p4Y = 600;
    int flag = 0;
    int flagg = 0;
    Bgm* bgm;
    SimpleImage display;
    SimpleImage profile1;
    SimpleImage profile2;
    SimpleImage help;
    SimpleImage pause;
    SimpleImage pause2;
    SimpleImage save;
    SimpleImage quit;
    SimpleImage volume;

};
