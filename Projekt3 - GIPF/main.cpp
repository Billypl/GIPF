#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <math.h>
#include "Point.h"
#include "Helper.h"

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

	const char EMPTY_FIELD = '_';
	const char STARTING_ROW_LETTER = 'b';
	const int STARTING_ROW_NUMBER = 2;

public:

	int sideSize = 3; 
	int getTotalRowsCount()
	{
		return (sideSize - 1) * 2 + 1;
	}
	
	vector<vector<char>> board;
	map<string, point> namedBoard;
	
	struct Field
	{
		point coords;
		char& value;
		Field(point coords, char& value) : coords(coords), value(value) {};
		void print()
		{
			coords.print();
			cout << " " << value;
		}
	};

	Field get(string fieldName)
	{
		if (namedBoard.find(fieldName) != namedBoard.end())
		{
			point coords = namedBoard[fieldName];
			Field field(coords, board[coords.y][coords.x]);
			return field;
		}
		throw "Wrong field name!";
	}
	Field operator[](string field)
	{
		return get(field);
	}

	enum Move
	{
		UPPER_LEFT,		// (0, -1)
		UPPER_RIGHT,	// (1, -1)
		RIGHT,			// (1, 0)
		BOTTOM_RIGHT,	// (0, 1)
		BOTTOM_LEFT,	// (1, 1)
		LEFT			// (-1, 0)
	};

	struct Moves
	{
		point UPPER_LEFT	= { 0, -1 };		
		point UPPER_RIGHT	= { 1, -1 };		
		point RIGHT			= { 1, 0 };				
		point BOTTOM_RIGHT	= { 0, 1 };		
		point BOTTOM_LEFT	= { -1, 1 };		
		point LEFT			= { -1, 0 };				
	} MOVES;

	Field getNeighbour(string fieldName, Move move)
	{
		return getNeighbour(this->get(fieldName), move);
	}

	Field getNeighbour(Field field, Move move)
	{
		point offset;

		switch (move)
		{
		case Board::UPPER_LEFT:
			offset = MOVES.UPPER_LEFT;
			break;
		case Board::UPPER_RIGHT:
			offset = MOVES.UPPER_RIGHT;
			break;
		case Board::RIGHT:
			offset = MOVES.RIGHT;
			break;
		case Board::BOTTOM_RIGHT:
			offset = MOVES.BOTTOM_RIGHT;
			break;
		case Board::BOTTOM_LEFT:
			offset = MOVES.BOTTOM_LEFT;
			break;
		case Board::LEFT:
			offset = MOVES.LEFT;
			break;
		default:
			offset = { -1,-1 };
			break;
		}

		point neighbourCoords = field.coords + offset;
		Field neighbour(neighbourCoords, board[neighbourCoords.y][neighbourCoords.x]);
		return neighbour;
	}

	Board() : board(getTotalRowsCount(), vector<char>(getTotalRowsCount())) {};
	// sideSize >= 2
	Board(int sideSize);

	void populateNamedBoard()
	{
		char fieldLetter = STARTING_ROW_LETTER;
		int rowExpansion = 0;
		point fieldCoords;

		for (int i = 0; i < getTotalRowsCount(); i++)
		{
			if (i < sideSize)
				fieldCoords = { 0, sideSize + i - 1};
			else
				fieldCoords = { i - sideSize + 1, (sideSize - 2) * 2 + 2 };
			
			int fieldNumber = STARTING_ROW_NUMBER;
			for (int j = 0; j < sideSize + rowExpansion; j++)
			{
				string fieldName = fieldLetter + to_string(fieldNumber);
				namedBoard[fieldName] = fieldCoords;
				fieldCoords += {1, -1};
				fieldNumber++;
			}
			// fixes rows expanding in the middle
			rowExpansion = (i < sideSize - 1) ? rowExpansion + 1 : rowExpansion - 1;

			fieldLetter++;
		}

		for (pair<string, point> p : namedBoard)
		{
			point field = p.second;
			board[field.x][field.y] = EMPTY_FIELD;
		}

	}

	void printBoard()
	{
		for (int i = 0; i < getTotalRowsCount(); i++)
		{
			if (i > sideSize - 1)
			{
				for (int k = 0; k < i - sideSize + 1; k++)
				{
					cout << " ";
				}
			}
			for (int j = 0; j < getTotalRowsCount(); j++)
			{
				cout << board[i][j] << " ";
			}
			cout << endl;
		}
	}

	void loadBoard()
	{
		string trash;

		cin >> this->sideSize;
		getline(cin, trash);
		getline(cin, trash);

		/*
			4 4 15 15
			12 12 W
			   W _ _ B
			  _ _ _ _ _
			 _ _ _ _ _ _
			B _ _ _ _ _ W
			 _ _ _ _ _ _
			  _ _ _ _ _
			   W _ _ B

		*/

		vector<vector<string>> boardString;
		int rowExpansion = 0;
		for (int i = 0; i < getTotalRowsCount(); i++)
		{
			string line;
			getline(cin, line);
			line = Helper::trimString(line);
			vector<string> lineFields = Helper::explodeString(line, ' ');

			if (lineFields.size() != this->sideSize + rowExpansion)
			{
				cout << "WRONG_BOARD_ROW_LENGTH" << endl;
				return;
			}
			else
			{
				boardString.push_back(lineFields);
			}
			rowExpansion = (i < sideSize - 1) ? rowExpansion + 1 : rowExpansion - 1;
		}
		

		for (auto it : boardString)
		{
			for (auto itt : it)
			{
				cout << itt << " ";
			}
			cout << endl;
		}


		//point fieldCoords;
		//int rowExpansion = STARTING_ROW_NUMBER;
		//char fieldLetter = STARTING_ROW_LETTER;

		//for (int i = 0; i < getTotalRowsCount(); i++)
		//{
		//	if (i < sideSize)
		//		fieldCoords = { 0, sideSize + i - 1 };
		//	else
		//		fieldCoords = { i - sideSize + 1, (sideSize - 2) * 2 + 2 };

		//	int fieldNumber = 2;
		//	for (int j = 0; j < sideSize + rowExpansion; j++)
		//	{
		//		string fieldName = fieldLetter + to_string(fieldNumber);
		//		namedBoard[fieldName] = fieldCoords;
		//		fieldCoords += {1, -1};
		//		fieldNumber++;
		//	}
		//	// fixes rows expanding in the middle
		//	if (i < sideSize - 1)
		//		rowExpansion++;
		//	else
		//		rowExpansion--;

		//	fieldLetter++;
		//}

		

	}


};

class Game
{

public:

	Board board;

	int triggerCollectionNumber;
	int whitePieces;
	int blackPiecies;

	// sideSize >= 2
	// 2 < triggerCollectionNumber < 2*sideSize - 1
	// white/black Piecies > 3	   
	// | white - black | <= 3
	// S K GW GB
	/*void loadBoard(int sideSize, int triggerCollectionNumber, int whitePieces, int blackPiecies)
	{

	}*/

	

};


int main()
{
	//Board board;
	//board.populateNamedBoard();

	//board["c3"].value = 'X';
	//board["c4"].value = 'Z';
	//board["b2"].value = '2';
	//board["b3"].value = '3';
	//board["b4"].value = '4';
	//board.getNeighbour("c3", Board::Move::UPPER_RIGHT).print();
	//cout << endl;
	//board.printBoard();
	Game game;
	game.board.loadBoard();


}