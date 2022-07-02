#include "positionCompiler.h"

std::vector<std::vector<char>> FENtoVector(std::string FEN)
{
	std::vector<std::vector<char>> position(8, std::vector<char>(8));
	auto current = FEN.begin();
	for (int row = 7; row >= 0; --row, ++current)
	{
		for (int column = 0; current != FEN.end() && *current != '/'; ++current)
		{
			try
			{
				position[column][row] = *current;
			}
			catch (std::string excep)
			{
				column += *current - '0';
			}

			++column;
		}
		if (current == FEN.end())
		{
			break;
		}
	}
	return position;
}

bool checkCheck(std::string FEN)
{
	char kingChar = FEN.back() % 2 ? 'k' : 'K';
	auto position = FENtoVector(FEN);
	sf::Vector2i kingPosition;
	bool isCheck = false;
	for (int row = 1; row <= 8; ++row)
	{
		for (int column = 1; column <= 8; ++column)
		{
			if (position[row][column] == kingChar)
				kingPosition = sf::Vector2i(column, row);
		}
	}
	auto king = Piece::getPiece(kingChar, kingPosition);
	for (int column = kingPosition.x; column > 0; --column)
	{
		auto type = position[kingPosition.y][column];
		if (type != char())
		{
			auto piece = Piece::getPiece(type, sf::Vector2i(column, kingPosition.y));
			if (piece->isBeaten(kingPosition) && king->isWhite() == piece->isWhite())
				isCheck = true;
			delete piece;
		}
	}
	for (int column = kingPosition.x; column <= 8; ++column)
	{
		auto type = position[kingPosition.y][column];
		if (type != char())
		{
			auto piece = Piece::getPiece(type, sf::Vector2i(column, kingPosition.y));
			if (piece->isBeaten(kingPosition) && king->isWhite() == piece->isWhite())
				isCheck = true;
			delete piece;
		}
	}
	for (int row = kingPosition.y; row > 0; --row)
	{
		auto type = position[row][kingPosition.x];
		if (type != char())
		{
			auto piece = Piece::getPiece(type, sf::Vector2i(kingPosition.x, row));
			if (piece->isBeaten(kingPosition) && king->isWhite() == piece->isWhite())
				isCheck = true;
			delete piece;
		}
	}
	for (int row = kingPosition.y; row <= 8; ++row)
	{
		auto type = position[row][kingPosition.x];
		if (type != char())
		{
			auto piece = Piece::getPiece(type, sf::Vector2i(kingPosition.x, row));
			if (piece->isBeaten(kingPosition) && king->isWhite() == piece->isWhite())
				isCheck = true;
			delete piece;
		}
	}
	delete king;
	return isCheck;
}

bool check(std::string lastTurn, std::string currentTurn)
{
	auto lastPosition = FENtoVector(lastTurn);
	auto currentPosition = FENtoVector(lastTurn);

	std::list<std::tuple<int, int, char, char>> difference;
	for (int row = 1; row <= 8; ++row)
	{
		for (int column = 1; column <= 8; ++column)
		{
			if (lastPosition[row][column] != currentPosition[row][column])
			{
				difference.push_back({ row,column,lastPosition[row][column], currentPosition[row][column] });
			}
		}
	}
	return true;
}

void compile(std::string FEN)
{
	Piece::desrtoyPieces();
	auto current = FEN.begin();
	for (int row = 8; row > 0; --row, ++current)
	{
		for (int column = 1; current != FEN.end() && *current != '/'; ++current)
		{
			try
			{
				Piece::pieces.push_back(Piece::getPiece(*current, sf::Vector2i(column, row)));
			}
			catch (std::string excep)
			{
				column += *current - '0';
			}

			++column;
		}
		if (current == FEN.end())
		{
			break;
		}
	}
}