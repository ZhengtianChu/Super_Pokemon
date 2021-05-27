
#include "bullet.h"
#include <iostream>
#include "UtilCollisionDetection.h"
using namespace std;


bullet::bullet(Scyzc3Engine* pEngine, RunningState* state, const char* path, Scyzc3Object* host, int direction)
    :Scyzc3Object(pEngine, state, path),
    m_dSX(0),
    m_dSY(0),
    m_dX(0),
    m_dY(0),
    hostPlayer(host),
    m_direction(direction)
{
    image = path;
    m_iCurrentScreenX = host->getPositionX() + 12;
    m_iCurrentScreenY = host->getPositionY() + 12;
    bulletImage = ImageManager::loadImage(image, true);

    for (int i = 0; i < bulletImage.getHeight(); i++)
    {
        int w = 0, start = 0, end = 0;
        SDL_Rect dot;
        dot.y = i;
        for (int j = 0; j < bulletImage.getWidth(); j++)
        {
            if (bulletImage.getPixelColour(j, i) > 0)
            {
                if (start == 0)
                {
                    start = end = j;
                    w = 1;
                }
                else if (end == j - 1)
                {
                    w++;
                    end = j;
                }
                else
                {
                    start = j = end;
                }
            }
            else
            {
                if (w > 0) {
                    dot.x = start;
                    dot.w = w;
                    dot.h = 1;
                    box.push_back(dot);
                }
                start = end = w = 0;
            }
        }
    }
        
        
}

bool bullet::detectBound()
{
    int left = state->getTileValue(m_iCurrentScreenX, m_iCurrentScreenY + 13);
    int right = state->getTileValue(m_iCurrentScreenX + 26, m_iCurrentScreenY + 13);
    int up = state->getTileValue(m_iCurrentScreenX + 13, m_iCurrentScreenY);
    int down = state->getTileValue(m_iCurrentScreenX + 13, m_iCurrentScreenY + 26);


    if (left <= 2 || right <= 2 || up <= 2 || down <= 2) {
        if (hostPlayer->getClassId() == 1)  setVisible(false);
        else  m_engine->storeObjectInArray(objectID, NULL);
    }
    return false;
}

void bullet::virtDraw()
{
    if (isVisible()) {
    bulletImage.renderImageWithMask(m_engine->getForegroundSurface(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, bulletImage.getWidth(), bulletImage.getHeight());
    savePixel(bulletImage, 0, 0, bulletImage.getWidth(), bulletImage.getHeight());
    detectBound();
    }
}

void bullet::virtDoUpdate(int iCurrentTime)
{
    if (isVisible()) 
    {
        Scyzc3Object* pObject;
        Scyzc3Object* obj;
        int tempId = 0;

        if (m_direction == 0 || (m_direction >= 5 && m_direction <= 8)) {
            m_dX = 0;
            m_dY = 5;
        }
        else if (m_direction >= 1 && m_direction <= 4) {
            m_dX = 0;
            m_dY = -5;
        }
        else if (m_direction >= 9 && m_direction <= 12) {
            m_dX = -5;
            m_dY = 0;
        }
        else if (m_direction >= 13 && m_direction <= 16) {
            m_dX = 5;
            m_dY = 0;
        }
        else {
            m_dX = 0;
            m_dY = 0;
        }

        if (hostPlayer->getClassId() == 1) {
            for (int iObjectId = 0;
                (pObject = (Scyzc3Object*)m_engine->getDisplayableObject(iObjectId)
                    ) != NULL;
                iObjectId++)
            {
               
                if (pObject->getClassId() == 2 && check_collision(box, pObject->getBox())) {

                    m_iCurrentScreenX = hostPlayer->getPositionX() + 12;
                    m_iCurrentScreenY = hostPlayer->getPositionY() + 12;
       
                    if (isVisible())
                    {
                        pObject->setHP(pObject->getHP() - hostPlayer->getAttack());
                        hostPlayer->setScore(hostPlayer->getScore() + hostPlayer->getAttack() * 2);
                    }
                    this->setVisible(false);
                    
                }

                if (pObject->getClassId() == 9 && check_collision(box, pObject->getBox())) {

                    if (check_collision(box, pObject->getBox()))
                    {

                        m_iCurrentScreenX = hostPlayer->getPositionX() + 12;
                        m_iCurrentScreenY = hostPlayer->getPositionY() + 12;

                        if (isVisible())
                        {
                            pObject->setHP(pObject->getHP() - hostPlayer->getAttack());
                            hostPlayer->setScore(hostPlayer->getScore() + hostPlayer->getAttack() * 2);
                        }
                        this->setVisible(false);
                    }
                   
                    
                }
            }
        }

        if (hostPlayer->getClassId() == 2) {
            for (int iObjectId = 0;
                (pObject = (Scyzc3Object*)m_engine->getDisplayableObject(iObjectId)
                    ) != NULL;
                iObjectId++)
            {
                if (pObject->getClassId() == 1 && check_collision(box, pObject->getBox())) { 
                    m_iCurrentScreenX = hostPlayer->getPositionX() + 12;
                    m_iCurrentScreenY = hostPlayer->getPositionY() + 12;

                    if (isVisible())
                    {
                        pObject->setHP(pObject->getHP() - hostPlayer->getAttack());
                        hostPlayer->setScore(hostPlayer->getScore() + hostPlayer->getAttack() * 2);
                    }
                    //this->setVisible(false);  
                    m_engine->storeObjectInArray(objectID, NULL);
                    
                }
                if (pObject->getClassId() == 9 && check_collision(box, pObject->getBox())) {

                    m_iCurrentScreenX = hostPlayer->getPositionX() + 12;
                    m_iCurrentScreenY = hostPlayer->getPositionY() + 12;

              

                    if (isVisible())
                    {
                        pObject->setHP(pObject->getHP() - hostPlayer->getAttack());
                        hostPlayer->setScore(hostPlayer->getScore() + hostPlayer->getAttack() * 2);
                    }
                    m_engine->storeObjectInArray(objectID, NULL);
                }
            }

        }

        m_iCurrentScreenX += m_dX;
        m_iCurrentScreenY += m_dY;


        redrawDisplay();
    }
    
}

//bool bullet::isCollision(Scyzc3Object* obj)
//{
//    if (this->hostPlayer->getClassId() == 1)
//        return checkFlash(m_iCurrentScreenX + 13, m_iCurrentScreenY, m_iCurrentScreenX, m_iCurrentScreenY + 13, 
//            m_iCurrentScreenX + 26, m_iCurrentScreenY + 13, m_iCurrentScreenX + 13, m_iCurrentScreenY + 26, 
//            obj->getPositionX(), obj->getPositionY(), obj->getPositionX() + 50, obj->getPositionY() + 50);
//    else return checkFlame(m_iCurrentScreenX + 13, m_iCurrentScreenY, m_iCurrentScreenX, m_iCurrentScreenY + 18,
//        m_iCurrentScreenX + 9, m_iCurrentScreenY + 26, m_iCurrentScreenX + 18, m_iCurrentScreenY + 26,
//        m_iCurrentScreenX + 26, m_iCurrentScreenY + 18, obj->getPositionX(), obj->getPositionY(), obj->getPositionX() + 50, obj->getPositionY() + 50);
//}
//
//bool bullet::checkFlash(int x11, int y11, int x12, int y12, int x13, int y13, int x14, int y14, int x21, int y21, int x22, int y22)
//{
//    return !(x13 < x21 || y14 < y21 || x12 > x22 || y11 > y22);
//}
//
//bool bullet::checkFlame(int x11, int y11, int x12, int y12, int x13, int y13, int x14, int y14, int x15, int y15, int x21, int y21, int x22, int y22)
//{
//    return !(x15 < x21 || y13 < y21 || x12 > x22 || y11 > y22);
//}

