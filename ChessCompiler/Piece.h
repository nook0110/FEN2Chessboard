#pragma once
#include <SFML/Graphics.hpp>
#include <list>
extern sf::Vector2f size;
extern sf::RenderWindow window;

class Piece;

class PieceBoard
{

public:
	struct _info
	{
		_info(std::string _info) : info(_info) {}
		std::string info;
		static const unsigned int color = 0;
		static const unsigned int enPassantColumn = 2;
		static const unsigned int enPassantRow = 3;
		char& operator[] (int i)  { return info[i]; }
		const char& operator[] (int i) const { return info[i]; }
		void changeColor() { info[_info::color] = (info[_info::color] == 'w' ? 'b' : 'w'); }
	};
private:
	_info info;
	std::vector<std::vector<Piece*>> board = std::vector<std::vector<Piece*>>(9, std::vector<Piece*>(9));
	static bool checkCheck(const std::vector<std::vector<Piece*>>& board, bool colorWhite, const _info& info);
public:
	PieceBoard(std::string FEN);
	void move(std::string move);
	void draw();
	std::string getFEN();
};

class Piece
{
protected:
	sf::Vector2i position;
	static std::vector<sf::Texture> textures;
	static std::vector<sf::Sprite> sprites;
public:
	static void initialize();
	enum Type
	{
		K, Q, R, B, N, P,
		k, q, r, b, n, p
	};
	static Type getType(char name);
protected:
	Type type;
	Piece(char type, const sf::Vector2i& position) : type(getType(type)), position(sf::Vector2i(position.x, position.y)) {}
public:
	void setPosition(sf::Vector2i pos) { position = pos; };
	sf::Vector2i getPosition() const { return position; };
	void draw() const;
	bool isWhite() const { return type <= Piece::P; }
	static Piece* getPiece(char type, const sf::Vector2i& position);
	virtual ~Piece() {};
	virtual bool canBeat(const sf::Vector2i& square) const = 0;
	virtual bool canMoveTo(sf::Vector2i square, const std::vector<std::vector<Piece*>>& board, const PieceBoard::_info& info) const = 0;
};

class King : public Piece
{
public:
	King(char type, const sf::Vector2i& position) : Piece(type, position) {};
	bool canBeat(const sf::Vector2i& square) const override;
	bool canMoveTo(sf::Vector2i square, const std::vector<std::vector<Piece*>>& board, const PieceBoard::_info& info) const override;
};

class Queen : public Piece
{
public:
	Queen(char type, const sf::Vector2i& position) : Piece(type, position) {};
	bool canBeat(const sf::Vector2i& square) const override;
	bool canMoveTo(sf::Vector2i square, const std::vector<std::vector<Piece*>>& board, const PieceBoard::_info& info) const override;
};

class Rook : public Piece
{
public:
	Rook(char type, const sf::Vector2i& position) : Piece(type, position) {};
	bool canBeat(const sf::Vector2i& square) const override;
	bool canMoveTo(sf::Vector2i square, const std::vector<std::vector<Piece*>>& board, const PieceBoard::_info& info) const override;
};

class Bishop : public Piece
{
public:
	Bishop(char type, const sf::Vector2i& position) : Piece(type, position) {};
	bool canBeat(const sf::Vector2i& square) const override;
	bool canMoveTo(sf::Vector2i square, const std::vector<std::vector<Piece*>>& board, const PieceBoard::_info& info) const override;
};

class Knight : public Piece
{
public:
	Knight(char type, const sf::Vector2i& position) : Piece(type, position) {};
	bool canBeat(const sf::Vector2i& square) const override;
	bool canMoveTo(sf::Vector2i square, const std::vector<std::vector<Piece*>>& board, const PieceBoard::_info& info) const override;
};

class Pawn : public Piece
{
public:
	Pawn(char type, const sf::Vector2i& position) : Piece(type, position) {};
	bool canBeat(const sf::Vector2i& square) const override;
	bool canMoveTo(sf::Vector2i square, const std::vector<std::vector<Piece*>>& board, const PieceBoard::_info& info) const override;
};