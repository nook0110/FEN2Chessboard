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
	sf::Time timeLeft = sf::Time(sf::seconds(1000000));
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
	const std::string globPath = "Moves\\";
public:
	Game(std::string firstPath, std::string secondPath) : first(Player(firstPath)), second(Player(secondPath))
	{}
	void draw()
	{
		board.draw();
	}
	void makeMove()
	{
		std::string in = globPath + "Imove" + std::to_string(move) + ".txt";
		std::string out = globPath + "Omove" + std::to_string(move) + ".txt";
		std::ofstream moveFile(in, std::ios::trunc);
		moveFile << board.getFEN() + " " + std::to_string(move);
		if (move != 1)
		{
			std::ifstream lastMoveFile(globPath + "Omove" + std::to_string(move - 1) + ".txt");
			std::string lastMove;
			std::getline(lastMoveFile, lastMove);
			moveFile << std::endl << lastMove;
		}
		moveFile.close();
		std::ofstream create(out, std::ios::trunc);
		create.close();
		std::ifstream currentMoveFile(out);
		(move % 2 ? first : second).doMove(in, out);
		std::string currentMove;
		std::getline(currentMoveFile, currentMove);
		try
		{
			board.move(currentMove);
		}
		catch (std::string exc)
		{
			throw(exc + (move % 2 ? " by First Player" : " by Second Player") + "\n Move was:" + currentMove);
		}
		++move;
	}
};
