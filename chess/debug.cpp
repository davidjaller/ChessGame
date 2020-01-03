#include "debug.h"

static std::map<int, char> pieceChareMap =
{ {WHITE_PAWN, 'P'}, {WHITE_KNIGHT, 'N'},{WHITE_BISHOP, 'B'},{WHITE_ROOK, 'R'},{WHITE_QUEEN, 'Q'},{WHITE_KING, 'K'},
  {BLACK_PAWN, 'p'},{BLACK_KNIGHT, 'n'},{BLACK_BISHOP, 'b'},{BLACK_ROOK, 'r'},{BLACK_QUEEN, 'q'},{BLACK_KING, 'k'} };

void printFen(const Board* board)
{
	//rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
	std::stringstream ss;
	char token{};
	Square sq(0, 0);

	while (sq.rank < 8) {
		sq.file = 0;
		
		while (sq.file < 8) {
			token = '0';
			while (sq.file < 8 && board->IsEmptySquare(sq))
			{
				token++;
				sq.file++;
			}
			if (token > '0') 
				ss << token;
			while (sq.file < 8 && !board->IsEmptySquare(sq)) {
				token = pieceChareMap.find(board->getPieceOnSquare(sq))->second;
				ss << token;
				sq.file++;
			}
		}
		sq.rank++;
		token = '/';
		ss << token;
	}

	// side to move
	ss << ' ';
	if (board->getTurn() == PlayerColor::WHITE)
		token = 'w';
	else
		token = 'b';

	// casteling rights
	if (board->getCastelingPossible(WHITE_SHORT))
		ss << 'K';
	if (board->getCastelingPossible(BLACK_SHORT))
		ss << 'k';
	if (board->getCastelingPossible(WHITE_LONG))
		ss << 'Q';
	if (board->getCastelingPossible(BLACK_LONG))
		ss << 'q';
	//TODO: Implement also halfmove and en pasan
	cout << ss.str();
}