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

	bool draw() const;

public:
	Board();
	~Board() {}

	const Board &operator=(const Board &other);

	Square &getSquare(int x, int y) { return squares.at(x + y * SideLen); }
	const Square &getSquare(int x, int y) const { return squares.at(x + y * SideLen); }
	bool coordValid(int x, int y) const { return (x >= 0 && x < SideLen && y >= 0 && y < SideLen); }
	bool squareOccupied(int x, int y) { return (getSquare(x, y).getPlayer() != Nobody); }

	std::array<PieceStrip, 4 /* Num of directions */> getSurroundingPieces(int x, int y) const;

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
	BoardAnalyser(const Board & analysedBoard, int x, int y, Player analysedPlayer);
	~BoardAnalyser() {}

	int analysisResult() const;
};

class Game
{
private:
	Board board;
	std::vector<Square> occupiedSquares;

	Player currentPlayer;

	bool placePiece(int x, int y);

public:
	Game() : currentPlayer(X) {}

	Player getCurrentPlayer() const { return currentPlayer; }
	bool makeMove(int x, int y) { return placePiece(x, y); }
	void undo();
	void restart();

	/**
	 * Return 'r' if game is not over and still Running;
	 * 'x' if X has won;
	 * 'o' if O has won;
	 * 'd' if a draw.
	 */
	char gameStatus() const { return board.gameStatus(); }

	const std::vector<Square> &getPlacedPiecesList() const { return occupiedSquares; }
};

#endif
