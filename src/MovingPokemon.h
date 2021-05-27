#pragma once
#include "Scyzc3Object.h"
#include <vector>
#include <list>

struct Point;

class MovingPokemon :
    public Scyzc3Object
{
public:
    MovingPokemon(Scyzc3Engine* pEngine, RunningState* state, Scyzc3Object* player, string image, int posX, int posY);
    ~MovingPokemon();

    void virtDraw();
    void virtDoUpdate(int iCurrentTime);

    void GetPath();

    void setPosition(double dX, double dY);
    //bool detectBound();
    void setDirection(int n) { m_direction = n; };
    int getdirection() { return m_direction; };
    int getClassId() { return 9; };
    int getHP() { return hp; };
    void setHP(int newHp) { hp = newHp; };
    void findGreedyWay(int& x, int& y);
    shared_ptr<Point> getLeastFpoint();
    vector<shared_ptr<Point>> getSurroundPoints(const shared_ptr<Point> point) const;
    shared_ptr<Point> MovingPokemon::isInList(const std::list<shared_ptr<Point>>& list, const shared_ptr<Point> point) const;
    int calcG(shared_ptr<Point> temp_start, shared_ptr<Point> point);
    int calcH(shared_ptr<Point> point, shared_ptr<Point> end);
    shared_ptr<Point> findWayToPlayer(int& x, int& y);
    void findAStarWay();

    void setPlayer(Scyzc3Object* p) { player = p; };
    void drawHP() override;
    void setSpeed(int s) { speed = s; };
    int getSpeed() { return speed; };
    bool isCollision();
    void setMaxHp(int hp) { maxHp = hp; };
    int getMaxHp(){ return maxHp; };
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
    std::list<shared_ptr<Point>> openList; 
    std::list<shared_ptr<Point>> closeList;
    SimpleImage displayImage;
};

struct Point
{
    int x, y;
    int F, G, H; 
    shared_ptr<Point> parent; 
    Point(int _x, int _y) :x(_x), y(_y), F(0), G(0), H(0), parent(NULL) {}
};
