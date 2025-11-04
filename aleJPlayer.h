#ifndef ALEJPLAYER_H
#define ALEJPLAYER_H

#include "game.h"

class AlejPlayer: public IPlayer{
    virtual int DecideMove(const IParcheesi& parcheesi, int current_player_index, int dice_roll) const override;
};

#endif