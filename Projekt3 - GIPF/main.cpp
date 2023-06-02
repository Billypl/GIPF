#include <iostream>
#include <vector>
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

	vector<vector<char>> board;

public:

	int sideSize; 
	
	//enum move
	//{
	//	UPPER_LEFT,		// (0, -1)
	//	UPPER_RIGHT,	// (1, -1)
	//	RIGHT,			// (1, 0)
	//	BOTTOM_RIGHT,	// (0, 1)
	//	BOTTOM_LEFT,	// (1, 1)
	//	LEFT			// (-1, 0)
	//};

	struct Moves
	{
		point UPPER_LEFT = { 0, -1 };		// (0, -1)
		point UPPER_RIGHT = { 1, -1 };		// (1, -1)
		point RIGHT = { 1, 0 };				// (1, 0)
		point BOTTOM_RIGHT = { 0, 1 };		// (0, 1)
		point BOTTOM_LEFT = { 1, 1 };		// (1, 1)
		point LEFT = { -1, 0 };				// (-1, 0)

	} MOVES;


	Board() {};
	// sideSize >= 2
	Board(int sideSize);
	char& get();


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
	cout << "sada\n";
	Board b;
	cout << b.MOVES.LEFT.x;
}