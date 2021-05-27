#include "MovingPokemon.h"
#include <ctime>
#include <iostream>
#include "UtilCollisionDetection.h"
using namespace std;


MovingPokemon::MovingPokemon(Scyzc3Engine* pEngine, RunningState* state, Scyzc3Object* p, string path, int posX, int posY)
    :Scyzc3Object(pEngine, state, path),
    m_dSX(0),
    m_dSY(0),
    m_dX(1),
    m_dY(1),
    m_direction(3),
    player(p)
{
    image = path;
    tm = state->GetTileManager();
    m_iCurrentScreenX = posX;
    m_iCurrentScreenY = posY;
    currentTileX = tm->getMapXForScreenX(m_iCurrentScreenX);
    currentTileY = tm->getMapYForScreenY(m_iCurrentScreenY);
    displayImage = ImageManager::loadImage(image, true);
}

MovingPokemon::~MovingPokemon()
{
    cout << "delete Gengar" << endl;
}

void MovingPokemon::virtDraw()
{
    if (isVisible()) {
       
        if (hp > 0)
        {
            int x = 0, y = 0;
            if (m_direction == 0)
                x = 0, y = 0;
            else if (m_direction == 1)
                x = 0, y = 150;
            else if (m_direction == 2)
                x = 50, y = 150;
            else if (m_direction == 3)
                x = 100, y = 150;
            else if (m_direction == 4)
                x = 150, y = 150;
            else if (m_direction == 5)
                x = 0, y = 0;
            else if (m_direction == 6)
                x = 50, y = 0;
            else if (m_direction == 7)
                x = 100, y = 0;
            else if (m_direction == 8)
                x = 150, y = 0;
            else if (m_direction == 9)
                x = 0, y = 50;
            else if (m_direction == 10)
                x = 50, y = 50;
            else if (m_direction == 11)
                x = 100, y = 50;
            else if (m_direction == 12)
                x = 150, y = 50;
            else if (m_direction == 13)
                x = 0, y = 100;
            else if (m_direction == 14)
                x = 50, y = 100;
            else if (m_direction == 15)
                x = 100, y = 100;
            else if (m_direction == 16)
                x = 150, y = 100;

            displayImage.renderImageWithMask(m_engine->getForegroundSurface(), x, y, m_iCurrentScreenX, m_iCurrentScreenY, drawWidth, drawHeight);
            savePixel(displayImage, 0, 0, 50, 50);

            drawHP();
        }
        else {
            displayImage = ImageManager::loadImage("images/gengarDead.png", true);
            displayImage.renderImageWithMask(m_engine->getForegroundSurface(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, displayImage.getWidth(), displayImage.getHeight());
        }

   

   
    }
}

void MovingPokemon::virtDoUpdate(int iCurrentTime)
{
    if (!m_engine->isPaused() && hp > 0)
    {
        GetPath();
        isCollision();
    }

}

void MovingPokemon::GetPath()
{
    if (!move)
    {
        if (m_engine->getLevel() == 0)
            findGreedyWay(m_iCurrentScreenX, m_iCurrentScreenY);
        else  findAStarWay();
        
        move = true;
    }
    else
    {
        if (direction == 1)
        {
            int t = currentTileY - 1;
            if (m_iCurrentScreenY > t * 26 + 1)
            {
                m_iCurrentScreenY -= speed;
                dFlag1++;
                move = true;
            }
            else
            {
                move = false;
                currentTileY = tm->getMapYForScreenY(m_iCurrentScreenY);
            }
        }
        if (direction == 2)
        {
            int t = currentTileY + 1;
            if (m_iCurrentScreenY < (t + 1) * 26 - 1)
            {
                m_iCurrentScreenY += speed;
                dFlag2++;
                move = true;
            }
            else
            {
                move = false;
                currentTileY = tm->getMapYForScreenY(m_iCurrentScreenY);
            }

        }
        if (direction == 3)
        {
            int t = currentTileX - 1;
            if (m_iCurrentScreenX > t * 26 + 1)
            {
                m_iCurrentScreenX -= speed;
                dFlag3++;
                move = true;
            }
            else {
                move = false;
                currentTileX = tm->getMapXForScreenX(m_iCurrentScreenX);
            }

        }
        if (direction == 4)
        {
            int t = currentTileX + 1;
            if (m_iCurrentScreenX < (t + 1) * 26 - 1)
            {
                m_iCurrentScreenX += speed;
                dFlag4++;
                move = true;
            }
            else
            {
                move = false;
                currentTileX = tm->getMapXForScreenX(m_iCurrentScreenX);
            }
        }
    }
}

void MovingPokemon::setPosition(double dX, double dY)
{
    m_iCurrentScreenX = (int)dX;
    m_iCurrentScreenY = (int)dY;
}

//bool MovingPokemon::detectBound()
//{
//    int left = state->getTileValue(m_iCurrentScreenX, m_iCurrentScreenY + drawHeight / 2);
//    int right = state->getTileValue(m_iCurrentScreenX + drawWidth + 1, m_iCurrentScreenY + drawHeight / 2);
//    int up = state->getTileValue(m_iCurrentScreenX + drawWidth / 2, m_iCurrentScreenY);
//    int down = state->getTileValue(m_iCurrentScreenX + drawWidth / 2, m_iCurrentScreenY + drawHeight + 1);
//
//
//    if (left <= 2) {
//        m_iCurrentScreenX += 2;
//        return true;
//    }
//    if (right <= 2) { 
//        m_iCurrentScreenX -= 2;
//        return true; 
//    }
//    if (up <= 2) {
//        m_iCurrentScreenY += 2;
//        return true;
//    }
//    if (down <= 2) {
//        m_iCurrentScreenY -= 2;
//        return true;
//    }
//    return false;
//}

void MovingPokemon::findGreedyWay(int& x, int& y)
{
    float distance[4], distance2[4];
    int tileX = tm->getMapXForScreenX(x + drawWidth / 2);
    int tileY = tm->getMapYForScreenY(y + drawHeight / 2);
    int playerX = tm->getMapXForScreenX(player->getPositionX() + 25);
    int playerY = tm->getMapXForScreenX(player->getPositionY() + 25);

    if (tileY > 0)  distance[0] = state->GetDistance(tileX, tileY - 1, playerX, playerY);
    if (tileY < 49) distance[1] = state->GetDistance(tileX, tileY + 1, playerX, playerY);
    if (tileX > 0) distance[2] = state->GetDistance(tileX - 1, tileY, playerX, playerY);
    if (tileY < 49) distance[3] = state->GetDistance(tileX + 1, tileY, playerX, playerY);

    memcpy(distance2, distance, sizeof(float) * 4);
    sort(distance2, distance2 + 4);

    float minValue = 100000;

    for (int i = 0; i < 4; i++)
    {
        minValue = distance2[i];
        if (minValue == distance[0] 
            && tm->getMapValue(tm->getMapXForScreenX(x), tm->getMapYForScreenY(y) - 1) > 2
            && tm->getMapValue(tm->getMapXForScreenX(x + drawWidth / 2), tm->getMapYForScreenY(y) - 1) > 2
            && tm->getMapValue(tm->getMapXForScreenX(x + drawWidth), tm->getMapYForScreenY(y) - 1) > 2)
            {
            if (preDir == 1) continue;
            preDir = 0;
           
            direction = 1;
    
            if (dFlag1 % 100 < 25) m_direction = 1;
            else if (dFlag1 % 100 < 50) m_direction = 2;
            else if (dFlag1 % 100 < 75) m_direction = 3;
            else if (dFlag1 % 100 < 100) m_direction = 4;

            //cout << "up" << endl;
            return;
        }

        if (minValue == distance[1]
            && tm->getMapValue(tm->getMapXForScreenX(x), tm->getMapYForScreenY(y + drawHeight) + 1) > 2
            && tm->getMapValue(tm->getMapXForScreenX(x + drawWidth / 2), tm->getMapYForScreenY(y + drawHeight) + 1) > 2
            && tm->getMapValue(tm->getMapXForScreenX(x + drawWidth), tm->getMapYForScreenY(y + drawHeight) + 1) > 2) {

            if (preDir == 0) continue;
            preDir = 1;

            direction = 2;
       
            if (dFlag2 % 100 < 25) m_direction = 5;
            else if (dFlag2 % 100 < 50) m_direction = 6;
            else if (dFlag2 % 100 < 75) m_direction = 7;
            else if (dFlag2 % 100 < 100) m_direction = 8;

            //cout << "down" << endl;
            return;
        }
        if (minValue == distance[2] 
            && tm->getMapValue(tm->getMapXForScreenX(x) - 1, tm->getMapYForScreenY(y)) > 2
            && tm->getMapValue(tm->getMapXForScreenX(x) - 1, tm->getMapYForScreenY(y + drawHeight / 2)) > 2
            && tm->getMapValue(tm->getMapXForScreenX(x) - 1, tm->getMapYForScreenY(y + drawHeight)) > 2) {
            if (preDir == 3) continue;
            preDir = 2;

            if (dFlag3 % 100 < 25) m_direction = 9;
            else if (dFlag3 % 100 < 50) m_direction = 10;
            else if (dFlag3 % 100 < 75) m_direction = 11;
            else if (dFlag3 % 100 < 100) m_direction = 12;

            direction = 3;

            //cout << "left" << endl;
            return;
        }
        if (minValue == distance[3] 
            && tm->getMapValue(tm->getMapXForScreenX(x + drawWidth) + 1, tm->getMapYForScreenY(y)) > 2
            && tm->getMapValue(tm->getMapXForScreenX(x + drawWidth) + 1, tm->getMapYForScreenY(y + drawHeight / 2)) > 2
            && tm->getMapValue(tm->getMapXForScreenX(x + drawWidth) + 1, tm->getMapYForScreenY(y + drawHeight)) > 2) {
            if (preDir == 2) continue;
            preDir = 3;

            direction = 4;
       
            if (dFlag4 % 100 < 25) m_direction = 13;
            else if (dFlag4 % 100 < 50) m_direction = 14;
            else if (dFlag4 % 100 < 75) m_direction = 15;
            else if (dFlag4 % 100 < 100) m_direction = 16;

            //cout << "right" << endl;
            return;
        }
           
    }
    
}

shared_ptr<Point> MovingPokemon::getLeastFpoint()
{
    if (!openList.empty())
    {
        auto resPoint = openList.front();
        for (auto& point : openList)
            if (point->F < resPoint->F)
                resPoint = point;
        return resPoint;
    }
    return NULL;
}

vector<shared_ptr<Point>> MovingPokemon::getSurroundPoints(const shared_ptr<Point> point) const
{
    vector<shared_ptr<Point>> surroundPoints;
    int x = point->x;
    int y = point->y;
    if (tm->getMapValue(x - 1, y) > 2)   surroundPoints.push_back(make_shared<Point>(x - 1, y));
    if (tm->getMapValue(x + 1, y) > 2)   surroundPoints.push_back(make_shared<Point>(x + 1, y));
    if (tm->getMapValue(x, y - 1) > 2)   surroundPoints.push_back(make_shared<Point>(x, y - 1));
    if (tm->getMapValue(x, y + 1) > 2)   surroundPoints.push_back(make_shared<Point>(x, y + 1));
    return surroundPoints;
}

shared_ptr<Point> MovingPokemon::isInList(const std::list<shared_ptr<Point>>& list, const shared_ptr<Point> point) const
{
    for (auto p : list)
        if (p->x == point->x && p->y == point->y)
            return p;
    return NULL;
}

int MovingPokemon::calcG(shared_ptr<Point> temp_start, shared_ptr<Point> point)
{
    int parentG = point->parent == NULL ? 0 : point->parent->G;
    return parentG + 1;
}

int MovingPokemon::calcH(shared_ptr<Point> point, shared_ptr<Point> end)
{
    return (int)sqrt((double)(end->x - point->x) * (double)(end->x - point->x) + (double)(end->y - point->y) * (double)(end->y - point->y));
}

shared_ptr<Point> MovingPokemon::findWayToPlayer(int& x, int& y)
{
    int tileX = tm->getMapXForScreenX(x + drawWidth / 2);
    int tileY = tm->getMapYForScreenY(y + drawHeight / 2);
    int playerX = tm->getMapXForScreenX(player->getPositionX() + 25);
    int playerY = tm->getMapXForScreenX(player->getPositionY() + 25);    

    openList.push_back(make_shared<Point>(tileX, tileY));
    shared_ptr<Point> endPoint(new Point(playerX, playerY));

    while (!openList.empty())
    {
        auto curPoint = getLeastFpoint();
        openList.remove(curPoint);
        closeList.push_back(curPoint);
        
        auto surroundPoints = getSurroundPoints(curPoint);

        for (auto& target : surroundPoints)
        {
            if (!isInList(openList, target))
            {
                target->parent = curPoint;
                target->G = calcG(curPoint, target);
                target->H = calcH(target, endPoint);
                target->F = target->G + target->H;

                openList.push_back(target);
            }
            else
            {
                int tempG = calcG(curPoint, target);
                if (tempG < target->G)
                {
                    target->parent = curPoint;
                    target->G = tempG;
                    target->F = target->G + target->H;
                }
            }
        
            shared_ptr<Point> resPoint = isInList(openList, endPoint);
            if (resPoint)
            {
                return resPoint;
            }
        }
    }
    return NULL;
}

void MovingPokemon::findAStarWay()
{
    shared_ptr<Point> result = findWayToPlayer(m_iCurrentScreenX, m_iCurrentScreenY);
    std::list<shared_ptr<Point>> path;
    while (result)
    {
        path.push_front(result);
        result = result->parent;
    }

    openList.clear();
    closeList.clear();

    list<shared_ptr<Point>>::iterator iter = path.begin();
    iter++;

    int nextX = (*iter)->x;
    int nextY = (*iter)->y;

    int tileX = tm->getMapXForScreenX(m_iCurrentScreenX + drawWidth / 2);
    int tileY = tm->getMapYForScreenY(m_iCurrentScreenY + drawHeight / 2);


    if (nextY == tileY - 1)
    {
        direction = 1;
        if (dFlag1 % 100 < 25) m_direction = 1;
        else if (dFlag1 % 100 < 50) m_direction = 2;
        else if (dFlag1 % 100 < 75) m_direction = 3;
        else if (dFlag1 % 100 < 100) m_direction = 4;
    }
    if (nextY == tileY + 1)
    {
        direction = 2;
        if (dFlag2 % 100 < 25) m_direction = 5;
        else if (dFlag2 % 100 < 50) m_direction = 6;
        else if (dFlag2 % 100 < 75) m_direction = 7;
        else if (dFlag2 % 100 < 100) m_direction = 8;
    }
    if (nextX == tileX - 1)
    {
        direction = 3;
        if (dFlag3 % 100 < 25) m_direction = 9;
        else if (dFlag3 % 100 < 50) m_direction = 10;
        else if (dFlag3 % 100 < 75) m_direction = 11;
        else if (dFlag3 % 100 < 100) m_direction = 12;
    }
    if (nextX == tileX + 1)
    {
        direction = 4;
        if (dFlag4 % 100 < 25) m_direction = 13;
        else if (dFlag4 % 100 < 50) m_direction = 14;
        else if (dFlag4 % 100 < 75) m_direction = 15;
        else if (dFlag4 % 100 < 100) m_direction = 16;
    }

}



void MovingPokemon::drawHP()
{
    double curHealth = (double)getHP() / (double)maxHp;
    m_engine->drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY - 2, m_iCurrentScreenX + 50, m_iCurrentScreenY + 5, 0xFFFFFF);
    if (curHealth > 1)    m_engine->drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY - 2, m_iCurrentScreenX + 50, m_iCurrentScreenY + 5, 0xFF0000);
    else if (curHealth > 0)    m_engine->drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY - 2, m_iCurrentScreenX + (int)(curHealth * 50.0), m_iCurrentScreenY + 5, 0xFF0000);
}

bool MovingPokemon::isCollision()
{
    Scyzc3Object* pObject;
    for (int iObjectId = 0;
        (pObject = (Scyzc3Object*)m_engine->getDisplayableObject(iObjectId)
            ) != NULL;
        iObjectId++)
    {
        if (pObject->getClassId() == 1 && check_collision(box, pObject->getBox())) {
            pObject->setHP(pObject->getHP() - 1);
            return true;
        }
        if (pObject->getClassId() == 2 && check_collision(box, pObject->getBox())) {
            pObject->setHP(pObject->getHP() - 1);
            return true;
        }

    }
    return false;
}