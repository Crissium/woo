#include "game.h"

const Square &Square::operator=(const Square &other)
{
	if (*this == other)
		return *this;

	x = other.x;
	y = other.y;
	occupant = other.occupant;

	return *this;
}

Board::Board()
{
	for (int x = 0; x < SideLen; ++x)
	{
		for (int y = 0; y < SideLen; ++y)
		{
			getSquare(x, y).setCoord(x, y);
		}
	}
}

const Board &Board::operator=(const Board &other)
{
	if (squares == other.squares)
		return *this;

	squares = other.squares;
	return *this;
}

bool Board::draw() const
{
	for (int x = 0; x < SideLen; ++x)
	{
		for (int y = 0; y < SideLen; ++y)
		{
			if (getSquare(x, y).getPlayer() == Nobody)
				return false;
		}
	}

	return true;
}

char Board::gameStatus() const
{
	Player player = Nobody;

	// -
	for (int x = 0; x < SideLen; ++x)
	{
		for (int y = 0; y < SideLen - 4; ++y)
		{
			if ((player = (getSquare(x, y)).getPlayer()) != Nobody && player == getSquare(x, y + 1).getPlayer() && player == getSquare(x, y + 2).getPlayer() && player == getSquare(x, y + 3).getPlayer() && player == getSquare(x, y + 4).getPlayer())
			{
				if (player == X)
					return 'x';
				else
					return 'o';
			}
		}
	}

	// |
	for (int y = 0; y < SideLen; ++y)
	{
		for (int x = 0; x < SideLen - 4; ++x)
		{
			if ((player = (getSquare(x, y)).getPlayer()) != Nobody && player == getSquare(x + 1, y).getPlayer() && player == getSquare(x + 2, y).getPlayer() && player == getSquare(x + 3, y).getPlayer() && player == getSquare(x + 4, y).getPlayer())
			{
				if (player == X)
					return 'x';
				else
					return 'o';
			}
		}
	}

	// '\'
	for (int x = 0; x < SideLen - 4; ++x)
	{
		for (int y = 0; y < SideLen - 4; ++y)
		{
			if ((player = (getSquare(x, y)).getPlayer()) != Nobody && player == getSquare(x + 1, y + 1).getPlayer() && player == getSquare(x + 2, y + 2).getPlayer() && player == getSquare(x + 3, y + 3).getPlayer() && player == getSquare(x + 4, y + 4).getPlayer())
			{
				if (player == X)
					return 'x';
				else
					return 'o';
			}
		}
	}

	// /
	for (int x = 0; x < SideLen - 4; ++x)
	{
		for (int y = SideLen - 1; y > 3; --y)
		{
			if ((player = (getSquare(x, y)).getPlayer()) != Nobody && player == getSquare(x + 1, y - 1).getPlayer() && player == getSquare(x + 2, y - 2).getPlayer() && player == getSquare(x + 3, y - 3).getPlayer() && player == getSquare(x + 4, y - 4).getPlayer())
			{
				if (player == X)
					return 'x';
				else
					return 'o';
			}
		}
	}

	if (draw())
		return 'd';
	else
		return 'r';
}

void Board::clear()
{
	for (auto &it : squares)
	{
		if (it.getPlayer() != Nobody)
			it.setPlayer(Nobody);
	}
}

bool Game::placePiece(int x, int y)
{
	if (board.coordValid(x, y) && !board.squareOccupied(x, y))
	{
		board.getSquare(x, y).setPlayer(currentPlayer);
		occupiedSquares.push_back(board.getSquare(x, y));

		currentPlayer = ((currentPlayer == X) ? O : X);
		return true;
	}
	else
		return false;
}

void Game::undo()
{
	board.getSquare(occupiedSquares.crbegin()->getX(), occupiedSquares.crbegin()->getY()).setPlayer(Nobody);
	occupiedSquares.pop_back();

	board.getSquare(occupiedSquares.crbegin()->getX(), occupiedSquares.crbegin()->getY()).setPlayer(Nobody);
	occupiedSquares.pop_back();
}

void Game::restart()
{
	board.clear();
	occupiedSquares.clear();
	currentPlayer = X;
}
