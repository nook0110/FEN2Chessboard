#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include <stdio.h>
#include <tchar.h>
#include "Piece.h"

#include <random>
#include <fstream>
#include <iostream>
std::string generateChess960();

class Player
{
	sf::Time timeLeft = sf::Time(sf::seconds(180));
	sf::Clock timeForMove;
	std::string path;

public:
	Player(std::string path) : path(path) {};
	void doMove(std::string in, std::string out);
};

class Game
{
	PieceBoard board = PieceBoard(generateChess960());
	Player first, second;
	int move = 1;

public:
	Game(std::string firstPath, std::string secondPath) : first(Player(firstPath)), second(Player(secondPath))
	{}
	void draw()
	{
		board.draw();
	}
	void makeMove()
	{
		std::string in = "Imove" + std::to_string(move) + ".txt";
		std::string out = "Omove" + std::to_string(move) + ".txt";
		std::ofstream moveFile(in);

		moveFile << board.getFEN() + " " + std::to_string(move);
		if (move != 1)
		{
			std::ifstream lastMoveFile("Omove" + std::to_string(move - 1) + ".txt");
			std::string lastMove;
			std::getline(lastMoveFile, lastMove);
			moveFile << std::endl << lastMove;
		}
		(move % 2 ? first : second).doMove(in, out);
		std::ifstream currentMoveFile(out);
		std::string currentMove;
		std::getline(currentMoveFile, currentMove);
		try
		{
			board.move(currentMove);
		}
		catch (std::string exc)
		{
			std::cout << exc << std::endl;
			std::cout << (move % 2 ? "First Player" : "Second Player");
		}
	}
};
