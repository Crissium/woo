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

inline Player adversaryOf(const Player);

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
	bool hasAWinningConnection() const;
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
	std::vector<Square> occupiedSquares;

public:
	Square *mostRecentlyModifiedSquare;

private:
	static const int Directions[4][2];

	inline bool draw() const;

public:
	Board();
	Board(const Board &other);
	Board(const Board &other, int moveX, int moveY);
	~Board() {}

	const Board &operator=(const Board &other);

	Square &getSquare(int x, int y) { return squares.at(x + y * SideLen); }
	const Square &getSquare(int x, int y) const { return squares.at(x + y * SideLen); }
	const Square & getSquare(size_t moveIndex) const { return occupiedSquares.at(moveIndex); }
	bool coordValid(int x, int y) const { return (x >= 0 && x < SideLen && y >= 0 && y < SideLen); }
	bool squareOccupied(int x, int y) const { return (getSquare(x, y).getPlayer() != Nobody); }

	inline size_t numSquareOccupied() const {return occupiedSquares.size();}
	inline int numSquaresOccupiedBy(Player) const;
	inline Player getCurrentPlayer() const;
	std::array<PieceStrip, 4 /* Num of directions */> getSurroundingPieces(int x, int y) const;
	bool hasOccupiedSquaresNearby(int x, int y) const;

	void makeMove(int x, int y);
	void undo();

	/**
	 * Return 'r' if game is not over and still Running;
	 * 'x' if X has won;
	 * 'o' if O has won;
	 * 'd' if a draw.
	 */
	char gameStatus() const;
	inline bool terminatingMove(int x, int y) const;

	/**
	 * Reset all squares to unoccupied.
	 */
	void clear();
};

class MoveAnalyser
{
private:
	Player evaluatedPlayer;
	std::array<PieceStrip, 4 /* Num of directions */> analysedStrips;

	static const std::array<const std::string, 22> Patterns;
	int scoreOfPattern(size_t patternSubscript) const;

	/** Return score for match */
	int getScoreOfStrip(const PieceStrip &) const;

public:
	/**
	 * This constructor does not modify the square's occupant
	 */
	inline MoveAnalyser(const Board &analysedBoard, int x, int y);

	/**
	 * This constructor 'changes' the square's occupant
	 */
	MoveAnalyser(const Board &analysedBoard, int x, int y, Player analysedPlayer);

	~MoveAnalyser() {}

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
private:
	struct Coord
	{
		int x;
		int y;

		Coord(int p = 0, int q = 0) : x(p), y(q) {}
	};

	Board board;

	bool terminal() const { return board.gameStatus() != 'r'; }

	int recentMovesAnalysisResult(Player) const;

	int utility(Player) const;

	/**
	 * Takes the current state and a particular move (Coord)
	 * Returns the resulting state.
	 */
	inline GameState result(const Coord &move) const;

	/** 
	 * Returns a vector of legal moves in this state,
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
	GameState(const Board &b, int moveX, int moveY);
	~GameState() {}

	static size_t numMovesMadeSoFarWhenCalled;

	int minimax(Player, int depth) const;
	int alphaBetaAnalysis(Player, int depth) const;
};

class Game
{
private:
	Board board;

	Player currentPlayer;

	int aiDepth;

	bool placePiece(int x, int y);

public:
	Game() : currentPlayer(X), aiDepth(4) {}

	Player getCurrentPlayer() const { return currentPlayer; }
	bool makeMove(int x, int y) { return placePiece(x, y); }
	bool autoMove();
	inline void undo() {board.undo();}
	void restart();

	void setDepth(int depth) {aiDepth = depth;}

	/**
	 * Return 'r' if game is not over and still Running;
	 * 'x' if X has won;
	 * 'o' if O has won;
	 * 'd' if a draw.
	 */
	char gameStatus() const { return board.gameStatus(); }

	const Square &getLastestMovedSquare() const { return *board.mostRecentlyModifiedSquare; }
};

#endif
