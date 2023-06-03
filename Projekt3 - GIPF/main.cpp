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
} ORGINAL_BOARD;

class Board
{

	char EMPTY_FIELD = '_';
	char STARTING_ROW_LETTER = 'b';
	int STARTING_ROW_NUMBER = 2;

public:

	// sideSize >= 2
	int sideSize; 
	int getTotalRowsCount()
	{
		return (sideSize - 1) * 2 + 1;
	}
	
	vector<vector<char>> board;
	map<string, point> namedBoard;

	Board() : Board(ORGINAL_BOARD.sideSize) {};
	Board(int sideSize) : sideSize(sideSize), board(getTotalRowsCount(), vector<char>(getTotalRowsCount())) 
	{
		populateNamedBoard();
	};
	
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

	void loadBoard()
	{
		int sideSize; cin >> sideSize;
		Board newBoard(sideSize);

		string trash;
		getline(cin, trash);
		getline(cin, trash);

		vector<vector<string>> boardString = loadFields(newBoard);
		if (boardString.empty())
		{
			cout << "WRONG_BOARD_ROW_LENGTH" << endl;
			return;
		}
		offsetFields(boardString, newBoard);
		board = newBoard;
	}

	vector<vector<string>> loadFields(Board newBoard)
	{
		vector<vector<string>> boardString;
		int rowExpansion = 0;
		for (int i = 0; i < newBoard.getTotalRowsCount(); i++)
		{
			string line;
			getline(cin, line);
			line = Helper::trimString(line);
			vector<string> lineFields = Helper::explodeString(line, ' ');

			if (lineFields.size() != newBoard.sideSize + rowExpansion)
			{
				// empty means loading failed
				return {};
			}
			else
			{
				boardString.push_back(lineFields);
			}
			rowExpansion = (i < newBoard.sideSize - 1) ? rowExpansion + 1 : rowExpansion - 1;
		}
		return boardString;
	}

	void offsetFields(const vector<vector<string>>& boardString, Board& newBoard)
	{
		int rowExpansion = newBoard.sideSize - 1;
		for (int i = 0; i < newBoard.getTotalRowsCount(); i++)
		{
			for (int j = 0; j < boardString[i].size(); j++)
			{
				newBoard.board[i][j + rowExpansion] = boardString[i][j][0];
			}
			rowExpansion = (rowExpansion > 0) ? rowExpansion - 1 : 0;
		}
	}

};


void boardTest()
{
	Board board;
	board.populateNamedBoard();

	board["c3"].value = 'X';
	board["c4"].value = 'Z';
	board["b2"].value = '2';
	board["b3"].value = '3';
	board["b4"].value = '4';
	board.getNeighbour("c3", Board::Move::UPPER_RIGHT).print();
	cout << endl;
	board.printBoard();
}

int main()
{
	//boardTest();
	Game game;
	game.loadBoard();
	game.board["b2"].print();
	//
}