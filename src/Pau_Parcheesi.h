#ifndef PAUPARCHEESI_H
#define PAUPARCHEESI_H

#include "iparcheesi.h"
#include "game.h"
#include "Pau_Player.h"

class PauParcheesi : public IParcheesi {
  public:
    static const int kMaxPlayers = 4;
    static const int kSafePoints = 12; 
    static const int kBoxNum = 68;

    static const int safePoint[kSafePoints];
    static const int startPoint[kMaxPlayers];
    static const int finishPoint[kMaxPlayers];


    IParcheesi* Clone() const override;
    ~PauParcheesi() = default;
    

    int EntryBox(int player_index) const override;
    int ExitBox(int player_index) const override;
    int PiecesAtHome(int player_index) const override;
    int PiecesAtEnd(int player_index) const override;
    bool IsBoxSafe(int box_index) const override;
    Color ColorofPiece(int box_index, int piece_index) const override;

    Movement ApplyMovement(int piece_index, int player_index, int count) override;
    void SendPieceHome(int piece_index, int player_index) override;

    int CountPiecesOnBox(int box_index) const;

    PauPlayer players[kMaxPlayers];
};




#endif // PAUPARCHEESI_H