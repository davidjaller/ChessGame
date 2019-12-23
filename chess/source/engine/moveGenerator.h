#pragma once
#ifndef _moveGenerator_h
#define _moveGenerator_h
#include "source/board/position.h"
#include"source/comon.h"
#include<vector>
#include<list>
#include "source/board/PieceList.h"

class MoveGenerator {
public:
	static int generate(const Position* position, Square from, int Piece, list<Move> *moveList);

	static int generateMoves(const Position* position, list<Move>* moveList, bool generateCaptures);
	static bool IsLegalMove(const Position* position, Square squareFrom, Square squareTo);
private:
	static int bitBoardToMoves(bitBoard_t movementBB, list<Move>* moveList, Square fromSquare, bool kill, int pieceType);
	static bitBoard_t getCastellingBB(const Position* position, PlayerColor turn);
	static bitBoard_t getPinMask(const Position* position, Square from, Square kingSquare);
	static bitBoard_t getKingShadowsBB(list<const Piece*> attackingSquares, PlayerColor turn, Square kingSquare);
	static bitBoard_t getBlockAttackerBB(const Piece* attackingPiece, PlayerColor turn, Square kingSquare);

	static bitBoard_t filterOutOfCheck(bitBoard_t movementBB, const Position* position, PlayerColor turn, const Piece* attackingPiece, bitBoard_t captureAttackerBB);
	static bitBoard_t filterOutOfCheckKing(bitBoard_t movementBB, const Position* position, PlayerColor turn, list<const Piece*> attackingPieces, bitBoard_t captureAttackerBB);
	static bitBoard_t filterNormal(bitBoard_t movementBB, const Position* position, PlayerColor turn, const Piece* piece);
	static bitBoard_t filterNormalKing(bitBoard_t movementBB, const Position* position, PlayerColor turn);
	

	static int generateForPawn(const Position* position, Square from, int piece, list<Move> *moveList);

	static int generateForBishop(const Position* position, Square from, int piece, list<Move> *moveList);

	static int generateForKnight(const Position* position, Square from, int piece, list<Move> *moveList);

	static int generateForRook(const Position* position, Square from, int piece, list<Move> *moveList);

	static int generateForQueen(const Position* position, Square from, int piece, list<Move> *moveList);

	static int generateForKing(const Position* position, Square from, int piece, list<Move> *moveList);

	static int generateForSliders(const Position* position, Square from, int piece, list<Move>* moveList, vector<array<int, 2>> directions);

	static int generateGetOutOfAttack(const Position* position, Square attackedSquare, vector<Square> attackingSquares, list<Move>* moveList);
	
	static bool movingIntoCheck(const Position* position, Move move);



};

#endif