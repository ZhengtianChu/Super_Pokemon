#pragma once
#include "Scyzc3Object.h"
class MovingPokemon2 :
    public Scyzc3Object
{
public:
    MovingPokemon2(Scyzc3Engine* pEngine, RunningState* state, Scyzc3Object* player, string image, int posX, int posY);
    ~MovingPokemon2() { cout << "fly delete!" << endl; box.swap(box); };

    void virtDraw();
    void virtDoUpdate(int iCurrentTime);

    void setPosition(double dX, double dY);
    void setDirection(int n) { m_direction = n; };
    int getdirection() { return m_direction; };
    int getClassId() { return 9; };
    int getHP() { return hp; };
    void setHP(int newHp) { hp = newHp; };
    void findWayToPlayer(int& x, int& y);
    void setPlayer(Scyzc3Object* p) { player = p; };
    void drawHP() override;
    void setSpeed(int s) { speed = s; };
    int getSpeed() { return speed; };
    bool isCollision();
    void setMaxHp(int hp) { maxHp = hp; };
    int getMaxHp() { return maxHp; };
    void setFly(bool f) { fly = false; };

private:
    double m_dX, m_dY, m_dSX, m_dSY;
    string image;
    int m_direction, hp;
    int drawX = 0, drawY = 0;
    int drawHeight = 50;
    int drawWidth = 50;
    Scyzc3Object* player;
    int maxHp = 100;
    int speed = 1;
    bool fly = false;
    int direction = 0;
    bool move = false;
    int currentTileX;
    int currentTileY;

    int dFlag1 = 1, dFlag2 = 1, dFlag3 = 1, dFlag4 = 1;

    int preDir = -1;
};

