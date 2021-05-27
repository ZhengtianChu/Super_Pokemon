#pragma once
#include "DisplayableObject.h"
#include "Scyzc3Engine.h"
#include "UtilMovementPosition.h"
#include "Scyzc3TileManager.h"
#include "RunningState.h"

class Scyzc3Object :
    public DisplayableObject
{
public:
    Scyzc3Object(Scyzc3Engine* pEngine, RunningState* state, string path);
    ~Scyzc3Object();

    bool check_collision(std::vector<SDL_Rect>& A, std::vector<SDL_Rect>& B);

    void virtDraw() {};
    void virtDoUpdate(int iCurrentTime) {};
    void setPosition(int dX, int dY) { m_iCurrentScreenX = dX; m_iCurrentScreenY = dY; };
    virtual void setDirection(int n) { m_direction = n; };
    virtual int getdirection() { return m_direction; };
    virtual int getPositionX() { return m_iCurrentScreenX; };
    virtual int getPositionY() { return m_iCurrentScreenY; };
    virtual bool isCollision();
    virtual bool detectBound();
    virtual int getHP() { return hp; };
    virtual void setHP(int newHp) { hp = newHp; };
    virtual int getClassId() { return 0; };
    double getSpeedX() { return m_dX; };
    double getSpeedY() { return m_dY; };
    void setSpeed(double x, double y) { m_dX = x; m_dY = y; };
    int getAttack() { return attack; };
    void setAttack(int a) { attack = a; };
    virtual void drawHP();
    virtual int getScore() { return score; };
    virtual void setScore(int s) { score = s; };
    std::vector<SDL_Rect> getBox() { return box; };
    void savePixel(SimpleImage img, int x, int y, int w, int h);

protected:
    Scyzc3TileManager* tm;
    RunningState* state;
    Scyzc3Engine* m_engine;
    int attack = 3;
    int drawWidth = 50, drawHeight = 50;
    int flag1 = 1;
    int flag2 = 1;
    int flag3 = 1;
    int flag4 = 1;
    int m_iUpdateTime = 0;
    std::vector<SDL_Rect> box;
    int tempTime = 0;
    SimpleImage displayImage;

private:
    double m_dX;
    double m_dY;
    double m_dSX;
    double m_dSY;
    string image;
    int hp;
    int maxHp = 100;
    int m_direction;
    int score = 0;

   
};

class player : public Scyzc3Object
{
public:
    player(Scyzc3Engine* pEngine, RunningState* state, string path, string name, int posX, int posY);
    ~player();

    void virtDraw();
    void virtDoUpdate(int iCurrentTime);
    void setPosition(double dX, double dY);
    void setDirection(int n) { m_direction = n; };
    int getdirection() { return m_direction; };
    int getClassId() { return 1; };
    int getHP() { return hp; };
    void setHP(int newHp) { hp = newHp; };
    int getScore() { return score; };
    void setScore(int s) { score = s; };
    void goNextMap();
    std::vector<SDL_Rect> getBox() { return box; };
private:
    string image;
    string name;
    int m_direction;
    int hp;
    int drawWidth = 50, drawHeight = 50;
    int score = 0;
    SimpleImage displayImage;
    std::vector<SDL_Rect> box;
};

class rival : public Scyzc3Object
{
public:
    void goNextMap();
    rival(Scyzc3Engine* pEngine, RunningState* state, string path, string name, int posX, int posY);
    ~rival() {
        cout << "player2 delete!" << endl;
        box.clear();
    };
    void virtDraw();
    void virtDoUpdate(int iCurrentTime);
    void setPosition(double dX, double dY);
    void setDirection(int n) { m_direction = n; };
    int getdirection() { return m_direction; };
    int getClassId() { return 2; };
    int getHP() { return hp; };
    void setHP(int newHp) { hp = newHp; };
    int getScore() { return score; };
    void setScore(int s) { score = s; };
    std::vector<SDL_Rect> getBox() { return box; };
private:
    string image;
    string name;
    int m_direction;
    int hp;
    int drawWidth = 50, drawHeight = 50;
    int score = 0;
    SimpleImage displayImage;
    std::vector<SDL_Rect> box;
};

