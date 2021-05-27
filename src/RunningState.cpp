#include "RunningState.h"
#include "Scyzc3Engine.h"
#include "Scyzc3Object.h"
#include "MovingPokemon.h"
#include "MovingPokemon2.h"
#include "Scyzc3TileManager.h"
#include "bullet.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <string>
#include "Bgm.h"

using namespace std;

void RunningState::Handle(Context* c)
{
	cout << "Running" << endl;
}

RunningState::RunningState(Scyzc3Engine* pEngine) :
	player1(NULL),
	player2(NULL),
	pokemon(NULL),
	pokemon2(NULL),
	firstBullet(NULL),
	secondBullet(NULL),
	thirdBullet(NULL),
	firstBullet2(NULL),
	secondBullet2(NULL),
	thirdBullet2(NULL),
	tileHeight(0),
	tileWidth(0),
	tm(),
	m_engine(pEngine),
	name1(" "),
	name2(" ")
{
	bgm = m_engine->getBgm();
	bgm->createChunk();
	//cout << "start RunningState" << endl;
}

RunningState::~RunningState()
{
	cout << "delete RunningState" << endl;
	if (m_engine->getDisplayableObject(7) == NULL)
	{
		delete firstBullet2;
	}
	if (m_engine->getDisplayableObject(8) == NULL)
	{
		delete secondBullet2;
	}
	if (m_engine->getDisplayableObject(9) == NULL)
	{
		delete thirdBullet2;
	}
}

void RunningState::virtSetupBackgroundBuffer()
{
	name1 = "images/" + m_engine->getName1();
	name2 = "images/" + m_engine->getName2();

	firstBullet2 = NULL;
	secondBullet2 = NULL;
	thirdBullet2 = NULL;

	Bgm :: playMusic(3);

	m_engine->fillBackground(0xFFFFFF);

	m_engine->lockBackgroundForDrawing();

	display = ImageManager::loadImage("images/display.png", true);
	display.renderImage(m_engine->getBackgroundSurface(), 10, 0, 0, 650,
		display.getWidth(), display.getHeight());

	profile1 = ImageManager::loadImage(name1 + "Image.png", true);
	profile1.renderImageWithMask(m_engine->getBackgroundSurface(), 0, 0, 60, 680,
		profile1.getWidth(), profile1.getHeight());

	profile2 = ImageManager::loadImage(name2 + "Image.png", true);
	profile2.renderImageWithMask(m_engine->getBackgroundSurface(), 0, 0, 1140, 680,
		profile2.getWidth(), profile2.getHeight());

	help = ImageManager::loadImage("images/help.png", true);
	help.renderImageWithMask(m_engine->getBackgroundSurface(), 0, 0, 620, 720,
		help.getWidth(), help.getHeight());

	pause = ImageManager::loadImage("images/pause1.png", true);
	pause.renderImageWithMask(m_engine->getBackgroundSurface(), 0, 0, 500, 720,
		pause.getWidth(), pause.getHeight());

	save = ImageManager::loadImage("images/save.png", true);
	save.renderImageWithMask(m_engine->getBackgroundSurface(), 0, 0, 680, 720,
		save.getWidth(), save.getHeight());

	quit = ImageManager::loadImage("images/quit.png", true);
	quit.renderImageWithMask(m_engine->getBackgroundSurface(), 0, 0, 560, 720,
		quit.getWidth(), quit.getHeight());

	volume = ImageManager::loadImage("images/volume1.png", true);
	volume.renderImageWithMask(m_engine->getBackgroundSurface(), 0, 0, 200, 720,
		volume.getWidth(), volume.getHeight());


	if (m_engine->getMapId() == 0)
		loadMap("resources/map1.txt");
	else if (m_engine->getMapId() == 1)
		loadMap("resources/map2.txt");
	else if (m_engine->getMapId() == 2)
		loadMap("resources/map3.txt");

	UpdateView();
	vector<string> map = currentView;
	tm.setMapSize(tileWidth, tileHeight);

	for (int i = 0; i < tileWidth; i++)
		for (int j = 0; j < tileHeight; j++) {
			int value = map[j].at(i) - 'a';
			tm.setMapValue(i, j, value);
		}	

	tm.setTopLeftPositionOnScreen(0, 0);
	tm.drawAllTiles(m_engine, m_engine->getBackgroundSurface());

	map.swap(map);
	map.clear();

	virtInitialiseObjects();
}

void RunningState::virtMouseDown(int iButton, int iX, int iY)
{
	printf("Mouse clicked at %d %d\n", iX, iY);
	if (iButton == SDL_BUTTON_LEFT)
	{
		if (iX > 500 && iX < 550 && iY > 720 && iY < 770)
		{
			if (!m_engine->isPaused())
			{
				m_engine->pause();
				Bgm::pauseMusic();
				pause2 = ImageManager::loadImage("images/pause2.png", true);
				pause2.renderImageWithMask(m_engine->getBackgroundSurface(), 0, 0, 500, 720,
					pause2.getWidth(), pause2.getHeight());
			}
			else if (m_engine->isPaused())
			{
				m_engine->unpause();
				Bgm::resumeMusic();
				pause = ImageManager::loadImage("images/pause1.png", true);
				pause.renderImageWithMask(m_engine->getBackgroundSurface(), 0, 0, 500, 720,
					pause.getWidth(), pause.getHeight());
			}
		}
		if (iX > 560 && iX < 610 && iY > 720 && iY < 770)
		{
			exit(0);
		}
		if (iX > 620 && iX < 670 && iY > 720 && iY < 770) {
			if (!m_engine->isPaused())	m_engine->pause();

			saveGame("resources/saveInfo.txt", 1);
			m_engine->setLoad(4);
			m_engine->setAllObjectsVisible(false);
			m_engine->StateSwitch(2);
			
		}
		if (iX > 680 && iX < 730 && iY > 720 && iY < 770) {
		
			if (!m_engine->isPaused())	m_engine->pause();

			saveGame("resources/saveInfo.txt", 1);
			m_engine->setLoad(4);
			m_engine->setAllObjectsVisible(false);
			m_engine->StateSwitch(4);
		}
		if (iX > 200 && iX < 250 && iY > 720 && iY < 770) {
			if (Mix_PausedMusic() == 0)
			{
				Bgm::pauseMusic();
				bgm->pauseChunk();
				volume = ImageManager::loadImage("images/volume2.png", true);
				volume.renderImageWithMask(m_engine->getBackgroundSurface(), 0, 0, 200, 720,
					volume.getWidth(), volume.getHeight());
			}
			else if (Mix_PausedMusic() == 1)
			{
				Bgm::resumeMusic();
				bgm->createChunk();
				volume = ImageManager::loadImage("images/volume1.png", true);
				volume.renderImageWithMask(m_engine->getBackgroundSurface(), 0, 0, 200, 720,
					volume.getWidth(), volume.getHeight());
			}
		}
		if (tm.isValidTilePosition(iX, iY)) // Clicked within tiles?
		{
			int mapX = tm.getMapXForScreenX(iX); // Which column?
			int mapY = tm.getMapYForScreenY(iY); // Which row?
			int value = tm.getMapValue(mapX, mapY); // Current value?
			if (value == 3 || value == 9) tm.setAndRedrawMapValueAt(mapX, mapY,  0, m_engine, m_engine->getBackgroundSurface());
			m_engine->redrawDisplay(); // Force background to be redrawn to foreground
		}
	}
	
}

int RunningState::virtInitialiseObjects()
{
	m_engine->drawableObjectsChanged();
	m_engine->destroyOldObjects(true);
	if (m_engine->isPaused())	m_engine->unpause();

	player1 = new player(m_engine, this, name1 + ".png", name1, p1X, p1Y);
	player2 = new rival(m_engine, this, name2 + ".png", name2, p2X, p2Y);

	pokemon = new MovingPokemon(m_engine, this, player2, "images/gengar.png", p3X, p3Y);
	pokemon2 = new MovingPokemon2(m_engine, this, player1, "images/dragon.png", p4X, p4Y);

	firstBullet = new bullet(m_engine, this, "images/flash.png", player1, -1);
	secondBullet = new bullet(m_engine, this, "images/flash.png", player1, -1);
	thirdBullet = new bullet(m_engine, this, "images/flash.png", player1, -1);

	player1->setHP(hp1);
	player2->setHP(hp2); 
	player1->setScore(s1);
	player2->setScore(s2);
	pokemon->setHP(hp3);
	pokemon2->setHP(hp4);
	pokemon2->setFly(true);

	if (m_engine->getLevel() == 0)
	{
		pokemon->setMaxHp(100);
		pokemon->setSpeed(1);
		pokemon2->setMaxHp(100);
		pokemon2->setSpeed(1);
	}
	else if (m_engine->getLevel() == 1)
	{
		pokemon->setMaxHp(200);
		pokemon->setSpeed(2);
		pokemon2->setMaxHp(200);
		pokemon2->setSpeed(2);
	}
	else if (m_engine->getLevel() == 2)
	{
		pokemon->setMaxHp(300);
		pokemon->setSpeed(3);
		pokemon2->setMaxHp(300);
		pokemon2->setSpeed(3);
	}
	if (m_engine->getLoad() == 0)
	{
		pokemon->setHP(pokemon->getMaxHp());
		pokemon2->setHP(pokemon2->getMaxHp());
	}
	//pokemon2->setHP(0);
	m_engine->createObjectArray(10);
	m_engine->storeObjectInArray(0, player1);
	m_engine->storeObjectInArray(1, player2);
	m_engine->storeObjectInArray(2, pokemon);
	m_engine->storeObjectInArray(3, pokemon2);
	m_engine->storeObjectInArray(4, firstBullet);
	m_engine->storeObjectInArray(5, secondBullet);
	m_engine->storeObjectInArray(6, thirdBullet);


	m_engine->setAllObjectsVisible(true);
	firstBullet->setVisible(false);
	secondBullet->setVisible(false);
	thirdBullet->setVisible(false);

	return 0;
}

void RunningState::virtDrawStringsOnTop() 
{
	char buf[128];
	char buf2[128];
	char buf3[128];
	if (player1->getHP() <= 0) {
		if (!m_engine->isPaused())	m_engine->pause();
		m_engine->setWinId(2);
		m_engine->setWinScore(player2->getScore());
		m_engine->StateSwitch(6);
	}
	else if(player2->getHP() <= 0){
		if (!m_engine->isPaused())	m_engine->pause();
		m_engine->setWinId(1);
		m_engine->setWinScore(player1->getScore());
		m_engine->StateSwitch(6);
	}
	

	if (player1->getHP() < player2->getHP())
	{
		pokemon->setPlayer(player1);
		pokemon2->setPlayer(player2);
	}
	else
	{
		pokemon->setPlayer(player2);
		pokemon2->setPlayer(player1);
	}
	sprintf(buf, "Game Time: %3.3f s", m_engine->getModifiedTime() / 1000.0 - m_engine->getRunTime());
	sprintf(buf2, "Score: %d", player1->getScore());
	sprintf(buf3, "Score: %d", player2->getScore());

	m_engine->drawForegroundString(500, 680, buf, 0xffffff, NULL);
	m_engine->drawForegroundString(200, 680, buf2, 0xff0000, NULL);
	m_engine->drawForegroundString(1000, 680, buf3, 0xff0000, NULL);
	m_engine->redrawDisplay();
}

void RunningState::loadMap(char* filename)
{
	ifstream infoFile;
	bool init = false;
	if (m_engine->getLoad() == 0)
	{
		infoFile.open("resources/info.txt", ios::in);
		init = true;
	}
	else if(m_engine->getLoad() == 4)
		infoFile.open("resources/saveInfo.txt", ios::in);
	else if (m_engine->getLoad() == 1)
		infoFile.open("resources/Archive1.txt", ios::in);
	else if (m_engine->getLoad() == 2)
		infoFile.open("resources/Archive2.txt", ios::in);
	else if (m_engine->getLoad() == 3)
		infoFile.open("resources/Archive3.txt", ios::in);
	
	if (!infoFile.is_open()) {
		cout << "info cannot open" << endl;
		return;
	}
	int id, level;
	infoFile >> id >> level >> p1X >> p1Y >> hp1 >> s1 >> p2X >> p2Y >> hp2 >> s2 >> p3X >> p3Y >> hp3 >> p4X >> p4Y >> hp4;

	if (!init)
	{
		m_engine->setMapId(id);
		m_engine->setLevel(level);
	}

	infoFile.close();


	ifstream mapFile;
	string line;
	int count = 0;
	int mapLine = 0;
	mapFile.open(filename, ios::in);
	mapData.clear();

	if (!mapFile.is_open()) {
		cout << "File failed to open!" << endl;
		return;
	}

	cout << "Reading map data..." << endl;

	while (getline(mapFile, line, ',')) {
		if (line.substr(0, 1) == "\n") {
			line.erase(0, 1);
		}
		mapData.push_back(line);
	}

	tileHeight = mapData.size();
	tileWidth = mapData[1].length();
}


void RunningState::saveGame(char* filename, int reset)
{
	ofstream outFile;
	time_t now = time(0);

	if (reset == 1)
		outFile.open(filename, ios::out);
	else if (reset == 0)
		outFile.open(filename, ios::app);


	//outFile << m_engine->getMapId() << " "
	//	<< player1->getPositionX() << " " << player1->getPositionY() << " " << player1->getHP() << " " << player1->getScore() << " "
	//	<< player2->getPositionX() << " " << player2->getPositionY() << " " << player2->getHP() << " " << player2->getScore() << " "
	//	<< pokemon->getPositionX() << " " << pokemon->getPositionY() << " " << pokemon->getHP() << "\n" << ctime(&now);

	// after operator oveloading

	outFile << m_engine->getMapId() << " " << m_engine->getLevel() << " ";
	outFile << player1 << player2 << pokemon << pokemon2;
	outFile << "\n" << ctime(&now);

	now = NULL;
	outFile.close();
}

vector<string> RunningState::GetMapData()
{
	return mapData;
}

vector<string> RunningState::GetCurrentView()
{
	return currentView;
}

void RunningState::UpdateView()
{
	currentView.clear();
	currentView = mapData;
	mapData.clear();
}

int RunningState::getTileValue(int x, int y)
{
	int tmX = x / tm.getTileWidth();
	int tmY = y / tm.getTileHeight();

	return tm.getMapValue(tmX, tmY);
}

void RunningState::virtKeyDown(int ikeyDown)
{
	if (ikeyDown == SDLK_ESCAPE) m_engine->setExitWithCode(1);
	if (!m_engine->isPaused())
	{
		if (ikeyDown == SDLK_SPACE) {
			bgm->playChunk();
			if (flag % 3 == 0) {
				firstBullet->setDirection(player1->getdirection());
				firstBullet->setPosition(player1->getPositionX() + 12, player1->getPositionY() + 12);
				firstBullet->setVisible(true);
			}
			else if (flag % 3 == 1) {
				secondBullet->setDirection(player1->getdirection());
				secondBullet->setPosition(player1->getPositionX() + 12, player1->getPositionY() + 12);
				secondBullet->setVisible(true);
			}
			else if (flag % 3 == 2) {
				thirdBullet->setDirection(player1->getdirection());
				thirdBullet->setPosition(player1->getPositionX() + 12, player1->getPositionY() + 12);
				thirdBullet->setVisible(true);
			}
			flag++;
		}

		if (ikeyDown == SDLK_RSHIFT) {
			bgm->playChunk();
			if (flagg % 3 == 0) {
				delete firstBullet2;
				firstBullet2 = new bullet(m_engine, this, "images/flame.png", player2, -1);
				firstBullet2->setObjectID(7);
				m_engine->storeObjectInArray(7, firstBullet2);
				firstBullet2->setDirection(player2->getdirection());
			}
			else if (flagg % 3 == 1) {
				delete secondBullet2;
				secondBullet2 = new bullet(m_engine, this, "images/flame.png", player2, -1);
				secondBullet2->setObjectID(8);
				m_engine->storeObjectInArray(8, secondBullet2);
				secondBullet2->setDirection(player2->getdirection());
			}
			else if (flagg % 3 == 2) {
				delete thirdBullet2;
				thirdBullet2 = new bullet(m_engine, this, "images/flame.png", player2, -1);
				thirdBullet2->setObjectID(9);
				m_engine->storeObjectInArray(9, thirdBullet2);
				thirdBullet2->setDirection(player2->getdirection());
			}
			flagg++;
		}


	}
	
}

float RunningState::GetDistance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void RunningState::copyAllBackgroundBuffer()
{
	m_engine->copyBackgroundPixels(0, 0, 1300, 800);
}


ofstream& operator<<(ofstream& output, player*& obj)
{
	// TODO: insert return statement here
	output << obj->getPositionX() << " " << obj->getPositionY() << " " << obj->getHP() << " " << obj->getScore() << " ";
	return output;
}

ofstream& operator<<(ofstream& output, rival*& obj)
{
	// TODO: insert return statement here
	output << obj->getPositionX() << " " << obj->getPositionY() << " " << obj->getHP() << " " << obj->getScore() << " ";
	return output;
}

ofstream& operator<<(ofstream& output, MovingPokemon2*& obj)
{
	// TODO: insert return statement here
	output << obj->getPositionX() << " " << obj->getPositionY() << " " << obj->getHP() << " ";
	return output;
}

ofstream& operator<<(ofstream& output, MovingPokemon*& obj)
{
	// TODO: insert return statement here
	output << obj->getPositionX() << " " << obj->getPositionY() << " " << obj->getHP() << " ";
	return output;
}