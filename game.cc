#include "game.h"
#include <algorithm>
#include <numeric>
#include <fstream>
#include <climits>
#include <ctime>
#include <iostream>

Player adversaryOf(const Player p)
{
	return ((p == X) ? O : X);
}

const Square &Square::operator=(const Square &other)
{
	if (*this == other)
		return *this;

	x = other.x;
	y = other.y;
	occupant = other.occupant;

	return *this;
}

bool PieceStrip::hasAWinningConnection() const
{
	for (size_t i = 0; i < 5; ++i)
	{
		if ((at(i) == X || at(i) == O) && (at(i) == at(i + 1) && at(i) == at(i + 2) && at(i) == at(i + 3) && at(i) == at(i + 4)))
			return true;
	}
	return false;
}

Board::Board() : mostRecentlyModifiedSquare(nullptr)
{
	for (int x = 0; x < SideLen; ++x)
	{
		for (int y = 0; y < SideLen; ++y)
		{
			getSquare(x, y).setCoord(x, y);
		}
	}
}

const int Board::Directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

const Board &Board::operator=(const Board &other)
{
	if (squares == other.squares)
		return *this;

	squares = other.squares;
	mostRecentlyModifiedSquare = &getSquare(other.mostRecentlyModifiedSquare->getX(), other.mostRecentlyModifiedSquare->getY());
	return *this;
}

bool Board::draw() const
{
	return (numSquaresOccupiedBy(Nobody) == 0);
}

char Board::gameStatus() const
{
	if (draw())
		return 'd';

	for (auto const &s : getSurroundingPieces(mostRecentlyModifiedSquare->getX(), mostRecentlyModifiedSquare->getY()))
	{
		if (s.hasAWinningConnection())
		{
			if (getCurrentPlayer() == X) // if the previous turn was O's
				return 'o';
			else
				return 'x';
		}
	}

	return 'r';
}

std::array<PieceStrip, 4> Board::getSurroundingPieces(int x, int y) const
{
	std::array<PieceStrip, 4> surroundings;

	for (size_t direction = 0; direction < 4; ++direction)
	{
		surroundings.at(direction).setPlayer(4, getSquare(x, y).getPlayer());

		for (int distance = 4; distance > 0; --distance)
		{
			if (coordValid(x + distance * Directions[direction][0], y + distance * Directions[direction][1]))
				surroundings.at(direction).setPlayer(4 - distance, getSquare(x + distance * Directions[direction][0], y + distance * Directions[direction][1]).getPlayer());
			else
				surroundings.at(direction).setPlayer(4 - distance, Invalid);

			if (coordValid(x - distance * Directions[direction][0], y - distance * Directions[direction][1]))
				surroundings.at(direction).setPlayer(4 + distance, getSquare(x - distance * Directions[direction][0], y - distance * Directions[direction][1]).getPlayer());
			else
				surroundings.at(direction).setPlayer(4 + distance, Invalid);
		}
	}

	return surroundings;
}

Board::Board(const Board &other) : squares(other.squares), mostRecentlyModifiedSquare(nullptr)
{
	std::transform(other.occupiedSquares.cbegin(), other.occupiedSquares.cend(), std::back_inserter(occupiedSquares), [this](const Square &s)
				   { return getSquare(s.getX(), s.getY()); });
}

Board::Board(const Board &other, int moveX, int moveY) : squares(other.squares)
{
	getSquare(moveX, moveY).setPlayer(getCurrentPlayer());
	mostRecentlyModifiedSquare = &getSquare(moveX, moveY);

	std::transform(other.occupiedSquares.cbegin(), other.occupiedSquares.cend(), std::back_inserter(occupiedSquares), [this](const Square &s)
				   { return getSquare(s.getX(), s.getY()); });

	occupiedSquares.push_back(getSquare(moveX, moveY));
}

int Board::numSquaresOccupiedBy(Player player) const
{
	return std::count_if(squares.cbegin(), squares.cend(), [player](const Square &s)
						 { return s.getPlayer() == player; });
}

bool Board::hasOccupiedSquaresNearby(int x, int y) const
{
	for (size_t direction = 0; direction < 4; ++direction)
	{
		for (int distance = 1; distance <= 2 /* magic number here! */; ++distance)
		{
			if ((coordValid(x + Directions[direction][0] * distance, y + Directions[direction][1] * distance) && squareOccupied(x + Directions[direction][0] * distance, y + Directions[direction][1] * distance)) || (coordValid(x - Directions[direction][0] * distance, y - Directions[direction][1] * distance) && squareOccupied(x - Directions[direction][0] * distance, y - Directions[direction][1] * distance)))
				return true;
		}
	}
	return false;
}

bool Board::terminatingMove(int x, int y) const
{
	return Board(*this, x, y).gameStatus() != 'r';
}

Player Board::getCurrentPlayer() const
{
	return ((numSquaresOccupiedBy(X) > numSquaresOccupiedBy(O)) ? O : X);
}

void Board::makeMove(int x, int y)
{
	getSquare(x, y).setPlayer(getCurrentPlayer());
	mostRecentlyModifiedSquare = &getSquare(x, y);

	occupiedSquares.push_back(getSquare(x, y));
}

void Board::undo()
{
	getSquare(occupiedSquares.crbegin()->getX(), occupiedSquares.crbegin()->getY()).setPlayer(Nobody);
	occupiedSquares.pop_back();

	getSquare(occupiedSquares.crbegin()->getX(), occupiedSquares.crbegin()->getY()).setPlayer(Nobody);
	occupiedSquares.pop_back();

	mostRecentlyModifiedSquare = &getSquare(occupiedSquares.crbegin()->getX(), occupiedSquares.crbegin()->getY());
}

void Board::clear()
{
	for (auto &it : squares)
	{
		if (it.getPlayer() != Nobody)
			it.setPlayer(Nobody);
	}

	occupiedSquares.clear();
}

const std::array<const std::string, 22> MoveAnalyser::Patterns({"11111", "011110", "011112", "0101110", "0110110", "01110", "010110", "001112", "010112", "011012", "10011", "10101", "2011102", "00110", "01010", "010010", "000112", "001012", "010012", "10001", "2010102", "2011002"});

int MoveAnalyser::scoreOfPattern(size_t patternSubscript) const
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

int MoveAnalyser::getScoreOfStrip(const PieceStrip &strip) const
{
	int score = 0;

	for (size_t patternSubscript = 0; patternSubscript < 22; ++patternSubscript)
	{
		for (size_t pieceSubscript = 0; pieceSubscript <= 9 - Patterns[patternSubscript].length(); ++pieceSubscript)
		{
			std::string searched;

			for (size_t i = 0; i < Patterns[patternSubscript].length(); ++i)
			{
				if (strip.at(pieceSubscript + i) == evaluatedPlayer)
					searched.push_back('1');
				else if (strip.at(pieceSubscript + i) == adversaryOf(evaluatedPlayer))
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

MoveAnalyser::MoveAnalyser(const Board &analysedBoard, int x, int y) : evaluatedPlayer(analysedBoard.getSquare(x, y).getPlayer()), analysedStrips(analysedBoard.getSurroundingPieces(x, y))
{
}

MoveAnalyser::MoveAnalyser(const Board &analysedBoard, int x, int y, Player analysedPlayer) : evaluatedPlayer(analysedPlayer), analysedStrips(analysedBoard.getSurroundingPieces(x, y))
{
	for (auto &strip : analysedStrips)
	{
		strip.setPlayer(4, evaluatedPlayer);
	}
}

int MoveAnalyser::analysisResult() const
{
	return std::accumulate(analysedStrips.cbegin(), analysedStrips.cend(), 0, [this](int previousScoreSum, const PieceStrip &s)
						   { return previousScoreSum + getScoreOfStrip(s); });
}

size_t GameState::numMovesMadeSoFarWhenCalled = 0;

GameState::GameState(const Board &b, int moveX, int moveY) : board(b, moveX, moveY)
{
}

int GameState::recentMovesAnalysisResult(Player player) const
{
	int scoreSum = 0;

	for (size_t moveIndex = numMovesMadeSoFarWhenCalled; moveIndex < board.numSquareOccupied(); ++moveIndex)
	{
		auto analysedSquare = board.getSquare(moveIndex);
		Player thisMovesPlayer = analysedSquare.getPlayer();

		int score = MoveAnalyser(board, analysedSquare.getX(), analysedSquare.getY()).analysisResult();
		score += MoveAnalyser(board, analysedSquare.getX(), analysedSquare.getY(), adversaryOf(thisMovesPlayer)).analysisResult();

		if (thisMovesPlayer == player)
			scoreSum += score;
		else
			scoreSum -= score;
	}

	return scoreSum;
	// int score = MoveAnalyser(board, board.mostRecentlyModifiedSquare->getX(), board.mostRecentlyModifiedSquare->getY()).analysisResult(); // For the last mover, how brilliant the move was!

	// score += MoveAnalyser(board, board.mostRecentlyModifiedSquare->getX(), board.mostRecentlyModifiedSquare->getY(), currentPlayer).analysisResult(); // From the last mover's point of view, if the adversary ever moved here, how terrible that would be! So it was lucky of me to block his wicked move.

	// if (player == lastMover)
	// 	return score; // Boy, you did a good job!
	// else
	// 	return -score; // Damn, what a move the enemy has made!
}

int GameState::utility(Player player) const
{
	char status = board.gameStatus();

	if (status == 'd')
		return 0;
	else if (status == 'r')
	{
		return recentMovesAnalysisResult(player);
	}
	else
	{
		if (player == ((status == 'x') ? X : O))
			return INT_MAX;
		else
			return INT_MIN;
	}
}

GameState GameState::result(const Coord &move) const
{
	return GameState(board, move.x, move.y);
}

std::vector<GameState::Coord> GameState::actions() const
{
	std::vector<GameState::Coord> moves;

	for (int x = 0; x < Board::SideLen; ++x)
	{
		for (int y = 0; y < Board::SideLen; ++y)
		{
			if (!board.squareOccupied(x, y) && board.hasOccupiedSquaresNearby(x, y))
				moves.push_back(Coord(x, y));
		}
	}

	return moves;
}

int GameState::maxValue(int alpha, int beta, Player player, int depth) const
{
	if (depth == 1 || terminal())
		return utility(player);

	int v = INT_MIN;

	for (auto const &a : actions())
	{
		v = std::max(v, result(a).minValue(alpha, beta, player, depth - 1));
		if (v >= beta)
			return v;
		alpha = std::max(alpha, v);
	}
	return v;
}

int GameState::minValue(int alpha, int beta, Player player, int depth) const
{
	if (depth == 1 || terminal())
		return utility(player);

	int v = INT_MAX;

	for (auto const &a : actions())
	{
		v = std::min(v, result(a).maxValue(alpha, beta, player, depth - 1));
		if (v <= alpha)
			return v;
		beta = std::min(beta, v);
	}
	return v;
}

int GameState::minimax(Player player, int depth) const
{
	if (depth == 1 || terminal())
	{
		return utility(player);
	}
	else
	{
		auto moves = actions();
		std::vector<int> values;
		std::transform(moves.cbegin(), moves.cend(), std::back_inserter(values), [this, player, depth](const Coord &move)
					   { return result(move).minimax(player, depth - 1); });
		if (board.getCurrentPlayer() == player)
			return *std::max_element(values.cbegin(), values.cend());
		else
			return *std::min_element(values.cbegin(), values.cend());
	}
}

int GameState::alphaBetaAnalysis(Player player, int depth) const
{
	if (board.getCurrentPlayer() == player)
		return maxValue(INT_MIN, INT_MAX, player, depth);
	else
		return minValue(INT_MIN, INT_MAX, player, depth);
}

bool Game::placePiece(int x, int y)
{
	if (board.coordValid(x, y) && !board.squareOccupied(x, y))
	{
		board.makeMove(x, y);

		currentPlayer = adversaryOf(currentPlayer);

		return true;
	}
	else
		return false;
}

bool Game::autoMove()
{
	// Make a very fast opening move
	if (board.numSquaresOccupiedBy(X) == 0)
	{
		return placePiece(Board::SideLen / 2, Board::SideLen / 2);
	}
	else if (board.numSquaresOccupiedBy(X) == 1)
	{
		if (!placePiece(Board::SideLen / 2, Board::SideLen / 2))
			while (!placePiece(Board::SideLen / 2 + rand() % 3 - 1, Board::SideLen / 2 + rand() % 3 - 1))
				;
		return true;
	}
	else
	{
		int maxScore = INT_MIN;
		int bestX = 0, bestY = 0;

		time_t startTime = time(nullptr);

		for (int x = 0; x < Board::SideLen; ++x)
		{
			for (int y = 0; y < Board::SideLen; ++y)
			{
				if (!board.squareOccupied(x, y) && board.hasOccupiedSquaresNearby(x, y))
				{
					if (board.terminatingMove(x, y))
						return placePiece(x, y);

					GameState::numMovesMadeSoFarWhenCalled = board.numSquareOccupied();
					int score = GameState(board, x, y).alphaBetaAnalysis(currentPlayer, aiDepth);
					std::cout << '(' << y << ',' << x << ')' << ':' << score << '\n';
					if (score > maxScore)
					{
						maxScore = score;
						bestX = x;
						bestY = y;
					}
				}
			}
		}

		std::cout << "Analysis took " << time(nullptr) - startTime << " second(s).\n"
				  << std::endl;

		return placePiece(bestX, bestY);
	}
}

void Game::restart()
{
	board.clear();
	currentPlayer = X;
}
