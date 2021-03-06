// Ryan Lopez 


#include <iostream>
#include <string>
#include <vector>
using namespace std;

const int SIZE = 64;
// changed up and down to be 8 instead of 1
enum {	LEFT = -1, RIGHT = 1, UP = -8, DOWN = 8, 
		ROW1 = 0, ROW2 = 8, ROW3 = 16, ROW4 = 24, ROW5 = 32, ROW6 = 40, ROW7 = 48, ROW8 = 56,
		COL1 = 0, COL2,		COL3,	   COL4,	  COL5,		 COL6,		COL7,	   COL8};



class Pawn;
class ChessPiece;
class Square;
class ChessBoard;
class King;
class Rook;


class ChessPiece {
protected:
	char color;
	char pieceLetter;
	vector<int> availableMoves;
public:
	vector<int> getMoves() {
		return availableMoves;
	}
	
	bool inVectorAvailableMoves(int x) {
		for (int i = 0; i < availableMoves.size(); ++i) {
			if (availableMoves[i] == x) {
				return true;
			}
		}
		return false;
	}
	bool outOfBounds(int end) {
		if (end < 0 || end > SIZE - 1) {
			return true;
		}
		return false;

	}

	char getLetter() { 
		return pieceLetter;
	}
	char getColor() {
		return color;
	}

	virtual void move(Square*, int, int) = 0;
	
	void printMoves() {
		for (int i = 0; i < availableMoves.size(); ++i) {
			cout << availableMoves[i] << " ";
		}
		cout << endl;
	}

	
	virtual void updateMoves(Square*, int) = 0;
	virtual bool is_friendly(Square*, int) = 0;
	

	
};

class Square {
private:
	ChessPiece* piece;
public:
	Square() {
		piece = nullptr;
	}
	~Square() {
		// should delete even if piece is nullptr?
		if (piece != nullptr) 
			delete piece;
			
		piece = nullptr;

	}
	bool is_empty() {
		if (piece == nullptr) {
			return true;
		}
		else {
			return false;
		}
	}
	// Returns 'N' if square is empty
	char getPieceLetter() {
		if (piece == nullptr) {
			return 'N';
		}
		return piece->getLetter();
	}

	// needs to be set to nullptr if a piece wants to get onto a different square
	ChessPiece* getPiece() {
		return piece;
	}
	void setPiece(ChessPiece* c) {
		piece = c;
	}

};

class Pawn : public ChessPiece {
private:
	int forwardOne = UP;
	bool hasMoved = false;

	bool is_friendly(Square* b, int here) {
		if (b[here].getPiece()->getColor() == color) {
			return true;
		}
		return false;
	}
	bool is_pawn(Square* b, int here) {
		if (b[here].getPiece()->getLetter() == 'P') {
			return true;
		}
		return false;
	}

	

public:
	Pawn(char c) {
		if (c == 'B') forwardOne = DOWN;
		
		color = c;
		pieceLetter = 'P';
	}
	~Pawn() {}

	// Assume that Black is top and White is bottom
	void move(Square* b, int initialPositionOfPiece, int finalPositionOfPiece) {
		if (!inVectorAvailableMoves(finalPositionOfPiece)) {
			cout << "Could not move to location" << endl;
			return;
		}
		// Now the move is definetly available
		



		
		
		if (!b[finalPositionOfPiece].is_empty()) {
			delete b[finalPositionOfPiece].getPiece();
			cout << "Opponent piece captured!" << endl;
		}
		
		b[finalPositionOfPiece].setPiece(b[initialPositionOfPiece].getPiece());
		b[initialPositionOfPiece].setPiece(nullptr);
		
	
		
		hasMoved = true;
		//updateMoves(b, finalPositionOfPiece);
	}

	void updateMoves(Square* b, int pos) {
		availableMoves.clear();
		// needs to check front left/front and right/front

		// in front of pawn is 8
		// left/front of pawn is 7
		// right/front of pawn is 9


		bool onLeftEdge = pos % 8 == 0;
		bool onRightEdge = (pos - 7) % 8 == 0;
		
	

		// check up one
		if (!outOfBounds(pos + forwardOne) && b[pos + forwardOne].is_empty()) {
			availableMoves.push_back(pos + forwardOne);

			//tilesMove.push_back(&b[pos+forwardOne]);
		}




		// check if enPassant left is possible first
		//if (is_enPassant_Possible_Left(b, pos)) {
		//	availableMoves.push_back(pos + forwardOne + LEFT);
		//	passantLeft = true;
		//	//i_enPassantLeft = pos + forwardOne + LEFT;
		//}
		

		// check left up // took out else
		if (!outOfBounds(pos + forwardOne + LEFT) && !b[pos + forwardOne + LEFT].is_empty()) {
			if (!onLeftEdge && /*!is_friendly(b, pos+forwardOne+LEFT)*/b[pos + forwardOne + LEFT].getPiece()->getColor() != color) {
				availableMoves.push_back(pos + forwardOne + LEFT);
				//tilesMove.push_back(&b[pos + forwardOne + LEFT]);
			}
		}

		// check right up
		if (!outOfBounds(pos + forwardOne + RIGHT) && !b[pos + forwardOne + RIGHT].is_empty()) {
			if (!onRightEdge && !is_friendly(b, pos + forwardOne + RIGHT)/*b[pos + forwardOne + RIGHT].getPiece()->getColor() != color*/) {
				// This piece can go diagonal to the right.
				availableMoves.push_back(pos + forwardOne + RIGHT);
				//tilesMove.push_back(&b[pos + forwardOne + RIGHT]);
			}
		}
		// check up 2
		if (!outOfBounds(pos + forwardOne + forwardOne)) {
			// Second, check if tile is empty and this pawn hasnt moved AND there is nothing in between pawn and 2 up
			if (b[pos + forwardOne + forwardOne].is_empty() && b[pos+forwardOne].is_empty() && !hasMoved) {
				availableMoves.push_back(pos + forwardOne + forwardOne);
				//tilesMove.push_back(&b[pos + forwardOne + forwardOne]);
			}
		}

		
		
		// also needs to check this for one turn
		
		
	}
	

	
};

class Rook : public ChessPiece {
private:
	bool is_friendly(Square* b, int here) {
		if (b[here].getPiece()->getColor() == color) {
			return true;
		}
		return false;
	}
public:
	
	Rook(char c) {
		color = c;
		pieceLetter = 'R';
	}
	~Rook() {}

	void move(Square* b, int initialPositionOfPiece, int finalPositionOfPiece) {
		if (!inVectorAvailableMoves(finalPositionOfPiece)) {
			cout << "Could not move to location" << endl;
			return;
		}
		// Now the move is definetly available

		if (!b[finalPositionOfPiece].is_empty()) {
			delete b[finalPositionOfPiece].getPiece();
			cout << "Opponent piece captured!" << endl;
		}

		b[finalPositionOfPiece].setPiece(b[initialPositionOfPiece].getPiece());
		b[initialPositionOfPiece].setPiece(nullptr);
	}
	void updateMoves(Square* b, int pos) {
		availableMoves.clear();

		int columnRelative;
		columnRelative = pos % 8;

		// check down
		for (int i = 8; pos + i < 64; i += 8) {
			if (!outOfBounds(pos + i) && b[pos + i].is_empty()) {
				availableMoves.push_back(pos + i);
			}
			else if (!outOfBounds(pos + i) && !is_friendly(b, pos + i)) {
				availableMoves.push_back(pos + i);
				break;
			}
			else {
				break;
			}
		}
		// check up
		for (int i = 8; pos - i >= 0; i += 8) {
			if (!outOfBounds(pos - i) && b[pos - i].is_empty()) {
				availableMoves.push_back(pos - i);
			}
			else if (!outOfBounds(pos - i) && !is_friendly(b, pos - i)) {
				availableMoves.push_back(pos - i);
				break;
			}
			else {
				break;
			}
		}
		// check left 
		for (int i = 1; columnRelative + i < 8; i++) {
			if (!outOfBounds(pos + i) && b[pos + i].is_empty()) {
				availableMoves.push_back(pos + i);
			}
			else if (!outOfBounds(pos + i) && !is_friendly(b, pos + i)) {
				availableMoves.push_back(pos + i);
				break;
			}
			else {
				break;
			}
		}
		// check right
		for (int i = 1; i <= columnRelative; i++) {
			if (!outOfBounds(pos - i) && b[pos - i].is_empty()) {
				availableMoves.push_back(pos - i);
			}
			else if (!outOfBounds(pos - i) && !is_friendly(b, pos - i)) {
				availableMoves.push_back(pos - i);
				break;
			}
			else {
				break;
			}
		}
	}
};

class King : public ChessPiece {
private:
	bool is_friendly(Square* b, int here) {
		if (b[here].getPiece()->getColor() == color) {
			return true;
		}
		return false;
	}
public:
	
	King(char c) {
		color = c;
		pieceLetter = 'K';
	}
	~King() {}

	void move(Square* b, int initialPositionOfPiece, int finalPositionOfPiece) {
		if (!inVectorAvailableMoves(finalPositionOfPiece)) {
			cout << "Could not move to location" << endl;
			return;
		}
		// Now the move is definetly available

		if (!b[finalPositionOfPiece].is_empty()) {
			delete b[finalPositionOfPiece].getPiece();
			cout << "Opponent piece captured!" << endl;
		}

		b[finalPositionOfPiece].setPiece(b[initialPositionOfPiece].getPiece());
		b[initialPositionOfPiece].setPiece(nullptr);
	}

	void updateMoves(Square* b, int pos) {
		availableMoves.clear();

		
		int columnRelative;
		columnRelative = pos % 8;

		// check one down
		int i = 8;
		if (pos + i < 64) {
			if (!outOfBounds(pos + i) && b[pos + i].is_empty()) {
				availableMoves.push_back(pos + i);
			}
			else if (!outOfBounds(pos + i) && !is_friendly(b, pos + i)) {
				availableMoves.push_back(pos + i);
			}
		}
		// check one up
		if (pos - i >= 0) {
			if (!outOfBounds(pos - i) && b[pos - i].is_empty()) {
				availableMoves.push_back(pos - i);
			}
			else if (!outOfBounds(pos - i) && !is_friendly(b, pos - i)) {
				availableMoves.push_back(pos - i);
			}
		}
		// check right
		i = 1;
		if (columnRelative + i < 64) {
			if (!outOfBounds(pos + i) && b[pos + i].is_empty()) {
				availableMoves.push_back(pos + i);
			}
			else if (!outOfBounds(pos + i) && !is_friendly(b, pos + i)) {
				availableMoves.push_back(pos + i);
			}
		}
		// check left
		if (columnRelative >= i) {
			if (!outOfBounds(pos - i) && b[pos - i].is_empty()) {
				availableMoves.push_back(pos - i);
			}
			else if (!outOfBounds(pos - i) && !is_friendly(b,pos-i)) {
			
				availableMoves.push_back(pos - i);
			}
		}
		/////////////////////////////////////////
		// in front is 8
		// left/front is 7
		// right/front is 9


		// top left
		i = 7;
		if (pos + i < 64 && !(pos % 8 == 0)) {
			if (!outOfBounds(pos + i) && b[pos + i].is_empty()) {
				availableMoves.push_back(pos + i);
			}
			else if (!outOfBounds(pos + i) && !is_friendly(b, pos + i)) {
				availableMoves.push_back(pos + i);
			}
		}
		// bottom right
		if (pos - i >= 0 && !((pos - 7) % 8 == 0)) {
			if (!outOfBounds(pos - i) && b[pos - i].is_empty()) {
				availableMoves.push_back(pos - i);
			}
			else if (!outOfBounds(pos - i) && !is_friendly(b, pos - i)) {
				availableMoves.push_back(pos - i);
			}
		}
		// top right
		i = 9;
		if (pos + i < 64 && !((pos - 7) % 8 == 0)) {
			if (!outOfBounds(pos + i) && b[pos + i].is_empty()) {
				availableMoves.push_back(pos + i);
			}
			else if (!outOfBounds(pos + i) && !is_friendly(b, pos + i)) {
				availableMoves.push_back(pos + i);
			}
		}
		// bottom left
		if (pos - i >= 0 && !(pos % 8 == 0)) {
			if (!outOfBounds(pos - i) && b[pos - i].is_empty()) {
				availableMoves.push_back(pos - i);
			}
			else if (!outOfBounds(pos - i) && !is_friendly(b, pos - i)) {
				availableMoves.push_back(pos - i);
			}
		}

			
		/*if (inCheck(b,pos)) {
			if (availableMoves.size() == 0) {
				cout << "Checkmate! " << color << " wins" << endl;
			}
			else {
				cout << "In check!" << endl;
			}
			
		}*/
	}

	void removeCheckableMoves(Square* b) {
		// Check every piece
		for (int i = 0; i < SIZE; ++i) {

			// if square is not empty and not friendly
			if (!b[i].is_empty() && !is_friendly(b, i)) {

				// get availableMoves of that piece and compare with king, if there is a matching move then king should delete that option.
				vector<int> opposingPieceMoves = b[i].getPiece()->getMoves();
				// Exception for pawn class
				
				if (b[i].getPieceLetter() == 'P') {
					opposingPieceMoves.clear();
					if (b[i].getPiece()->getColor() == 'W' && color == 'B') {
						if (b[i + UP + LEFT].is_empty()) {
							opposingPieceMoves.push_back(i + UP + LEFT);
						}
						if (b[i + UP + RIGHT].is_empty()) {
							opposingPieceMoves.push_back(i + UP + RIGHT);
						}
					}
					else if (b[i].getPiece()->getColor() == 'B' && color == 'W') {
						if (b[i + DOWN + LEFT].is_empty()) {
							opposingPieceMoves.push_back(i + DOWN + LEFT);
						}
						
						if (b[i + DOWN + RIGHT].is_empty()) {
							opposingPieceMoves.push_back(i + DOWN + RIGHT);
						}
						
					}
				}



				// For every move in opposing piece it should check every move in king. If a match then it erases that
				for (int j = 0; j < opposingPieceMoves.size(); ++j) {
					for (int k = 0; k < availableMoves.size(); ++k) {
						if (opposingPieceMoves[j] == availableMoves[k]) {
							// delete move from king
							availableMoves.erase(availableMoves.begin() + k);
						}
					}
				}
			}
		}
	}



	bool inCheck(Square* b, int initialPos) {
		vector<int> opposingPieceMoves;

		// Checks every opponent piece moves and determines if they contain the position of the King
		for (int i = 0; i < SIZE; ++i) {

			if (!b[i].is_empty() && !is_friendly(b, i)) {
				opposingPieceMoves = b[i].getPiece()->getMoves();



				// Exception for pawn class
				if (b[i].getPieceLetter() == 'P') {
					opposingPieceMoves.clear();
					if (b[i].getPiece()->getColor() == 'W' && color == 'B') {
						if (b[i + UP + LEFT].is_empty()) {
							opposingPieceMoves.push_back(i + UP + LEFT);
						}
						if (b[i + UP + RIGHT].is_empty()) {
							opposingPieceMoves.push_back(i + UP + RIGHT);
						}
					}
					else if (b[i].getPiece()->getColor() == 'B' && color == 'W') {
						if (b[i + DOWN + LEFT].is_empty()) {
							opposingPieceMoves.push_back(i + DOWN + LEFT);
						}

						if (b[i + DOWN + RIGHT].is_empty()) {
							opposingPieceMoves.push_back(i + DOWN + RIGHT);
						}

					}
				}


				for (int j = 0; j < opposingPieceMoves.size(); ++j) {
					if (opposingPieceMoves[j] == initialPos) {
						return true;
					}
				}

			}
		}


		return false;
	}
};

class Bishop : public ChessPiece {
private:
	bool is_friendly(Square* b, int here) {
		if (b[here].getPiece()->getColor() == color) {
			return true;
		}
		return false;
	}
public:

	Bishop(char c) {
		color = c;
		pieceLetter = 'B';
	}
	~Bishop() {}

	void move(Square* b, int initialPositionOfPiece, int finalPositionOfPiece) {
		if (!inVectorAvailableMoves(finalPositionOfPiece)) {
			cout << "Could not move to location" << endl;
			return;
		}
		// Now the move is definetly available

		if (!b[finalPositionOfPiece].is_empty()) {
			delete b[finalPositionOfPiece].getPiece();
			cout << "Opponent piece captured!" << endl;
		}

		b[finalPositionOfPiece].setPiece(b[initialPositionOfPiece].getPiece());
		b[initialPositionOfPiece].setPiece(nullptr);
	}
	void updateMoves(Square* b, int pos) {
		availableMoves.clear();

		int columnRelative;
		columnRelative = pos % 8;

		int counter = 0;

		// check bottom left
		for (int i = 7; pos + i < 64; i += 7) {
			counter++;
			if (columnRelative == 0) {
				break;
			}
			if (counter > columnRelative) {
				break;
			}
			if (!outOfBounds(pos + i) && b[pos + i].is_empty()) {
				availableMoves.push_back(pos + i);
			}
			else if (!outOfBounds(pos + i) && !is_friendly(b, pos + i)) {
				availableMoves.push_back(pos + i);
				break;
			}
			else {
				break;
			}
		}
		// check top right
		counter = 0;
		for (int i = 7; pos - i >= 0; i += 7) {
			counter++;
			if (columnRelative == 8) {
				break;
			}
			if (counter >= 8 - columnRelative) {
				break;
			}
			if (!outOfBounds(pos - i) && b[pos - i].is_empty()) {
				availableMoves.push_back(pos - i);
			}
			else if (!outOfBounds(pos - i) && !is_friendly(b, pos - i)) {
				availableMoves.push_back(pos - i);
				break;
			}
			else {
				break;
			}
		}
		counter = 0;
		// check bottom right
		for (int i = 9; pos + i < 64; i += 9) {
			counter++;
			if (columnRelative == 8) {
				break;
			}
			if (counter >= 8 - columnRelative) {
				break;
			}
			if (!outOfBounds(pos + i) && b[pos + i].is_empty()) {
				availableMoves.push_back(pos + i);
			}
			else if (!outOfBounds(pos + i) && !is_friendly(b, pos + i)) {
				availableMoves.push_back(pos + i);
				break;
			}
			else {
				break;
			}
		}
		counter = 0;
		// check top left
		for (int i = 9; pos - i >= 0; i += 9) {
			counter++;
			if (columnRelative == 0) {
				break;
			}
			if (counter > columnRelative) {				////////// took out =
				break;
			}
			if (!outOfBounds(pos - i) && b[pos - i].is_empty()) {
				availableMoves.push_back(pos - i);
			}
			else if (!outOfBounds(pos - i) && !is_friendly(b, pos - i)) {
				availableMoves.push_back(pos - i);
				break;
			}
			else {
				break;
			}
		}
	}
};

class Queen : public ChessPiece {
private:
	bool is_friendly(Square* b, int here) {
		if (b[here].getPiece()->getColor() == color) {
			return true;
		}
		return false;
	}
public:

	Queen(char c) {
		color = c;
		pieceLetter = 'Q';
	}
	~Queen() {}

	void move(Square* b, int initialPositionOfPiece, int finalPositionOfPiece) {
		if (!inVectorAvailableMoves(finalPositionOfPiece)) {
			cout << "Could not move to location" << endl;
			return;
		}
		// Now the move is definetly available

		if (!b[finalPositionOfPiece].is_empty()) {
			delete b[finalPositionOfPiece].getPiece();
			cout << "Opponent piece captured!" << endl;
		}

		b[finalPositionOfPiece].setPiece(b[initialPositionOfPiece].getPiece());
		b[initialPositionOfPiece].setPiece(nullptr);
	}
	void updateMoves(Square* b, int pos) {
		availableMoves.clear();

		int columnRelative;
		columnRelative = pos % 8;

		int counter = 0;

		// check top left
		for (int i = 7; pos + i < 64; i += 7) {
			counter++;
			if (columnRelative == 0) {
				break;
			}
			if (counter > columnRelative) {
				break;
			}
			if (!outOfBounds(pos + i) && b[pos + i].is_empty()) {
				availableMoves.push_back(pos + i);
			}
			else if (!outOfBounds(pos + i) && !is_friendly(b, pos + i)) {
				availableMoves.push_back(pos + i);
				break;
			}
			else {
				break;
			}
		}
		// check bottom right
		counter = 0;
		for (int i = 7; pos - i > 0; i += 7) {
			counter++;
			if (columnRelative == 8) {
				break;
			}
			if (counter >= 8 - columnRelative) {
				break;
			}
			if (!outOfBounds(pos - i) && b[pos - i].is_empty()) {
				availableMoves.push_back(pos - i);
			}
			else if (!outOfBounds(pos - i) && !is_friendly(b, pos - i)) {
				availableMoves.push_back(pos - i);
				break;
			}
			else {
				break;
			}
		}
		counter = 0;
		// check top right
		for (int i = 9; pos + i < 64; i += 9) {
			counter++;
			if (columnRelative == 8) {
				break;
			}
			if (counter >= 8 - columnRelative) {
				break;
			}
			if (!outOfBounds(pos + i) && b[pos + i].is_empty()) {
				availableMoves.push_back(pos + i);
			}
			else if (!outOfBounds(pos + i) && !is_friendly(b, pos + i)) {
				availableMoves.push_back(pos + i);
				break;
			}
			else {
				break;
			}
		}
		counter = 0;
		// check bottom left
		for (int i = 9; pos - i > 0; i += 9) {
			counter++;
			if (columnRelative == 0) {
				break;
			}
			if (counter >= columnRelative) {
				break;
			}
			if (!outOfBounds(pos - i) && b[pos - i].is_empty()) {
				availableMoves.push_back(pos - i);
			}
			else if (!outOfBounds(pos - i) && !is_friendly(b, pos - i)) {
				availableMoves.push_back(pos - i);
				break;
			}
			else {
				break;
			}
		}


		///////////////////////////////////////////////////////////////////

		// check down
		for (int i = 8; pos + i < 64; i += 8) {
			if (!outOfBounds(pos + i) && b[pos + i].is_empty()) {
				availableMoves.push_back(pos + i);
			}
			else if (!outOfBounds(pos + i) && !is_friendly(b, pos + i)) {
				availableMoves.push_back(pos + i);
				break;
			}
			else {
				break;
			}
		}
		// check up
		for (int i = 8; pos - i > 0; i += 8) {
			if (!outOfBounds(pos - i) && b[pos - i].is_empty()) {
				availableMoves.push_back(pos - i);
			}
			else if (!outOfBounds(pos - i) && !is_friendly(b, pos - i)) {
				availableMoves.push_back(pos - i);
				break;
			}
			else {
				break;
			}
		}
		// check left 
		for (int i = 1; columnRelative + i < 8; i++) {
			if (!outOfBounds(pos + i) && b[pos + i].is_empty()) {
				availableMoves.push_back(pos + i);
			}
			else if (!outOfBounds(pos + i) && !is_friendly(b, pos + i)) {
				availableMoves.push_back(pos + i);
				break;
			}
			else {
				break;
			}
		}
		// check right
		for (int i = 1; i <= columnRelative; i++) {
			if (!outOfBounds(pos - i) && b[pos - i].is_empty()) {
				availableMoves.push_back(pos - i);
			}
			else if (!outOfBounds(pos - i) && !is_friendly(b, pos - i)) {
				availableMoves.push_back(pos - i);
				break;
			}
			else {
				break;
			}
		}
	}
};

class Knight : public ChessPiece {
private:
	bool is_friendly(Square* b, int here) {
		if (b[here].getPiece()->getColor() == color) {
			return true;
		}
		return false;
	}
public:

	Knight(char c) {
		color = c;
		pieceLetter = 'L';
	}
	~Knight() {}

	void move(Square* b, int initialPositionOfPiece, int finalPositionOfPiece) {
		if (!inVectorAvailableMoves(finalPositionOfPiece)) {
			cout << "Could not move to location" << endl;
			return;
		}
		// Now the move is definetly available

		if (!b[finalPositionOfPiece].is_empty()) {
			delete b[finalPositionOfPiece].getPiece();
			cout << "Opponent piece captured!" << endl;
		}

		b[finalPositionOfPiece].setPiece(b[initialPositionOfPiece].getPiece());
		b[initialPositionOfPiece].setPiece(nullptr);
	}

	void updateMoves(Square* b, int pos) {
		availableMoves.clear();

		/*bool onLeftEdge = pos % 8 == 0;
		bool onRightEdge = (pos - 7) % 8 == 0;*/


		if (!outOfBounds(pos + UP + UP + LEFT) && !(pos >= 0 && pos <= 15) && !(pos % 8 == 0)) {
			if (b[pos + UP + UP + LEFT].is_empty() || !is_friendly(b, pos + UP + UP + LEFT)) {
				availableMoves.push_back(pos + UP + UP + LEFT);
			}
		}
		if (!outOfBounds(pos + UP + UP + RIGHT) && !(pos >= 0 && pos <= 15) && !((pos - 7) % 8 == 0)) {
			if (b[pos + UP + UP + RIGHT].is_empty() || !is_friendly(b, pos + UP + UP + RIGHT)) {
				availableMoves.push_back(pos + UP + UP + RIGHT);
			}
		}
		if (!outOfBounds(pos + LEFT + LEFT + UP) && !((pos - 1) % 8 == 0) && !(pos % 8 == 0) && !(pos >= 0 && pos <= 7)) {
			if (b[pos + LEFT + LEFT + UP].is_empty() || !is_friendly(b, pos + LEFT + LEFT + UP)) {
				availableMoves.push_back(pos + LEFT + LEFT + UP);
			}
		}
		if (!outOfBounds(pos + LEFT + LEFT + DOWN) && !((pos - 1) % 8 == 0) && !(pos % 8 == 0) && !(pos >= 56 && pos <= 63)) {
			if(b[pos + LEFT + LEFT + DOWN].is_empty() || !is_friendly(b, pos + LEFT + LEFT + DOWN)) {
				availableMoves.push_back(pos + LEFT + LEFT + DOWN);
			}
		}
		if (!outOfBounds(pos + RIGHT + RIGHT + UP) && !((pos - 6) % 8 == 0) && !((pos - 7) % 8 == 0) && !(pos >= 0 && pos <= 7)) {
			if (b[pos + RIGHT + RIGHT + UP].is_empty() || !is_friendly(b, pos + RIGHT + RIGHT + UP)) {
				availableMoves.push_back(pos + RIGHT + RIGHT + UP);
			}
		}
		if (!outOfBounds(pos + RIGHT + RIGHT + DOWN) && !((pos - 6) % 8 == 0) && !((pos - 7) % 8 == 0) && !(pos >= 56 && pos <= 63)) {
			if (b[pos + RIGHT + RIGHT + DOWN].is_empty() || !is_friendly(b, pos + RIGHT + RIGHT + DOWN)) {
				availableMoves.push_back(pos + RIGHT + RIGHT + DOWN);
			}
		}
		if (!outOfBounds(pos + DOWN + DOWN + LEFT) && !(pos >= 48 && pos <= 63) && !(pos % 8 == 0)) {
			if (b[pos + DOWN + DOWN + LEFT].is_empty() || !is_friendly(b, pos + DOWN + DOWN + LEFT)) {
				availableMoves.push_back(pos + DOWN + DOWN + LEFT);
			}
		}
		if (!outOfBounds(pos + DOWN + DOWN + RIGHT) && !(pos >= 48 && pos <= 63) && !((pos - 7) % 8 == 0)) {
			if (b[pos + DOWN + DOWN + RIGHT].is_empty() || !is_friendly(b, pos + DOWN + DOWN + RIGHT)) {
				availableMoves.push_back(pos + DOWN + DOWN + RIGHT);
			}
		}

	}

};




class Chessboard {
private:
	// 1d chessboard array
	Square* Board;

	void updateAll() {
		for (int i = 0; i < SIZE; ++i) {
			if (!Board[i].is_empty()) {
				Board[i].getPiece()->updateMoves(Board,i);
			}
		}

		// for removing the moves that cause the king check
		for (int i = 0; i < SIZE; ++i) {
			if (!Board[i].is_empty() && Board[i].getPieceLetter() == 'K') {
				King* k = dynamic_cast<King*>(Board[i].getPiece());
				k->removeCheckableMoves(Board);
				



				// check for condition if a piece moves, will it still result in check?

				if (k->inCheck(Board, i)) {
					if (k->getMoves().empty()) {
						cout << "Checkmate! " << k->getColor() << " wins" << endl;
					}
					else {
						cout << "In check!" << endl;
						// check all teammate pieces
						//	- if that piece moves onto another square will the king still remain in check?




						//Square* imaginaryBoard = new Square[SIZE];

						//for (int i = 0; i < SIZE; ++i) {
						//	// delete teammate available moves and give them moves that will help king avoid check
						//}






					}

				}
			}
		}
	}
public:
	
	//will be used to initialize the chessboard
	Chessboard() {
		
		//Assume that Black is top and White it bottom
		//dynamically allocate each square 
		//dynamically allocate pieces onto the board


		Board = new Square[SIZE];


		//initialize entire row with black pawn
		for (int i = COL1; i <= COL8; ++i) {
			Board[ROW2 + i].setPiece(new Pawn('B'));
		}
		//initialize entire row with white pawn
		for (int i = COL1; i <= COL8; ++i) {
			Board[ROW7 + i].setPiece(new Pawn('W'));
		}
		
		// Rook
		{
			Board[0].setPiece(new Rook('B'));
			Board[7].setPiece(new Rook('B'));
			Board[56].setPiece(new Rook('W'));
			Board[63].setPiece(new Rook('W'));
		}

		// Knight
		{
			Board[1].setPiece(new Knight('B'));
			Board[6].setPiece(new Knight('B'));
			Board[57].setPiece(new Knight('W'));
			Board[62].setPiece(new Knight('W'));
		}

		// Bishop
		{
			Board[2].setPiece(new Bishop('B'));
			Board[5].setPiece(new Bishop('B'));
			Board[58].setPiece(new Bishop('W'));
			Board[61].setPiece(new Bishop('W'));
		}

		// Queen
		{
			Board[3].setPiece(new Queen('B'));
			Board[59].setPiece(new Queen('W'));
		}

		// King
		{
			Board[4].setPiece(new King('B'));
			Board[60].setPiece(new King('W'));
		}


		/*Board[10].setPiece(new Pawn('W'));
		Board[24].setPiece(new King('B'));

		Board[35].setPiece(new Bishop('W'));*/


		updateAll();
	}
	~Chessboard() {
		/*for (int i = 0; i < SIZE; ++i) {
			if (!Board[i].is_empty()) {
				delete Board[i].getPiece();
			}
		}*/
		delete[] Board;
	}

	void move(int initialPositionOfPiece, int finalPositionOfPiece) {
		
		if (!Board[initialPositionOfPiece].is_empty()) {
			Board[initialPositionOfPiece].getPiece()->move(Board, initialPositionOfPiece, finalPositionOfPiece);
		}
		else {
			cout << "Square is empty - Chessboard::move" << endl;
		}
		
		updateAll();
	}

	Square* getSquare(int pos) {
		return &Board[pos];

		//highlight the moves of that piece
		//if the player wants to move it then it should call move function of piece - if NOT then it should unhighlight the moves
	}

	void printChessBoard() {
		// display board with numbers - debugging purposes
		/*for (int i = 0; i < SIZE; ++i) {
			
			if (i % 8 == 0 && i != 0) {
				cout << endl;
			}
			if (i == 0 || i == 8) {
				cout << " ";
			}
			cout << i << Board[i].getPieceLetter() << " ";
			
		}*/
		// display Board only letters
		for (int i = 0; i < SIZE; ++i) {
			if (i % 8 == 0 && i != 0) {
				cout << endl;
			}
			cout << Board[i].getPieceLetter();

		}





		cout << endl;
	}
	
	void printMovesOnSquare(int pos) {
		
		if (!Board[pos].is_empty()) {
			//Board[pos].getPiece()->updateMoves(Board, pos);
			Board[pos].getPiece()->printMoves();
		}
		else {
			cout << "Square is empty - Chessboard::printMovesOnSquare" << endl;
		}
		
	}
	

};




int main() {

	Chessboard b1;

	int initialPos;
	int finalPos;


	while (true) {
		
		b1.printChessBoard();

		cout << "Enter index of piece: ";
		
		cin >> initialPos;
		if (cin.fail()) {
			cout << "Exited" << endl;
			break;
		}
	
		cout << "Moves: ";
		b1.printMovesOnSquare(initialPos);
		cin >> finalPos;
		if (cin.fail()) {
			cout << "Exited" << endl;
			break;
		}
		b1.move(initialPos, finalPos);

		
	}
	

}