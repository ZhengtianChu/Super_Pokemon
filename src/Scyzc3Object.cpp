#include "Scyzc3Object.h"
#include "header.h"
#include "Scyzc3Engine.h"
#include "UtilCollisionDetection.h"
#include "math.h"

Scyzc3Object::Scyzc3Object(Scyzc3Engine* pEngine, RunningState* state, string path)
    :DisplayableObject(pEngine),
    state(state),
    m_engine(pEngine),
    m_dSX(0),
    m_dSY(0),
    m_dX(0),
    m_dY(0),
    image(path),
    m_direction(4)
{
    tm = state->GetTileManager();
    m_iCurrentScreenX = 300;
    m_iCurrentScreenY = 500;
    hp = 100;
}

Scyzc3Object::~Scyzc3Object()
{
  
}

bool Scyzc3Object::check_collision(std::vector<SDL_Rect>& A, std::vector<SDL_Rect>& B)
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    for (int Abox = 0; Abox < A.size(); Abox++)
    {
        leftA = A[Abox].x;
        rightA = A[Abox].x + A[Abox].w;
        topA = A[Abox].y;
        bottomA = A[Abox].y + A[Abox].h;

        for (int Bbox = 0; Bbox < B.size(); Bbox++)
        {
            leftB = B[Bbox].x;
            rightB = B[Bbox].x + B[Bbox].w;
            topB = B[Bbox].y;
            bottomB = B[Bbox].y + B[Bbox].h;

            if (((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB)) == false)
            {
                return true;
            }
        }
    }
    return false;
}

void player::setPosition(double dX, double dY)
{
    m_iCurrentScreenX = (int)dX;
    m_iCurrentScreenY = (int)dY;
}

void player::goNextMap()
{
    if (state->getTileValue(m_iCurrentScreenX + drawWidth / 2, m_iCurrentScreenY + drawHeight / 2) == 5)
    {
        tm = state->GetTileManager();
        for (int i = 0; i < state->GetTileWidth(); i++)
            for (int j = 0; j < state->GetTileHeight(); j++) {
                if (tm->getMapValue(i, j) == 6)
                    tm->setMapValue(i, j, 4);
                if (tm->getMapValue(i, j) == 5)
                    tm->setMapValue(i, j, 3);
            }

        tm->drawAllTiles(m_engine, m_engine->getBackgroundSurface());
    }
    else if (state->getTileValue(m_iCurrentScreenX + drawWidth / 2, m_iCurrentScreenY + drawHeight / 2) == 4)
    {

        if (m_engine->getMapId() == 0) {
            m_engine->setMapId(1);
        }
        else if (m_engine->getMapId() == 1) {
            m_engine->setMapId(2);
        }
        else if (m_engine->getMapId() == 2) {
            m_engine->setMapId(0);
        }
        state->saveGame("resources/saveInfo.txt", 1);
        m_engine->setLoad(4);

        m_engine->StateSwitch(1);
    }
   
    else if (state->getTileValue(m_iCurrentScreenX + drawWidth / 2, m_iCurrentScreenY + drawHeight / 2) == 7)
    {

        tm = state->GetTileManager();
        int posx = tm->getMapXForScreenX(m_iCurrentScreenX + drawWidth / 2);
        int posy = tm->getMapYForScreenY(m_iCurrentScreenY + drawHeight / 2);
        tm->setMapValue(posx, posy, 3);
            
        if (this->getHP() > 80) this->setHP(100);
        else this->setHP(this->getHP() + 20);


        tm->drawAllTiles(m_engine, m_engine->getBackgroundSurface());
    }

    else if (state->getTileValue(m_iCurrentScreenX + drawWidth / 2, m_iCurrentScreenY + drawHeight / 2) == 8)
    {

        tm = state->GetTileManager();
        int posx = tm->getMapXForScreenX(m_iCurrentScreenX + drawWidth / 2);
        int posy = tm->getMapYForScreenY(m_iCurrentScreenY + drawHeight / 2);

        tm->setMapValue(posx, posy, 3);
            

        this->setAttack(this->getAttack() + 3);


        tm->drawAllTiles(m_engine, m_engine->getBackgroundSurface());
    }
}

void rival::goNextMap()
{
    if (state->getTileValue(m_iCurrentScreenX + drawWidth / 2, m_iCurrentScreenY + drawHeight / 2) == 5)
    {
        tm = state->GetTileManager();
        for (int i = 0; i < state->GetTileWidth(); i++)
            for (int j = 0; j < state->GetTileHeight(); j++) {
                if (tm->getMapValue(i, j) == 6)
                    tm->setMapValue(i, j, 4);
                if (tm->getMapValue(i, j) == 5)
                    tm->setMapValue(i, j, 3);
            }

        tm->drawAllTiles(m_engine, m_engine->getBackgroundSurface());
    }
    else if (state->getTileValue(m_iCurrentScreenX + drawWidth / 2, m_iCurrentScreenY + drawHeight / 2) == 4)
    {

        if (m_engine->getMapId() == 0) {
            m_engine->setMapId(1);
        }
        else if (m_engine->getMapId() == 1) {
            m_engine->setMapId(2);
        }
        else if (m_engine->getMapId() == 2) {
            m_engine->setMapId(0);
        }
        state->saveGame("resources/saveInfo.txt", 1);
        m_engine->setLoad(4);

        m_engine->StateSwitch(1);
    }

    else if (state->getTileValue(m_iCurrentScreenX + drawWidth / 2, m_iCurrentScreenY + drawHeight / 2) == 7)
    {

        tm = state->GetTileManager();
        int posx = tm->getMapXForScreenX(m_iCurrentScreenX + drawWidth / 2);
        int posy = tm->getMapYForScreenY(m_iCurrentScreenY + drawHeight / 2);

        tm->setMapValue(posx, posy, 3);

        if (this->getHP() > 80) this->setHP(100);
        else this->setHP(this->getHP() + 20);


        tm->drawAllTiles(m_engine, m_engine->getBackgroundSurface());
    }

    else if (state->getTileValue(m_iCurrentScreenX + drawWidth / 2, m_iCurrentScreenY + drawHeight / 2) == 8)
    {

        tm = state->GetTileManager();
        int posx = tm->getMapXForScreenX(m_iCurrentScreenX + drawWidth / 2);
        int posy = tm->getMapYForScreenY(m_iCurrentScreenY + drawHeight / 2);

        tm->setMapValue(posx, posy, 3);

        this->setAttack(this->getAttack() + 3);


        tm->drawAllTiles(m_engine, m_engine->getBackgroundSurface());
    }
}


bool Scyzc3Object::isCollision()
{
    DisplayableObject* pObject;
    for (int iObjectId = 0;
        (pObject = m_engine->getDisplayableObject(iObjectId)
            ) != NULL;
        iObjectId++)
    {
        if (pObject == this)
            continue;
        if (pObject == nullptr)
            continue;
        if (CollisionDetection::checkRectangles(pObject->getXCentre(), pObject->getXCentre() + 40,
            pObject->getYCentre(), pObject->getYCentre() + 30,
            m_iCurrentScreenX, m_iCurrentScreenX + 40, m_iCurrentScreenY, m_iCurrentScreenY + 30)) {
            return true;
        }
        else
            return false;
    }
    return false;
}

bool Scyzc3Object::detectBound()
{
    int left = state->getTileValue(m_iCurrentScreenX, m_iCurrentScreenY + drawHeight / 2);
    int right = state->getTileValue(m_iCurrentScreenX + drawWidth + 1, m_iCurrentScreenY + drawHeight / 2);
    int up = state->getTileValue(m_iCurrentScreenX + drawWidth / 2, m_iCurrentScreenY);
    int down = state->getTileValue(m_iCurrentScreenX + drawWidth / 2, m_iCurrentScreenY + drawHeight + 1);
    

    if (left <= 2 ) m_iCurrentScreenX += 2;
    if (right <= 2) m_iCurrentScreenX -= 2;
    if (up <= 2) m_iCurrentScreenY += 2;
    if (down <= 2) m_iCurrentScreenY -= 2;
    
    return false;

        
}

void Scyzc3Object::drawHP()
{
    double curHealth = (double)getHP() / (double)maxHp;
    m_engine->drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY - 2, m_iCurrentScreenX + 50, m_iCurrentScreenY + 5, 0xFFFFFF);
    if (curHealth > 1)    m_engine->drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY - 2, m_iCurrentScreenX + 50, m_iCurrentScreenY + 5, 0xFF0000);
    else if (curHealth > 0)    m_engine->drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY - 2, m_iCurrentScreenX + (int) (curHealth * 50.0), m_iCurrentScreenY + 5, 0xFF0000);
}

void Scyzc3Object::savePixel(SimpleImage img, int x, int y, int w, int h)
{
    box.clear();
    for (int i = y; i < y + h; i++)
    {
        int l = 0, start = 0, end = 0;
        SDL_Rect dot;
        dot.y = i + m_iCurrentScreenY - y;
        for (int j = x; j < x + w; j++)
        {
            if (img.getPixelColour(j, i) > 0)
            {
                if (start == 0)
                {
                    start = end = j;
                    l = 1;
                }
                else if (end == j - 1)
                {
                    l++;
                    end = j;
                }
                else
                {
                    start = j = end;
                }
            }
            else
            {
                if (l > 0) {
                    dot.x = start + m_iCurrentScreenX - x;
                    dot.w = l;
                    dot.h = 1;
                    box.push_back(dot);
                }
                start = end = l = 0;
            }
        }
    }
    //for (int i = 0; i < box.size(); i++)
    //{
    //    cout << box[i].y << " " << box[i].x << " " << box[i].w << endl;
    //}
}


player::player(Scyzc3Engine* pEngine, RunningState* state, string path, string name, int posX, int posY)
    :Scyzc3Object (pEngine, state, path),
    m_direction(0),
    name(name)
{
    image = path;
    tm = state->GetTileManager();
    m_iCurrentScreenX = posX;
    m_iCurrentScreenY = posY;
    hp = 100;
}

player::~player()
{
    cout << "player1 delete" << endl;
    box.clear();
}

void player::virtDraw()
{
    if (isVisible()) {
        displayImage = ImageManager::loadImage(image, true);
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
        drawHP();
        savePixel(displayImage, x, y, 50, 50);
        detectBound();
        goNextMap();

    }
}





void player::virtDoUpdate(int iCurrentTime)
{
    if (!m_engine->isPaused()) {

        if (m_engine->isKeyPressed(SDLK_w)) {
 
            if (flag1 % 100 < 25) m_direction = 1;
            else if (flag1 % 100 < 50) m_direction = 2;
            else if (flag1 % 100 < 75) m_direction = 3;
            else if (flag1 % 100 < 100) m_direction = 4;

            flag1++;

         
            m_iCurrentScreenY -= 2;
            if (isCollision() || detectBound()) { m_iCurrentScreenY += 2;}

        }

        if (m_engine->isKeyPressed(SDLK_s)) {

            if (flag2 % 100 < 25) m_direction = 5;
            else if (flag2 % 100 < 50) m_direction = 6;
            else if (flag2 % 100 < 75) m_direction = 7;
            else if (flag2 % 100 < 100) m_direction = 8;

            flag2++;

            m_iCurrentScreenY += 2;

            if (isCollision() || detectBound()) { m_iCurrentScreenY -= 2; }

        }

        if (m_engine->isKeyPressed(SDLK_a)) {
            if (flag3 % 100 < 25) m_direction = 9;
            else if (flag3 % 100 < 50) m_direction = 10;
            else if (flag3 % 100 < 75) m_direction = 11;
            else if (flag3 % 100 < 100) m_direction = 12;

            flag3++;

            m_iCurrentScreenX -= 2;
            if (isCollision() || detectBound()) { m_iCurrentScreenX += 2; }


        }

        if (m_engine->isKeyPressed(SDLK_d)) {
            if (flag4 % 100 < 25) m_direction = 13;
            else if (flag4 % 100 < 50) m_direction = 14;
            else if (flag4 % 100 < 75) m_direction = 15;
            else if (flag4 % 100 < 100) m_direction = 16;

            flag4++;

            m_iCurrentScreenX += 2;
            if (isCollision()|| detectBound()) { m_iCurrentScreenX -= 2; }

        }
    }
    
    if (m_iCurrentScreenX < 0 ) {
        m_direction = 9;
        m_iCurrentScreenX = 0;
    }

    if (m_iCurrentScreenX >= m_engine->getWindowWidth() - drawWidth) {
        m_direction = 10;
        m_iCurrentScreenX = m_engine->getWindowWidth() - drawWidth - 1;
    }

    if (m_iCurrentScreenY < 0) {
        m_direction = 11;
        m_iCurrentScreenY = 0;
    }

    if (m_iCurrentScreenY >= state->GetTileHeight() * 26 - drawHeight) {
        m_direction = 0;
        m_iCurrentScreenY = state->GetTileHeight() * 26 - drawHeight - 1;
    }


    this->redrawDisplay();
}



rival::rival(Scyzc3Engine* pEngine, RunningState* state, string path, string name, int posX, int posY)
    :Scyzc3Object(pEngine, state, path),
    m_direction(2),
    name(name)
{
    image = path;
    tm = state->GetTileManager();
    m_iCurrentScreenX = posX;
    m_iCurrentScreenY = posY;
    hp = 100;
}


void rival::virtDraw()
{
    if (isVisible()) {
        displayImage = ImageManager::loadImage(image, true);
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

        drawHP();
        savePixel(displayImage, x, y, 50, 50);
        detectBound();
        goNextMap();
    }
   
}

void rival::virtDoUpdate(int iCurrentTime)
{   
    if (!m_engine->isPaused()) {
        if (m_engine->isKeyPressed(SDLK_UP)) {
            if (flag1 % 100 < 25) m_direction = 1;
            else if (flag1 % 100 < 50) m_direction = 2;
            else if (flag1 % 100 < 75) m_direction = 3;
            else if (flag1 % 100 < 100) m_direction = 4;

            flag1++;

            m_iCurrentScreenY -= 2;
            if (isCollision()) { m_iCurrentScreenY += 2; }


        }

        if (m_engine->isKeyPressed(SDLK_DOWN)) {
            if (flag2 % 100 < 25) m_direction = 5;
            else if (flag2 % 100 < 50) m_direction = 6;
            else if (flag2 % 100 < 75) m_direction = 7;
            else if (flag2 % 100 < 100) m_direction = 8;

            flag2++;

            m_iCurrentScreenY += 2;

            if (isCollision()) { m_iCurrentScreenY -= 2; }
   
        }

        if (m_engine->isKeyPressed(SDLK_LEFT)) {
            if (flag3 % 100 < 25) m_direction = 9;
            else if (flag3 % 100 < 50) m_direction = 10;
            else if (flag3 % 100 < 75) m_direction = 11;
            else if (flag3 % 100 < 100) m_direction = 12;

            flag3++;

            m_iCurrentScreenX -= 2;
            if (isCollision()) { m_iCurrentScreenX += 2; }
 

        }

        if (m_engine->isKeyPressed(SDLK_RIGHT)) {
            if (flag4 % 100 < 25) m_direction = 13;
            else if (flag4 % 100 < 50) m_direction = 14;
            else if (flag4 % 100 < 75) m_direction = 15;
            else if (flag4 % 100 < 100) m_direction = 16;

            flag4++;


            m_iCurrentScreenX += 2;
            if (isCollision()) { m_iCurrentScreenX -= 2; }
        }
    }

    if (m_iCurrentScreenX < 0) {
        m_direction = 9;
        m_iCurrentScreenX = 0;
    }

    if (m_iCurrentScreenX >= m_engine->getWindowWidth() - drawWidth) {
        m_direction = 10;
        m_iCurrentScreenX = m_engine->getWindowWidth() - drawWidth - 1;
    }

    if (m_iCurrentScreenY < 0) {
        m_direction = 11;
        m_iCurrentScreenY = 0;
    }

    if (m_iCurrentScreenY >= state->GetTileHeight() * 26 - drawHeight) {
        m_direction = 0;
        m_iCurrentScreenY = state->GetTileHeight() * 26 - drawHeight - 1;
    }


    this->redrawDisplay();
}

void rival::setPosition(double dX, double dY)
{
    m_iCurrentScreenX = (int)dX;
    m_iCurrentScreenY = (int)dY;
}
