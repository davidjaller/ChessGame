#pragma once
#ifndef _comon_h

#define  _comon_h
#include "source/SharedTypes.h"
#include <string>
#include <iostream>
using namespace std;

bool insideBoard(Square square);
bool insideBoard(int rank, int file);
PlayerColor getOpposite(PlayerColor color);
void printMove(Move move);
string moveToStr(Move move);
void printMoveScore(Move move, float score);

bool isSliderPiece(int piece);
#endif