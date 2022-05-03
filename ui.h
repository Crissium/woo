#ifndef UI_H_
#define UI_H_

#include "game.h"
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define PixelsPerUnit 40.f

extern const unsigned int ConsoleHeight;

class X1 : public sf::Texture
{
public:
	X1() { loadFromFile("x1.png"); }
	virtual ~X1() {}
};

class X2 : public sf::Texture
{
public:
	X2() { loadFromFile("x2.png"); }
	virtual ~X2() {}
};

class X3 : public sf::Texture
{
public:
	X3() { loadFromFile("x3.png"); }
	virtual ~X3() {}
};

class X4 : public sf::Texture
{
public:
	X4() { loadFromFile("x4.png"); }
	virtual ~X4() {}
};

class X5 : public sf::Texture
{
public:
	X5() { loadFromFile("x5.png"); }
	virtual ~X5() {}
};

class O1 : public sf::Texture
{
public:
	O1() { loadFromFile("o1.png"); }
	virtual ~O1() {}
};

class O2 : public sf::Texture
{
public:
	O2() { loadFromFile("o2.png"); }
	virtual ~O2() {}
};

class O3 : public sf::Texture
{
public:
	O3() { loadFromFile("o3.png"); }
	virtual ~O3() {}
};

class O4 : public sf::Texture
{
public:
	O4() { loadFromFile("o4.png"); }
	virtual ~O4() {}
};

class O5 : public sf::Texture
{
public:
	O5() { loadFromFile("o5.png"); }
	virtual ~O5() {}
};

class XSprite : public sf::Sprite
{
private:
	const static X1 x1;
	const static X2 x2;
	const static X3 x3;
	const static X4 x4;
	const static X5 x5;

public:
	XSprite(int x, int y);
	virtual ~XSprite() {}
};

class OSprite : public sf::Sprite
{
private:
	const static O1 o1;
	const static O2 o2;
	const static O3 o3;
	const static O4 o4;
	const static O5 o5;

public:
	OSprite(int x, int y);
	virtual ~OSprite() {}
};

class Status : public sf::Text
{
private:
	sf::Font font;

public:
	Status();
	virtual ~Status() {}
	void updateStatus(char gameStatus);
};

class Woo
{
private:
	Game game;
	bool gameOver;

	sf::RenderWindow window;
	Status status;
	std::vector<XSprite> XPieces;
	std::vector<OSprite> OPieces;

	bool placePiece(sf::Vector2i position);
	void autoPlace();
	void undo();
	void restart();

	void processEvents();
	void drawLines();
	void render();

public:
	Woo() : gameOver(false), window(sf::VideoMode(Board::SideLen * PixelsPerUnit, Board::SideLen * PixelsPerUnit + ConsoleHeight), "Woo", sf::Style::Close | sf::Style::Titlebar) { srand(time(nullptr)); }
	~Woo() {}

	void run();
};

#endif
