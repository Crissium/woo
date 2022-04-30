#include "game.h"
#include <algorithm>

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

const std::array<const std::string, 22> BoardAnalyser::Patterns({"11111", "011110", "011112", "0101110", "0110110", "01110", "010110", "001112", "010112", "011012", "10011", "10101", "2011102", "00110", "01010", "010010", "000112", "001012", "010012", "10001", "2010102", "2011002"});

int BoardAnalyser::scoreOfPattern(size_t patternSubscript) const
{
	int score = 0;
	switch (patternSubscript)
	{
	case 0:
		score = 100000;
		break;
	case 1:
		score = 10000;
		break;
	case 2:
	case 3:
	case 4:
		score = 500;
		break;
	case 5:
	case 6:
		score = 200;
		break;
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
		score = 50;
		break;
	case 13:
	case 14:
	case 15:
		score = 5;
		break;
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
		score = 3;
		break;
	default:
		break;
	}
	return score;
}

std::array<PieceStrip, 4> Board::getSurroundingPieces(int x, int y) const
{
	std::array<PieceStrip, 4> surroundings;

	// -
	surroundings.at(0).setPlayer(4, getSquare(x, y).getPlayer());
	for (size_t i = 4; i > 0; --i)
	{
		// Left side
		if (coordValid(x - i, y))
		{
			surroundings.at(0).setPlayer(4 - i, getSquare(x - i, y).getPlayer());
		}
		else
		{
			surroundings.at(0).setPlayer(4 - i, Invalid);
		}

		// Right side
		if (coordValid(x + i, y))
		{
			surroundings.at(0).setPlayer(4 + i, getSquare(x + i, y).getPlayer());
		}
		else
		{
			surroundings.at(0).setPlayer(4 + i, Invalid);
		}
	}

	// |
	surroundings.at(1).setPlayer(4, getSquare(x, y).getPlayer());
	for (size_t i = 4; i > 0; --i)
	{
		if (coordValid(x, y - i))
		{
			surroundings.at(0).setPlayer(4 - i, getSquare(x, y - i).getPlayer());
		}
		else
		{
			surroundings.at(0).setPlayer(4 - i, Invalid);
		}

		if (coordValid(x, y + i))
		{
			surroundings.at(0).setPlayer(4 + i, getSquare(x, y + i).getPlayer());
		}
		else
		{
			surroundings.at(0).setPlayer(4 + i, Invalid);
		}
	}

	// '\'
	surroundings.at(2).setPlayer(4, getSquare(x, y).getPlayer());
	for (size_t i = 4; i > 0; --i)
	{
		if (coordValid(x - i, y - i))
		{
			surroundings.at(0).setPlayer(4 - i, getSquare(x - i, y - i).getPlayer());
		}
		else
		{
			surroundings.at(0).setPlayer(4 - i, Invalid);
		}

		if (coordValid(x + i, y + i))
		{
			surroundings.at(0).setPlayer(4 + i, getSquare(x + i, y + i).getPlayer());
		}
		else
		{
			surroundings.at(0).setPlayer(4 + i, Invalid);
		}
	}

	// /
	surroundings.at(3).setPlayer(4, getSquare(x, y).getPlayer());
	for (size_t i = 4; i > 0; --i)
	{
		if (coordValid(x + i, y + i))
		{
			surroundings.at(0).setPlayer(4 + i, getSquare(x + i, y + i).getPlayer());
		}
		else
		{
			surroundings.at(0).setPlayer(4 + i, Invalid);
		}

		if (coordValid(x + i, y + i))
		{
			surroundings.at(0).setPlayer(4 + i, getSquare(x + i, y + i).getPlayer());
		}
		else
		{
			surroundings.at(0).setPlayer(4 + i, Invalid);
		}
	}

	return surroundings;
}

int BoardAnalyser::getScoreOfStrip(const PieceStrip &strip) const
{
	int score = 0;
	Player adversary = (evaluatedPlayer == X) ? O : X;
	
	for (size_t patternSubscript = 0; patternSubscript < 22; ++patternSubscript)
	{
		for (size_t pieceSubscript = 0; pieceSubscript < 9 - Patterns[patternSubscript].length(); ++pieceSubscript)
		{
			std::string searched;

			for (size_t i = 0; i < Patterns[patternSubscript].length(); ++i)
			{
				if (strip.at(pieceSubscript + i) == evaluatedPlayer)
					searched.push_back('1');
				else if (strip.at(pieceSubscript + i) == adversary)
					searched.push_back('2');
				else if (strip.at(pieceSubscript + i) == Nobody)
					searched.push_back('0');
				else
					searched.push_back('i'); // 'I'nvalid
			}

			if (searched == Patterns[patternSubscript])
			{
				score += scoreOfPattern(patternSubscript);
			}
		}
	}

	return score;
}

BoardAnalyser::BoardAnalyser(const Board &analysedBoard, int x, int y, Player analysedPlayer) :evaluatedPlayer(analysedPlayer), analysedStrips(analysedBoard.getSurroundingPieces(x, y))
{
	for (auto & strip : analysedStrips)
	{
		strip.setPlayer(4, analysedPlayer);
	}
}

int BoardAnalyser::analysisResult() const
{
	int result = 0;

	for (auto const & strip : analysedStrips)
	{
		result += getScoreOfStrip(strip);
	}

	return result;
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
