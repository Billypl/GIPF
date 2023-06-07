#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
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

public:

	char EMPTY_FIELD = '_';
	char STARTING_ROW_LETTER = 'b';
	int STARTING_ROW_NUMBER = 2;
	char WHITE_PLAYER = 'W';
	char BLACK_PLAYER = 'B';

	// sideSize >= 2
	int sideSize; 
	int getTotalRowsCount()
	{
		return (sideSize - 1) * 2 + 1;
	}
	
	vector<vector<char>> board;
	map<string, point> namedBoard;
	map<string, point> namedFrame;
	map<point, string> positionedBoard;

	Board() {};
	Board(int sideSize) : sideSize(sideSize), board(getTotalRowsCount(), vector<char>(getTotalRowsCount())) 
	{
		createHashMaps();
	};
	
	struct Field
	{
		point coords;
		char& value;
		string name;
		Field(point coords, char& value, string name) : coords(coords), value(value), name(name) {};
		void print()
		{
			coords.print();
			cout << " " << value;
		}
	};

	Field get(string fieldName)
	{
		if (isBoard(fieldName))
		{
			point coords = namedBoard[fieldName];
			Field field(coords, board[coords.y][coords.x], fieldName);
			return field;
		}
		else if (isFrame(fieldName))
		{
			point coords = namedFrame[fieldName];
			Field field(coords, board[0][0], fieldName);
			return field;
		}
		throw "Wrong field name!";
	}
	Field operator[](string field)
	{
		return get(field);
	}

	Field operator[](point position)
	{
		return get(position);
	}

	Field get(point position)
	{
		if (positionedBoard.find(position) != positionedBoard.end())
		{
			string fieldName = positionedBoard[position];
			Field field(position, board[position.y][position.x], fieldName);
			return field;
		}
		throw "Wrong coords!";
	}


	enum Move
	{
		UPPER_LEFT,		
		UPPER_RIGHT,	
		RIGHT,			
		BOTTOM_RIGHT,	
		BOTTOM_LEFT,	
		LEFT			
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

		point fieldCoords;
		if(isFrame(fieldName))
		{
			fieldCoords = namedFrame[fieldName];
		}
		else if (isBoard(fieldName))
		{
			fieldCoords = namedBoard[fieldName];
		}

		point neighbourCoords = fieldCoords + offset;
		if (positionedBoard.find(neighbourCoords) != positionedBoard.end())
		{
			Field neighbour(neighbourCoords, board[neighbourCoords.y][neighbourCoords.x], positionedBoard[neighbourCoords]);
			return neighbour;
		}
		else
		{
			Field neighbour(neighbourCoords, board[0][0], "x9");
			return neighbour;
		}

	}


	void createHashMaps()
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
				positionedBoard.insert({ fieldCoords , fieldName });

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
		createFrameMap();
	}

	void createFrameMap()
	{
		sideSize++;
		char fieldLetter = 'a';
		int rowExpansion = 0;
		point fieldCoords;

		for (int i = 0; i < getTotalRowsCount(); i++)
		{
			if (i < sideSize)
				fieldCoords = { 0 - 1, sideSize + i - 1 - 1};
			else
				fieldCoords = { i - sideSize + 1 - 1, (sideSize - 2) * 2 + 2 - 1};

			int fieldNumber = 1;
			for (int j = 0; j < sideSize + rowExpansion; j++)
			{
				string fieldName = fieldLetter + to_string(fieldNumber);
				namedFrame[fieldName] = fieldCoords;

				fieldCoords += {1, -1};
				fieldNumber++;
			}
			// fixes rows expanding in the middle
			rowExpansion = (i < sideSize - 1) ? rowExpansion + 1 : rowExpansion - 1;
			fieldLetter++;
		}
		sideSize--;


		for (auto it : namedBoard)
		{
			if (namedFrame.find(it.first) != namedFrame.end())
			{
				namedFrame.erase(it.first);
			}
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
				if (board[i][j] >= 'A')
				{
					cout << board[i][j] << " ";
				}
				else
				{
					cout << " ";
				}
			}
			cout << endl;
		}
	}

	bool isInBounds(string filename)
	{
		return  isFrame(filename) || isBoard(filename);
	}

	bool isFrame(string filename)
	{
		return namedFrame.find(filename) != namedFrame.end();
	}

	bool isBoard(string filename)
	{
		return namedBoard.find(filename) != namedBoard.end();
	}

	int getActivePawnsNumberOfColor(char color)
	{
		int counter = 0;
		for (const auto& it : board)
		{
			for (const auto& itt : it)
			{
				if (itt == color)
				{
					counter++;
				}
			}
		}
		return counter;
	}

};

class Game
{

public:

	Board board;

	int triggerCollectionNumber;
	int whitePiecesNumber;
	int blackPieciesNumber;
	int reserveWhitePawnsNumber;
	int reserveBlackPawnsNumber;
	char currentPlayer;

	// sideSize >= 2
	// 2 < triggerCollectionNumber < 2*sideSize - 1
	// white/black Piecies > 3	   
	// | white - black | <= 3
	// S K GW GB


	/* BOARD LOADING */

	void loadBoard()
	{
		int sideSize; cin >> sideSize;
		Board newBoard(sideSize);

		cin >> this->triggerCollectionNumber;
		cin >> this->whitePiecesNumber;
		cin >> this->blackPieciesNumber;

		cin >> this->reserveWhitePawnsNumber;
		cin >> this->reserveBlackPawnsNumber;
		cin >> this->currentPlayer;


		vector<vector<string>> boardString = loadFields(newBoard);
		if (boardString.empty())
		{
			cout << "WRONG_BOARD_ROW_LENGTH" << endl;
			board = {};
			return;
		}
		offsetFields(boardString, newBoard);
		if (newBoard.getActivePawnsNumberOfColor(newBoard.WHITE_PLAYER) + reserveWhitePawnsNumber > whitePiecesNumber)
		{
			cout << "WRONG_WHITE_PAWNS_NUMBER" << endl;
			board = {};
			return;
		}
		else if (newBoard.getActivePawnsNumberOfColor(newBoard.BLACK_PLAYER) + reserveBlackPawnsNumber > blackPieciesNumber)
		{
			cout << "WRONG_BLACK_PAWNS_NUMBER" << endl;
			board = {};
			return;
		}
		// TODO: check for 4 nighbouring pawns
		int errorOfLengthK = getPawnsInBadStartingPositionNumber(newBoard);
		if (errorOfLengthK != 0)
		{
			string templateMsg1 = "ERROR_FOUND_";
			string templateMsg2 = "_ROW";
			string templateMsg3 = "_OF_LENGTH_K";
			if (errorOfLengthK > 1)
			{
				templateMsg2 += 'S';
			}
			string finalMsg = templateMsg1 + to_string(errorOfLengthK) + templateMsg2 + templateMsg3;
			cout << finalMsg << endl;
			return;
		}

		board = newBoard;
		cout << "BOARD_STATE_OK" << endl;
	}

	vector<vector<string>> loadFields(Board newBoard)
	{
		bool isBoardGood = true;
		string trash; getline(cin, trash);
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
				isBoardGood = false;
			}
			else
			{
				boardString.push_back(lineFields);
			}
			rowExpansion = (i < newBoard.sideSize - 1) ? rowExpansion + 1 : rowExpansion - 1;
		}
		if (isBoardGood)
		{
			return boardString;
		}
		else
		{
			return {};
		}
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


	/* ERROR_FOUND_x_ROW_OF_LENGTH_K */

	int getPawnsInBadStartingPositionNumber(Board newBoard)
	{
		// FIND DIAGONAL LETTER
		char diagonalLetter = board.STARTING_ROW_LETTER;
		for (int i = 0; i < board.sideSize - 1; i++)
		{
			diagonalLetter++;
		}

		int badRowsCounter = 0;
		// RIGHT
		badRowsCounter += getBadStartingPositionsRightRowsNumber(newBoard);
		// DOWN
		badRowsCounter += getBadStartingPositionsDownRowsNumber(newBoard);
		// UP-RIGHT
		badRowsCounter += getBadStartingPositionsDiagonalRowsNumber(newBoard);

		return badRowsCounter;
	}

	int getBadStartingPositionsRightRowsNumber(Board newBoard)
	{
		int badRowsCounter = 0;
		int rowExpansion = newBoard.sideSize - 1;
		for (int i = 0; i < newBoard.getTotalRowsCount(); i++)
		{
			int pawnsInRowCounter = 1;
			char prevPawn = newBoard.board[i][rowExpansion];
			for (int j = rowExpansion + 1; j < newBoard.board[i].size() && newBoard.board[i][j] != '\0'; j++)
			{
				if (newBoard.board[i][j] == prevPawn && newBoard.board[i][j] != '_')
				{
					pawnsInRowCounter++;
					if (pawnsInRowCounter == triggerCollectionNumber)
					{
						badRowsCounter++;
						continue;
					}
				}
				else
				{
					pawnsInRowCounter = 1;
				}
				prevPawn = newBoard.board[i][j];
			}
			rowExpansion = (rowExpansion > 0) ? rowExpansion - 1 : 0;
		}
		return badRowsCounter;
	}

	int getBadStartingPositionsDownRowsNumber(Board newBoard)
	{
		newBoard.board = Helper::transposeMatrix(newBoard.board);
		int badRowsCounter = getBadStartingPositionsRightRowsNumber(newBoard);
		return badRowsCounter;
	}

	int getBadStartingPositionsDiagonalRowsNumber(Board newBoard)
	{
		int badRowsCounter = 0;
		char rowLetter = newBoard.STARTING_ROW_LETTER;
		int rowExpansion = 0;
		for (int i = 0; i < newBoard.getTotalRowsCount(); i++)
		{
			int pawnsInRowCounter = 1;
			int rowNumber = newBoard.STARTING_ROW_NUMBER;
			char prevPawn = newBoard[rowLetter + to_string(rowNumber)].value;
			for (int j = 1; j < newBoard.sideSize + rowExpansion; j++)
			{
				rowNumber++;
				string fieldName = rowLetter + to_string(rowNumber);
				if (newBoard[fieldName].value == prevPawn && newBoard[fieldName].value != '_')
				{
					pawnsInRowCounter++;
					if (pawnsInRowCounter == triggerCollectionNumber)
					{
						badRowsCounter++;
						continue;
					}
				}
				else
				{
					pawnsInRowCounter = 1;
				}

				prevPawn = newBoard[fieldName].value;
			}
			rowExpansion = (i < newBoard.sideSize - 1) ? rowExpansion + 1 : rowExpansion - 1;
			rowLetter++;
		}
		return badRowsCounter;
	}


	/* MOVE */

	void doMove(string srcField, string dstField)
	{
		/* 1. ARE IN BOUNDS? */
		if (!board.isInBounds(srcField))
		{
			cout << "BAD_MOVE_" << srcField << "_IS_WRONG_INDEX" << endl;
			return;
		}
		else if (!board.isInBounds(dstField))
		{
			cout << "BAD_MOVE_" << dstField << "_IS_WRONG_INDEX" << endl;
			return;
		}

		/* 3. IS GOOD STARTING FIELD? */
		if (!board.isFrame(srcField))
		{
			cout << "BAD_MOVE_" << srcField << "_IS_WRONG_STARTING_FIELD" << endl;
			return;
		}

		/* 4. IS GOOD ENDING FIELD? */
		if (!board.isBoard(dstField))
		{
			cout << "BAD_MOVE_" << dstField << "_IS_WRONG_DESTINATION_FIELD" << endl;
			return;
		}

		/* 2. ARE NEIGHBOURS? */
		bool isNeighbour = false;
		Board::Move direction;
		for (int move = Board::Move::UPPER_LEFT; move != Board::Move::LEFT + 1; move++)
		{
			if (board.getNeighbour(srcField, static_cast<Board::Move>(move)).name == dstField)
			{
				isNeighbour = true;
				direction = static_cast<Board::Move>(move);
			}
		}
		if (!isNeighbour)
		{
			cout << "UNKNOWN_MOVE_DIRECTION" << endl;
			return;
		}

		/* 5. IF ROW FULL? */
		if (placePawn(direction, board[srcField], currentPlayer))
		{
			if (currentPlayer == 'W')
			{
				currentPlayer = 'B';
				reserveWhitePawnsNumber--;
			}
			else
			{
				currentPlayer = 'W';
				reserveBlackPawnsNumber--;
			}

			cout << "MOVE_COMMITTED" << endl;

		}
		else
		{
			cout << "BAD_MOVE_ROW_IS_FULL" << endl;
		}
		captureThePawns();
	}

	bool placePawn(Board::Move direction, Board::Field current, char player)
	{
		Board::Field next = board.getNeighbour(current.name, direction);
		if (next.value == '_')
		{
			next.value = player;
			return true;
		}
		if (next.value == '\0')
		{
			return false;
		}
		if (placePawn(direction, next, next.value))
		{
			next.value = player;
			return true;
		}
		return false;
	}

	void captureThePawns()
	{
		captureThePawnsRight();
		captureThePawnsDown();
	}

	void captureThePawnsRight()
	{
		int rowExpansion = board.sideSize - 1;
		for (int i = 0; i < board.getTotalRowsCount(); i++)
		{
			int blackPawnsNumber;
			int whitePawnsNumber;
			bool isRowToCapture = false;
			vector<Board::Field> pawns;

			int pawnsInRowCounter = 1;
			char prevPawn = board.board[i][rowExpansion];
			pawns.push_back(board[point(rowExpansion, i)]);
			for (int j = rowExpansion + 1; j < board.board[i].size() && board.board[i][j] != '\0'; j++)
			{
				if (board.board[i][j] == '_')
				{
					if (isRowToCapture)
					{
						break;
					}
				}
				else if (board.board[i][j] == prevPawn && board.board[i][j] != '_')
				{
					pawnsInRowCounter++;
					pawns.push_back(board[point(j, i)]);
					if (pawnsInRowCounter == triggerCollectionNumber)
					{
						isRowToCapture = true;
					}
				}
				else if (isRowToCapture && board.board[i][j] != '_')
				{
					pawnsInRowCounter++;
					pawns.push_back(board[point(j, i)]);
				}
				else
				{
					pawnsInRowCounter = 1;
					pawns.clear();
					pawns.push_back(board[point(i,j)]);
				}
				prevPawn = board.board[i][j];
			}
			rowExpansion = (rowExpansion > 0) ? rowExpansion - 1 : 0;
			if (isRowToCapture)
			{
				removePawns(pawns);
				isRowToCapture = false;
				break;
			}
		}
		
	}


	void captureThePawnsDown()
	{
		board.board = Helper::transposeMatrix(board.board);
		captureThePawnsRight();
		board.board = Helper::transposeMatrix(board.board);
	}

	void removePawns(vector<Board::Field> pawns)
	{
		vector<char> tmp;
		for (auto& it : pawns)
		{
			tmp.push_back(it.value);
			it.value = '_';
		}

		map<char, int> m = Helper::countDuplicates(tmp);
		int w = m['W'];
		int b = m['B'];
		if (w < b)
		{
			reserveBlackPawnsNumber += b;
		}
		else
		{
			reserveWhitePawnsNumber += w;
		}
	}

	/* OTHER */

	void printGame()
	{
		if (board.board.size() == 0)
		{
			cout << "EMPTY_BOARD" << endl;
			return;
		}

		cout << board.sideSize << " ";
		cout << triggerCollectionNumber << " ";
		cout << whitePiecesNumber << " ";
		cout << blackPieciesNumber << " ";
		cout << endl;
		cout << reserveWhitePawnsNumber << " ";
		cout << reserveBlackPawnsNumber << " ";
		cout << currentPlayer;
		cout << endl;

		board.printBoard();
	}

};

int main()
{
	Game game;
	string command;
	while (getline(cin, command))
	{
		if (command == "LOAD_GAME_BOARD")
		{
			game.loadBoard();
			getline(cin, command);
		}
		else if (command == "PRINT_GAME_BOARD")
		{
			game.printGame();
		}
		else if (command.find("DO_MOVE") != string::npos)
		{
			command = Helper::trimString(command);
			command = Helper::explodeString(command, ' ')[1];
			string srcField = Helper::explodeString(command, '-')[0];
			string dstField = Helper::explodeString(command, '-')[1];
			game.doMove(srcField, dstField);
		}
		command.clear();
	}

}