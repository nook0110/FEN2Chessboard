#include "Piece.h"
#include <random>
sf::Vector2f size(800, 800);
sf::RenderWindow window(sf::VideoMode(size.x, size.y), "Chess");

#include <iostream>

std::string generateChess960()
{
	std::string FENBlack = "--------";
	std::string FENWhite = "--------";
	std::string middle = "/pppppppp/8/8/8/8/PPPPPPPP/";
	std::string info = " w -- 0 1";
	std::mt19937 mt(time(nullptr));
	std::uniform_int_distribution <int> bishop(0, 3);
	unsigned int firstBishop = bishop(mt) * 2;
	unsigned int secondBishop = bishop(mt) * 2 + 1;
	FENBlack[firstBishop] = 'b';
	FENBlack[secondBishop] = 'b';
	FENWhite[firstBishop] = 'B';
	FENWhite[secondBishop] = 'B';
	std::uniform_int_distribution <int> queen(0, 5);
	unsigned int posQueen = queen(mt);
	for (int pos = 0; pos < 8; ++pos)
	{
		if (FENBlack[pos] != '-' && pos <= posQueen)
		{
			++posQueen;
		}
	}
	FENBlack[posQueen] = 'q';
	FENWhite[posQueen] = 'Q';
	std::uniform_int_distribution <int> knight1(0, 3);
	std::uniform_int_distribution <int> knight2(0, 2);
	unsigned int firstKnight = knight1(mt);
	unsigned int secondKnight = knight2(mt);
	for (int pos = 0; pos < 8; ++pos)
	{
		if (FENBlack[pos] != '-' && pos <= firstKnight)
		{
			++firstKnight;
		}
	}
	FENBlack[firstKnight] = 'n';
	FENWhite[firstKnight] = 'N';
	for (int pos = 0; pos < 8; ++pos)
	{
		if (FENBlack[pos] != '-' && pos <= secondKnight)
		{
			++secondKnight;
		}
	}
	FENBlack[secondKnight] = 'n';
	FENWhite[secondKnight] = 'N';
	for (int pos = 0, freeSq = 0; pos < 8; ++pos)
	{
		if (FENBlack[pos] == '-')
		{
			if (freeSq != 1)
			{
				FENBlack[pos] = 'r';
				FENWhite[pos] = 'R';
			}
			else
			{
				FENBlack[pos] = 'k';
				FENWhite[pos] = 'K';
			}
			++freeSq;
		}
	}

	return FENBlack + middle + FENWhite + info;
}

struct Board
{
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f size;
	sf::RenderWindow& window;

	Board(sf::RenderWindow& window, const std::string& path, sf::Vector2f size) : size(size), window(window)
	{
		texture.loadFromFile(path);
		sprite.setTexture(texture);
		sprite.setScale(size.x / sprite.getGlobalBounds().width, size.y / sprite.getGlobalBounds().height);
	}
	void draw()
	{
		window.draw(sprite);
	}
};

int main()
{
	Piece::initialize();
	PieceBoard board(generateChess960());
	std::cout << board.getFEN() << std::endl;
	Board background(window, "board.png", size);

	std::string move;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
			{
				exit(0);
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				auto pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				int column = pos.x / (size.x / 8.f);
				int row = 8 - pos.y / (size.y / 8.f);

				move += (char)(column + 'a');
				move += (char)(row + '1');
				move += ' ';
			}
		}
		window.clear(sf::Color::White);
		background.draw();
		board.draw();

		window.display();
		window.setVisible(1);

		if (move.size() == 6)
		{
			std::cout << move << std::endl;
			try
			{
				board.move(move);
			}
			catch (std::string error)
			{
				std::cout << error << std::endl;
			}
			move.clear();
			std::cout << board.getFEN() << std::endl;

		}
	}
}