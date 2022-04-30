#include "game.h"
#include <algorithm>
#include <numeric>
#include <fstream>

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
			surroundings.at(1).setPlayer(4 - i, getSquare(x, y - i).getPlayer());
		}
		else
		{
			surroundings.at(1).setPlayer(4 - i, Invalid);
		}

		if (coordValid(x, y + i))
		{
			surroundings.at(1).setPlayer(4 + i, getSquare(x, y + i).getPlayer());
		}
		else
		{
			surroundings.at(1).setPlayer(4 + i, Invalid);
		}
	}

	// '\'
	surroundings.at(2).setPlayer(4, getSquare(x, y).getPlayer());
	for (size_t i = 4; i > 0; --i)
	{
		if (coordValid(x - i, y - i))
		{
			surroundings.at(2).setPlayer(4 - i, getSquare(x - i, y - i).getPlayer());
		}
		else
		{
			surroundings.at(2).setPlayer(4 - i, Invalid);
		}

		if (coordValid(x + i, y + i))
		{
			surroundings.at(2).setPlayer(4 + i, getSquare(x + i, y + i).getPlayer());
		}
		else
		{
			surroundings.at(2).setPlayer(4 + i, Invalid);
		}
	}

	// /
	surroundings.at(3).setPlayer(4, getSquare(x, y).getPlayer());
	for (size_t i = 4; i > 0; --i)
	{
		if (coordValid(x + i, y - i))
		{
			surroundings.at(3).setPlayer(4 - i, getSquare(x + i, y - i).getPlayer());
		}
		else
		{
			surroundings.at(3).setPlayer(4 - i, Invalid);
		}

		if (coordValid(x - i, y + i))
		{
			surroundings.at(3).setPlayer(4 + i, getSquare(x - i, y + i).getPlayer());
		}
		else
		{
			surroundings.at(3).setPlayer(4 + i, Invalid);
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
		for (size_t pieceSubscript = 0; pieceSubscript <= 9 - Patterns[patternSubscript].length(); ++pieceSubscript)
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

BoardAnalyser::BoardAnalyser(const Board &analysedBoard, int x, int y, Player analysedPlayer) : evaluatedPlayer(analysedPlayer), analysedStrips(analysedBoard.getSurroundingPieces(x, y))
{
	for (auto &strip : analysedStrips)
	{
		strip.setPlayer(4, analysedPlayer);
	}
}

int BoardAnalyser::analysisResult() const
{
	int result = 0;

	for (auto const &strip : analysedStrips)
	{
		result += getScoreOfStrip(strip);
	}

	return result;
}

void Game::updateScoreOfSquare(int x, int y, Player player)
{
	if (board.squareOccupied(x, y))
	{
		getXScoreOfSquare(x, y) = 0;
		getOScoreOfSquare(x, y) = 0;
	}

	else
	{
		BoardAnalyser analyser(board, x, y, player);

		if (player == X)
		{
			getXScoreOfSquare(x, y) = analyser.analysisResult();
		}
		else
		{
			getOScoreOfSquare(x, y) = analyser.analysisResult();
		}
	}
}

void Game::evaluateBoard()
{
	for (int x = 0; x < Board::SideLen; ++x)
	{
		for (int y = 0; y < Board::SideLen; ++y)
		{
			updateScoreOfSquare(x, y, X);
			updateScoreOfSquare(x, y, O);
		}
	}

	xScoreSum = std::accumulate(xScores.begin(), xScores.end(), 0L, [](long previousSum, int i)
								{ return previousSum + i; });
	oScoreSum = std::accumulate(oScores.begin(), oScores.end(), 0L, [](long previousSum, int i)
								{ return previousSum + i; });

	xMaxLocations.clear();
	xMaxLocations.push_back({0, 0});
	for (int x = 0; x < Board::SideLen; ++x)
	{
		for (int y = 0; y < Board::SideLen; ++y)
		{
			if (getXScoreOfSquare(x, y) > getXScoreOfSquare(xMaxLocations.at(0)))
			{
				xMaxLocations.clear();
				xMaxLocations.push_back({x, y});
			}
			else if (getXScoreOfSquare(x, y) == getXScoreOfSquare(xMaxLocations.at(0)))
			{
				xMaxLocations.push_back({x, y});
			}
		}
	}

	oMaxLocations.clear();
	oMaxLocations.push_back({0, 0});
	for (int x = 0; x < Board::SideLen; ++x)
	{
		for (int y = 0; y < Board::SideLen; ++y)
		{
			if (getOScoreOfSquare(x, y) > getOScoreOfSquare(oMaxLocations.at(0)))
			{
				oMaxLocations.clear();
				oMaxLocations.push_back({x, y});
			}
			else if (getOScoreOfSquare(x, y) == getOScoreOfSquare(oMaxLocations.at(0)))
			{
				oMaxLocations.push_back({x, y});
			}
		}
	}

	printScores();
}

void Game::printScores() const
{
	/** \FIXME Please note that for some mysterious reason, x and y are reversed! */
	std::ofstream fout("XScores.txt");

	fout << "X Score Table\n"
		 << "Score sum = " << xScoreSum << '\n'
		 << "Maximum score location(s): ";
	for (auto const &location : xMaxLocations)
	{
		fout << '(' << location.at(1) + 1 << ", " << location.at(0) + 1 << ") ";
	}
	fout << '\n'
		 << '\n';

	for (int y = 0; y < Board::SideLen; ++y)
	{
		for (int x = 0; x < Board::SideLen; ++x)
		{
			fout << getXScoreOfSquare(x, y) << " \n"[x == Board::SideLen - 1];
		}
	}

	fout.close();

	fout.open("OScores.txt");
	fout << "O Score Table\n"
		 << "Score sum = " << oScoreSum << '\n'
		 << "Maximum score location(s): ";
	for (auto const &location : oMaxLocations)
	{
		fout << '(' << location.at(1) + 1 << ", " << location.at(0) + 1 << ") ";
	}
	fout << '\n'
		 << '\n';

	for (int y = 0; y < Board::SideLen; ++y)
	{
		for (int x = 0; x < Board::SideLen; ++x)
		{
			fout << getOScoreOfSquare(x, y) << " \n"[x == Board::SideLen - 1];
		}
	}
	fout.close();
}

bool Game::placePiece(int x, int y)
{
	if (board.coordValid(x, y) && !board.squareOccupied(x, y))
	{
		board.getSquare(x, y).setPlayer(currentPlayer);
		occupiedSquares.push_back(board.getSquare(x, y));

		currentPlayer = ((currentPlayer == X) ? O : X);

		evaluateBoard();
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

	evaluateBoard();
}

void Game::restart()
{
	board.clear();
	occupiedSquares.clear();
	currentPlayer = X;

	evaluateBoard();
}
