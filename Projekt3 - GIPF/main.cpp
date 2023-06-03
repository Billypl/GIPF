#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Point.h"

using namespace std;

struct ORGINAL
{
	const int sideSize = 4;
	const int triggerCollectionNumber = 4;
	const int whitePieces = 15;
	const int blackPiecies = 15;
	const int blackPieciesReserve = 12;
	const int whitePieciesReserve = 12;
	const char startingPlayer = 'W';
};

class Board
{


public:

	int sideSize = 5; 
	int totalRowsCount = (sideSize - 1) * 2 + 1;
	vector<vector<char>> board;
	map<string, point> namedBoard;

	
	struct Moves
	{
		point UPPER_LEFT	= { 0, -1 };		
		point UPPER_RIGHT	= { 1, -1 };		
		point RIGHT			= { 1, 0 };				
		point BOTTOM_RIGHT	= { 0, 1 };		
		point BOTTOM_LEFT	= { -1, 1 };		
		point LEFT			= { -1, 0 };				
	} MOVES;

	Board() : board(totalRowsCount, vector<char>(totalRowsCount)) {};
	// sideSize >= 2
	Board(int sideSize);
	char& get();

	void populateNamedBoard()
	{
		char fieldLetter = 'b';
		int rowExpansion = 0;
		point fieldCoords;

		for (int i = 0; i < totalRowsCount; i++)
		{
			if (i < sideSize)
				fieldCoords = { 0, sideSize + i - 1};
			else
				fieldCoords = { i - sideSize + 1, (sideSize - 2) * 2 + 2 };
			
			int fieldNumber = 2;
			for (int j = 0; j < sideSize + rowExpansion; j++)
			{
				string fieldName = fieldLetter + to_string(fieldNumber);
				namedBoard[fieldName] = fieldCoords;
				fieldCoords += {1, -1};
				fieldNumber++;
			}
			
			// fixes rows expanding in the middle
			if (i < sideSize - 1)
				rowExpansion++;
			else
				rowExpansion--;

			fieldLetter++;
		}

		for (pair<string, point> p : namedBoard)
		{
			point field = p.second;
			board[field.x][field.y] = 'W';
		}

	}

	void printBoard()
	{
		for (int i = 0; i < totalRowsCount; i++)
		{
			if (i > sideSize - 1)
			{
				for (int k = 0; k < i - sideSize + 1; k++)
				{
					cout << " ";
				}
			}
			for (int j = 0; j < totalRowsCount; j++)
			{
				cout << board[i][j] << " ";
			}
			cout << endl;
		}
	}


};

class Game
{

	Board board;

	int triggerCollectionNumber;
	int whitePieces;
	int blackPiecies;


	// sideSize >= 2
	// 2 < triggerCollectionNumber < 2*sideSize - 1
	// white/black Piecies > 3	   
	// | white - black | <= 3
	// S K GW GB
	Game(int sideSize, int triggerCollectionNumber, int whitePieces, int blackPiecies);

};


int main()
{
	Board b;
	b.populateNamedBoard();
	point a = b.namedBoard["c3"];
	//(a + b.MOVES.BOTTOM_RIGHT).print();
	b.printBoard();
}