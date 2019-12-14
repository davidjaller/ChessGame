#pragma once
#ifndef _comon_h

#define  _comon_h
#include "source/SharedTypes.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

bool insideBoard(Square square);
bool insideBoard(int rank, int file);
PlayerColor getOpposite(PlayerColor color);
void bitBoardToSquares(vector<Square>* square_v, bitBoard_t bitBoard);
bitBoard_t squareToBitBoard(Square sq);
void printMove(Move move);
string moveToStr(Move move);
void printMoveScore(Move move, float score);
int SquareToIndex(Square sq);
Square IndexToSquare(int i);
bool alignedSquares(Square square1, Square square2, Square* direction);

bool isSliderPiece(int piece);
#endif