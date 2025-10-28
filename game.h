#pragma once
#ifndef PARCHEESI_GAME_H
#define PARCHEESI_GAME_H

#include "pauparcheesi.h"

class IPlayer {
  public:
    virtual int DecideMove(const PauParcheesi& parcheesi, int current_player_index, int dice_roll) const = 0;
    virtual ~IPlayer() = default;
};

class Game {
  public:
    Game(PauParcheesi& parcheesi,const IPlayer* players[]);
    void PlayTurn();
    int Roll() const;
    PauParcheesi::Color IsGameOver() const;
    PauParcheesi::Color ActivePlayer() const;


  private:
    PauParcheesi& parcheesi_;
    PauParcheesi::Color current_player_;
    const IPlayer* players_[4];
};

#endif // PARCHEESI_GAME_H