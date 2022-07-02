#include "Piece.h"

sf::Vector2f size(800, 800);
sf::RenderWindow window(sf::VideoMode(size.x, size.y), "Chess");

#include <iostream>

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
	PieceBoard board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w -- 0 1");
	
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
		}
	}
}