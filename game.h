#ifndef GAME_H_
#define GAME_H_

#include <array>
#include <vector>
#include <string>

enum Player
{
	Nobody,
	X,
	O,
	Invalid
};

class Square
{
private:
	int x;
	int y;
	Player occupant;

public:
	Square(int p = 0, int q = 0, Player occupier = Nobody) : x(p), y(q), occupant(occupier) {}
	void setCoord(int p, int q)
	{
		x = p;
		y = q;
	}
	void setPlayer(Player occupier) { occupant = occupier; }
	int getX() const { return x; }
	int getY() const { return y; }
	Player getPlayer() const { return occupant; }
	bool operator==(const Square &other) const { return (x == other.x && y == other.y); }
	const Square &operator=(const Square &other);
};

class PieceStrip : public std::array<Player, 4 + 1 + 4>
{
public:
	PieceStrip() {}
	~PieceStrip() {}
	void setPlayer(size_t index, Player player) { at(index) = player; }
};

class Board
{
public:
	enum
	{
		SideLen = 15
	};

private:
	std::array<Square, SideLen * SideLen> squares;

	static const int Directions[4][2];

	bool draw() const;

public:
	Board();
	Board(const Board &other) : squares(other.squares) {}
	~Board() {}

	const Board &operator=(const Board &other);

	Square &getSquare(int x, int y) { return squares.at(x + y * SideLen); }
	const Square &getSquare(int x, int y) const { return squares.at(x + y * SideLen); }
	bool coordValid(int x, int y) const { return (x >= 0 && x < SideLen && y >= 0 && y < SideLen); }
	bool squareOccupied(int x, int y) const { return (getSquare(x, y).getPlayer() != Nobody); }

	inline int numSquaresOccupiedBy(Player) const;
	inline Player getCurrentPlayer() const;
	std::array<PieceStrip, 4 /* Num of directions */> getSurroundingPieces(int x, int y) const;
	bool hasOccupiedSquaresNearby(int x, int y) const;

	/**
	 * Return 'r' if game is not over and still Running;
	 * 'x' if X has won;
	 * 'o' if O has won;
	 * 'd' if a draw.
	 */
	char gameStatus() const;

	/**
	 * Reset all squares to unoccupied.
	 */
	void clear();
};

class BoardAnalyser
{
private:
	Player evaluatedPlayer;
	std::array<PieceStrip, 4 /* Num of directions */> analysedStrips;

	static const std::array<const std::string, 22> Patterns;
	int scoreOfPattern(size_t patternSubscript) const;

	/** Return score for match */
	int getScoreOfStrip(const PieceStrip &) const;

public:
	BoardAnalyser(const Board &analysedBoard, int x, int y, Player analysedPlayer);
	~BoardAnalyser() {}

	int analysisResult() const;
};

/**
 * This class is for implementing the minimax algorithm,
 * which can be defined as follows:
 *
 * Minimax(s, p, depth) =
 * 		Utility(s, p) if Terminal(s) or depth = 1
 * 		max(Minimax(Result(s, a), p, depth - 1)) for each a in Actions(s) if Player(s) = p
 * 		min(Minimax(Result(s, a), p, depth - 1)) for each a in Actions(s) if Player(s) is not p
 */
class GameState
{
public:
	struct Coord
	{
		int x;
		int y;

		Coord(int p = 0, int q = 0) : x(p), y(q) {}
	};

private:
	Board board;

	bool terminal() const { return board.gameStatus() != 'r'; }

	int boardAnalysisResult(Player) const;

	/**
	 * Takes this state and analysed player
	 * Return infty or -infty if state is terminal,
	 * Or return the sum of every unoccupied square's analysis result.
	 */
	int utility(Player) const;

	/**
	 * Takes the current state and a particular move (Coord)
	 * Returns the resulting state.
	 */
	GameState result(const Coord &move) const;

	/** Returns a vector of legal moves in this state,
	 * i.e. Unoccupied Squares
	 * 
	 * I am going to optimise this method
	 * by only returning those coordinates with occupied squares nearby,
	 * that is, if an unoccupied square does not have a piece within two squares
	 * in all four directions, then this would not be included in the returned vector.
	 * This calls for a helper function.
	 */
	std::vector<Coord> actions() const;

	// These two below are for alpha-beta pruning
	int maxValue(int alpha, int beta, Player, int depth) const;
	int minValue(int alpla, int beta, Player, int depth) const;

public:
	GameState(const Board &b) : board(b) {}
	~GameState() {}

	int minimax(Player, int depth) const;
	inline int alphaBetaSearch(Player, int depth) const;
};

class Game
{
private:
	Board board;
	std::vector<Square> occupiedSquares;

	Player currentPlayer;

	std::array<int, Board::SideLen * Board::SideLen> xScores;
	std::array<int, Board::SideLen * Board::SideLen> oScores;
	long xScoreSum;
	long oScoreSum;
	std::vector<std::array<int, 2>> xMaxLocations;
	std::vector<std::array<int, 2>> oMaxLocations;

	int aiDepth;

	int getXScoreOfSquare(int x, int y) const { return xScores.at(x + y * Board::SideLen); }
	int getXScoreOfSquare(const std::array<int, 2> &location) { return getXScoreOfSquare(location.at(0), location.at(1)); }
	int &getXScoreOfSquare(int x, int y) { return xScores.at(x + y * Board::SideLen); }
	int getOScoreOfSquare(int x, int y) const { return oScores.at(x + y * Board::SideLen); }
	int getOScoreOfSquare(const std::array<int, 2> &location) { return getOScoreOfSquare(location.at(0), location.at(1)); }
	int &getOScoreOfSquare(int x, int y) { return oScores.at(x + y * Board::SideLen); }

	void updateScoreOfSquare(int x, int y, Player player);
	void evaluateBoard();

	bool placePiece(int x, int y);

public:
	Game() : currentPlayer(X), xScores({0}), oScores({0}), xScoreSum(0L), oScoreSum(0L), aiDepth(3) {}

	Player getCurrentPlayer() const { return currentPlayer; }
	bool makeMove(int x, int y) { return placePiece(x, y); }
	bool autoMove();
	void undo();
	void restart();

	/**
	 * Return 'r' if game is not over and still Running;
	 * 'x' if X has won;
	 * 'o' if O has won;
	 * 'd' if a draw.
	 */
	char gameStatus() const { return board.gameStatus(); }

	/** Please note that for some mysterious reason, x and y are reversed! */
	void printScores() const;

	const Square &getLastestMovedSquare() const { return *occupiedSquares.crbegin(); }
	const std::vector<Square> &getPlacedPiecesList() const { return occupiedSquares; }
};

#endif
