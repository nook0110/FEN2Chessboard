#include "Piece.h"

#include <iostream>

std::vector<sf::Texture> Piece::textures;
std::vector<sf::Sprite> Piece::sprites;


bool PieceBoard::checkCheck(const std::vector<std::vector<Piece*>>& board, bool isWhite, const _info& info)
{
	Piece* king = nullptr;
	for (int column = 1; column <= 8 && !king; ++column)
	{
		for (int row = 1; row <= 8 && !king; ++row)
		{
			if (dynamic_cast<King*>(board[column][row]) && board[column][row]->isWhite() == isWhite)
			{
				king = board[column][row];
			}
		}
	}
	for (const auto& row : board)
	{
		for (const auto& piece : row)
		{
			if (piece && piece->isWhite() != isWhite)
			{
				if (piece->canMoveTo(king->getPosition(), board, info))
					return true;
			}
		}
	}
	return false;
}

PieceBoard::PieceBoard(std::string FEN) : info(std::string())
{
	std::vector<std::vector<Piece*>> position(9, std::vector<Piece*>(9));
	auto current = FEN.begin();
	for (int row = 8; row >= 1; --row, ++current)
	{
		for (int column = 1; current != FEN.end() && *current != '/' && column <= 8; ++current)
		{
			try
			{
				position[column][row] = Piece::getPiece(*current, sf::Vector2i(column, row));
			}
			catch (std::string excep)
			{
				column += *current - '1';
			}

			++column;
		}
		if (current == FEN.end())
		{
			break;
		}
	}
	board = position;
	info = _info(std::string(FEN.substr(current - FEN.begin())));
}

void PieceBoard::move(std::string move)
{
	// info = "  w -- 0 1
	//           0123456789
	sf::Vector2i pos = { move[0] - 'a' + 1 , move[1] - '0' };
	sf::Vector2i end = { move[3] - 'a' + 1 , move[4] - '0' };
	if (!board[pos.x][pos.y])
		throw (std::string)"No piece to move";
	if (board[pos.x][pos.y]->isWhite() != (info[_info::color] == 'w'))
		throw (std::string)"No piece to move";
	if (!board[pos.x][pos.y]->canMoveTo(end, board, info))
		throw (std::string)"This piece cant move like this";
	bool isWhite = info[_info::color] == 'w';

	info[_info::enPassantColumn] = '-';
	info[_info::enPassantRow] = '-';

	if (dynamic_cast<Pawn*>(board[pos.x][pos.y]))
	{
		auto delta = end - pos;
		if (abs(delta.y) == 2)
		{
			info[_info::enPassantColumn] = pos.x + 'a' - 1;
			info[_info::enPassantRow] = pos.y + delta.y / 2 + '0';
		}
	}	

	if (dynamic_cast<Pawn*>(board[pos.x][pos.y]) && pos.x != end.x)
	{
		board[pos.x][pos.y]->setPosition(end);

		if (dynamic_cast<Pawn*>(board[pos.x][pos.y])->isWhite())
		{
			if (!board[end.x][end.y])
			{
				delete board[end.x][5];
				board[end.x][5] = nullptr;
			}
		}
		else
		{
			if (!board[end.x][end.y])
			{
				delete board[end.x][4];
				board[end.x][4] = nullptr;
			}
		}
		if (board[end.x][end.y])
		{
			delete board[end.x][end.y];
			board[end.x][end.y] = nullptr;
		}
		board[end.x][end.y] = board[pos.x][pos.y];
		board[pos.x][pos.y] = nullptr;
	}
	else
	{
		board[pos.x][pos.y]->setPosition(end);
		if (board[end.x][end.y])
		{
			delete board[end.x][end.y];
			board[end.x][end.y] = nullptr;
		}
		board[end.x][end.y] = board[pos.x][pos.y];
		board[pos.x][pos.y] = nullptr;
	}


	if (checkCheck(board, isWhite, info))
		throw (std::string)"You are under check";

	if ((end.y == 1 || end.y == 8))
	{
		if (dynamic_cast<Pawn*>(board[end.x][end.y]))
		{
			delete board[end.x][end.y];
			if (move.size() < 7)
			{
				move.resize(7);
				std::cin >> move[6];
			}
			try
			{
				board[end.x][end.y] = Piece::getPiece(move[6], end);
			}
			catch (std::string error)
			{
				throw (std::string)"Wrong promotion";
			}
			if (dynamic_cast<Pawn*>(board[end.x][end.y])
				|| dynamic_cast<King*>(board[end.x][end.y]))
			{
				throw (std::string)"Wrong promotion";
			}
		}
	}

	info.changeColor();

}

void PieceBoard::draw()
{
	for (int column = 1; column <= 8; ++column)
	{
		for (int row = 1; row <= 8; ++row)
		{
			if (board[column][row])
				board[column][row]->draw();
		}
	}
}

std::string PieceBoard::getFEN()
{
	std::string FEN;
	for (int row = 8; row >= 1; --row)
	{
		int freeSq = 0;
		for (int column = 1; column <= 8; ++column)
		{
			if (board[column][row])
			{
				if (freeSq != 0)
				{
					FEN += freeSq + '0';
				}
				freeSq = 0;
				FEN += board[column][row]->getChar();
			}
			else
			{
				++freeSq;
			}
		}
		if (freeSq != 0)
		{
			FEN += freeSq + '0';
		}
		if (row != 1)
			FEN += '/';
	}
	FEN += " " + info.info;
	return FEN;
}


void Piece::initialize()
{
	if (!sprites.empty())
		return;
	sf::Texture texture;
	texture.loadFromFile("Pieces\\White\\wk.png");
	textures.push_back(texture);
	texture.loadFromFile("Pieces\\White\\wq.png");
	textures.push_back(texture);
	texture.loadFromFile("Pieces\\White\\wr.png");
	textures.push_back(texture);
	texture.loadFromFile("Pieces\\White\\wb.png");
	textures.push_back(texture);
	texture.loadFromFile("Pieces\\White\\wn.png");
	textures.push_back(texture);
	texture.loadFromFile("Pieces\\White\\wp.png");
	textures.push_back(texture);
	texture.loadFromFile("Pieces\\Black\\bk.png");
	textures.push_back(texture);
	texture.loadFromFile("Pieces\\Black\\bq.png");
	textures.push_back(texture);
	texture.loadFromFile("Pieces\\Black\\br.png");
	textures.push_back(texture);
	texture.loadFromFile("Pieces\\Black\\bb.png");
	textures.push_back(texture);
	texture.loadFromFile("Pieces\\Black\\bn.png");
	textures.push_back(texture);
	texture.loadFromFile("Pieces\\Black\\bp.png");
	textures.push_back(texture);
	sf::Vector2f pieceSize = size / 8.f;
	for (auto& texture : textures)
	{
		texture.setSmooth(true);
		sprites.push_back(sf::Sprite(texture));
		auto bounds = sprites.back().getGlobalBounds();
		sprites.back().setScale(pieceSize.x / bounds.width, pieceSize.y / bounds.height);
	}
}

Piece::Type Piece::getType(char name)
{
	switch (name)
	{
	case 'K':
		return Type::K;
		break;
	case 'Q':
		return Type::Q;
		break;
	case 'R':
		return Type::R;
		break;
	case 'B':
		return Type::B;
		break;
	case 'N':
		return Type::N;
		break;
	case 'P':
		return Type::P;
		break;
	case 'k':
		return Type::k;
		break;
	case 'q':
		return Type::q;
		break;
	case 'r':
		return Type::r;
		break;
	case 'b':
		return Type::b;
		break;
	case 'n':
		return Type::n;
		break;
	case 'p':
		return Type::p;
		break;
	default:
		throw std::string("Wrong piece");
		break;
	}
}

char Piece::getChar()
{
	switch (type)
	{
	case Piece::K:
		return 'K';
		break;
	case Piece::Q:
		return 'Q';
		break;
	case Piece::R:
		return 'R';
		break;
	case Piece::B:
		return 'B';
		break;
	case Piece::N:
		return 'N';
		break;
	case Piece::P:
		return 'P';
		break;
	case Piece::k:
		return 'k';
		break;
	case Piece::q:
		return 'q';
		break;
	case Piece::r:
		return 'r';
		break;
	case Piece::b:
		return 'b';
		break;
	case Piece::n:
		return 'n';
		break;
	case Piece::p:
		return 'p';
		break;
	default:
		break;
	}
}

void Piece::draw() const
{
	sprites[type].setPosition((position.x - 1) * size.x / 8.f, (8 - position.y) * size.y / 8.f);
	window.draw(sprites[type]);
}

Piece* Piece::getPiece(char type, const sf::Vector2i& position)
{
	switch (getType(type))
	{
	case Piece::K:
	case Piece::k:
		return new King(type, position);
		break;
	case Piece::Q:
	case Piece::q:
		return new Queen(type, position);
		break;
	case Piece::R:
	case Piece::r:
		return new Rook(type, position);
		break;
	case Piece::B:
	case Piece::b:
		return new Bishop(type, position);
		break;
	case Piece::N:
	case Piece::n:
		return new Knight(type, position);
		break;
	case Piece::P:
	case Piece::p:
		return new Pawn(type, position);
		break;
	default:
		throw std::string("Wrong type");
		break;
	}
}

bool King::canBeat(const sf::Vector2i& square) const
{
	auto delta = position - square;
	return abs(delta.x) <= 1 && abs(delta.y) <= 1;
}

bool King::canMoveTo(sf::Vector2i square, const std::vector<std::vector<Piece*>>& board, const PieceBoard::_info& info) const
{
	if (!canBeat(square))
		return false;
	if (board[square.x][square.y])
		if (board[square.x][square.y]->isWhite() == isWhite())
			return false;
	return true;
}


bool Queen::canBeat(const sf::Vector2i& square) const
{
	if (square.x == position.x || square.y == position.y)
		return true;
	if (square.x - square.y == position.x - position.y || square.x + square.y == position.x + position.y)
		return true;
	return false;
}

bool Queen::canMoveTo(sf::Vector2i square, const std::vector<std::vector<Piece*>>& board, const PieceBoard::_info& info) const
{
	if (!canBeat(square))
		return false;
	if (board[square.x][square.y])
		if (board[square.x][square.y]->isWhite() == isWhite())
			return false;
	auto delta = square - position;
	int max = std::max(abs(delta.x), abs(delta.y));
	delta /= max;
	for (int i = 1; i < max; ++i)
	{
		if (board[position.x + delta.x * i][position.y + delta.y * i])
			return false;
	}
	return true;
}

bool Rook::canBeat(const sf::Vector2i& square) const
{
	if (square.x == position.x || square.y == position.y)
		return true;
	return false;
}

bool Rook::canMoveTo(sf::Vector2i square, const std::vector<std::vector<Piece*>>& board, const PieceBoard::_info& info) const
{
	if (!canBeat(square))
		return false;
	if (board[square.x][square.y])
		if (board[square.x][square.y]->isWhite() == isWhite())
			return false;
	auto delta = square - position;
	int max = std::max(abs(delta.x), abs(delta.y));
	delta /= max;
	for (int i = 1; i < max; ++i)
	{
		if (board[position.x + delta.x * i][position.y + delta.y * i])
			return false;
	}
	return true;
}

bool Bishop::canBeat(const sf::Vector2i& square) const
{
	if (square.x - square.y == position.x - position.y || square.x + square.y == position.x + position.y)
		return true;
	return false;
}

bool Bishop::canMoveTo(sf::Vector2i square, const std::vector<std::vector<Piece*>>& board, const PieceBoard::_info& info) const
{
	if (!canBeat(square))
		return false;
	if (board[square.x][square.y])
		if (board[square.x][square.y]->isWhite() == isWhite())
			return false;
	auto delta = square - position;
	int max = std::max(abs(delta.x), abs(delta.y));
	delta /= max;
	for (int i = 1; i < max; ++i)
	{
		if (board[position.x + delta.x * i][position.y + delta.y * i])
			return false;
	}
	return true;
}


bool Knight::canBeat(const sf::Vector2i& square) const
{
	const std::vector<sf::Vector2i> knightMoves = { sf::Vector2i(1,2), sf::Vector2i(-1,2), sf::Vector2i(1,-2), sf::Vector2i(-1,-2),
		sf::Vector2i(2,1), sf::Vector2i(-2,1), sf::Vector2i(2,-1) ,sf::Vector2i(-2,-1) };
	auto delta = position - square;
	for (const auto& move : knightMoves)
	{
		if (delta == move)
			return true;
	}
	return false;
}

bool Knight::canMoveTo(sf::Vector2i square, const std::vector<std::vector<Piece*>>& board, const PieceBoard::_info& info) const
{
	if (!canBeat(square))
		return false;
	if (board[square.x][square.y])
		if (board[square.x][square.y]->isWhite() == isWhite())
			return false;
	return true;
}

bool Pawn::canBeat(const sf::Vector2i& square) const
{
	auto delta = square - position;
	const std::vector<sf::Vector2i> pawnMoves = { sf::Vector2i(1,1), sf::Vector2i(-1,1) };
	if (type == Piece::P)
	{
		for (const auto& move : pawnMoves)
		{
			if (delta == move)
				return true;
		}
	}
	else
	{
		for (const auto& move : pawnMoves)
		{
			if (delta == -move)
				return true;
		}
	}
	return false;
}

bool Pawn::canMoveTo(sf::Vector2i square, const std::vector<std::vector<Piece*>>& board, const PieceBoard::_info& info) const
{
	if (!canBeat(square))
	{
		if (square.x != position.x)
		{
			return false;
		}
		if (type == Piece::P)
		{
			if (position.y == 2)
			{
				if (square.y != 3 && square.y != 4)
					return false;
			}
			else
			{
				if (square.y != position.y + 1)
					return false;
			}
		}
		else
		{
			if (position.y == 7)
			{
				if (square.y != 6 && square.y != 5)
					return false;
			}
			else
			{
				if (square.y != position.y - 1)
					return false;
			}
		}
		auto delta = square - position;
		delta /= std::max(abs(delta.x), abs(delta.y));
		for (int i = 1; i <= std::max(abs(delta.x), abs(delta.y)); ++i)
		{
			if (board[position.x + delta.x * i][position.y + delta.y * i])
				return false;
		}
	}
	else
	{
		if (board[square.x][square.y])
			if (board[square.x][square.y]->isWhite() == isWhite())
				return false;
		if (!board[square.x][square.y])
		{
			if (info[PieceBoard::_info::enPassantColumn] == '-')
				return false;
			int column = info[PieceBoard::_info::enPassantColumn] - 'a' + 1;
			int row = info[PieceBoard::_info::enPassantRow] - '0';
			if (square != sf::Vector2i(column, row))
				return false;
		}
	}
	return true;
}

