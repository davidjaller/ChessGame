#pragma once
#ifndef _comon_h

#define  _comon_h
#include "source/SharedTypes.h"

bool insideBoard(Square square);
bool insideBoard(int rank, int file);
PlayerColor getOpposite(PlayerColor color);

bool isSliderPiece(int piece);
#endif