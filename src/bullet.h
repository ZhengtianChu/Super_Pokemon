#include "Scyzc3Object.h"
class bullet :
    public Scyzc3Object
{
public:
    bullet(Scyzc3Engine* pEngine, RunningState* state, const char* path, Scyzc3Object* host, int direction);
    ~bullet() { cout << "Bullet delete!" << endl; };
    bool detectBound();
    void virtDraw() override;
    void virtDoUpdate(int iCurrentTime);
    //bool isCollision(Scyzc3Object* obj);
    int getClassId() { return 3; };
    void setDirection(int d) { m_direction = d; };
    //bool checkFlash(int x11, int y11, int x12, int y12, int x13, int y13, int x14, int y14, int x21, int y21, int x22, int y22);
    //bool checkFlame(int x11, int y11, int x12, int y12, int x13, int y13, int x14, int y14, int x15, int y15, int x21, int y21, int x22, int y22);
    void setObjectID(int id) { objectID = id; };
    int getObjectID() { return objectID; };

private:
    int m_dX;
    int m_dY;
    double m_dSX;
    Scyzc3Object* hostPlayer;
    double m_dSY;
    const char* image;
    int m_direction;
    bool m_bVisible = false;
    int objectID = 0;
    SimpleImage bulletImage;
    
};
