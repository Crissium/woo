#include "ui.h"
#include <cstdlib>

const X1 XSprite::x1;
const X2 XSprite::x2;
const X3 XSprite::x3;
const X4 XSprite::x4;
const X5 XSprite::x5;
const O1 OSprite::o1;
const O2 OSprite::o2;
const O3 OSprite::o3;
const O4 OSprite::o4;
const O5 OSprite::o5;

XSprite::XSprite()
{
	switch (rand() % 5)
	{
	case 0:
		setTexture(x1);
		break;
	case 1:
		setTexture(x2);
		break;
	case 2:
		setTexture(x3);
		break;
	case 3:
		setTexture(x4);
		break;
	case 4:
		setTexture(x5);
		break;
	}
	setColor(sf::Color::Red);
}

OSprite::OSprite()
{
	switch (rand() % 5)
	{
	case 0:
		setTexture(o1);
		break;
	case 1:
		setTexture(o2);
		break;
	case 2:
		setTexture(o3);
		break;
	case 3:
		setTexture(o4);
		break;
	case 4:
		setTexture(o5);
		break;
	}
	setColor(sf::Color::Blue);
}

void Woo::drawLines()
{
	for (int x = 1; x < Board::SideLen; ++x)
	{
		sf::Vertex separator[] = {sf::Vertex(sf::Vector2f(x * PixelsPerUnit, 0.f)), sf::Vertex(sf::Vector2f(x * PixelsPerUnit, Board::SideLen * PixelsPerUnit))};
		window.draw(separator, 2, sf::Lines);
	}

	for (int y = 1; y < Board::SideLen; ++y)
	{
		sf::Vertex separator[] = {sf::Vertex(sf::Vector2f(0.f, y * PixelsPerUnit)),
								  sf::Vertex(sf::Vector2f(Board::SideLen * PixelsPerUnit, y * PixelsPerUnit))};
		window.draw(separator, 2, sf::Lines);
	}
}

bool Woo::placePiece(sf::Vector2i position)
{
	int x = position.x / int(PixelsPerUnit);
	int y = position.y / int(PixelsPerUnit);

	if (game.makeMove(x, y))
	{
		if (game.getCurrentPlayer() == X) // Player changes when a move is made
		{
			OSprite o;
			o.setPosition(x * PixelsPerUnit, y * PixelsPerUnit);
			OPieces.push_back(o);
		}
		else
		{
			XSprite s;
			s.setPosition(x * PixelsPerUnit, y * PixelsPerUnit);
			XPieces.push_back(s);
		}

		if (game.gameStatus() != 'r')
			gameOver = true;
		return true;
	}
	else
		return false;
}

void Woo::undo()
{
	if (!XPieces.empty() && !OPieces.empty())
	{
		game.undo();

		XPieces.pop_back();
		OPieces.pop_back();
	}
}

void Woo::restart()
{
	game.restart();
	gameOver = false;

	XPieces.clear();
	OPieces.clear();
}

void Woo::processEvents()
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::MouseButtonPressed:
			if (!gameOver)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					placePiece(sf::Mouse::getPosition(window));
				}
			}
			break;
		case sf::Event::KeyPressed:
			if (!gameOver)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Z:
					undo();
					break;
				case sf::Keyboard::R:
					restart();
					break;
				default:
					break;
				}
			}
			else
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Z:
					undo();
					gameOver = false;
					break;
				case sf::Keyboard::R:
					restart();
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}
	}
}

void Woo::render()
{
	drawLines();

	for (auto const &x : XPieces)
	{
		window.draw(x);
	}

	for (auto const &o : OPieces)
	{
		window.draw(o);
	}
}

void Woo::run()
{
	while (window.isOpen())
	{
		processEvents();

		window.clear(sf::Color(240, 220, 130));

		render();

		window.display();
	}
}