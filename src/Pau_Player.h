#pragma once

#include "iparcheesi.h"
#include "game.h"

class PauPlayer : public IPlayer {
public:
    class Piece {
    public:
        int position;
        int turn;

        Piece() : position{ 0 }, turn{ 0 } {}
    };
    static const int kPieceNum = 4;
    PauPlayer() : color{ IParcheesi::Color::None } {}
    IParcheesi::Color color;
    Piece piece[kPieceNum];

    virtual int DecideMove(const IParcheesi& parcheesi, int current_player_index, int dice_roll) const;
};