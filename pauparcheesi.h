#pragma once
#ifndef MOCK_PARCHEESI_H
#define MOCK_PARCHEESI_H

#include "game.h"

class Piece{
  private:
    int position_;
    int turn_;
  public:
    Piece() : position_{0}, turn_{0} {} 
    void SetPosition(int pos){position_ = pos;}
    int GetPosition() const {return position_;}

    void SetTurn(int turn){turn_ = turn;}
    int GetTurn() const {return turn_;}
};

class PlayerData{
  public:
    static const int kPieceNum = 4;     

    PlayerData() : color_{IParcheesi::Color::None}{}
    IParcheesi::Color color_;
    Piece piece_[kPieceNum];

    int CountPiecesOnBox(int box);
};


class PauParcheesi : public IParcheesi {
  public:

  PauParcheesi(const PauParcheesi& other);
  PauParcheesi* Clone();

  static const int kMaxPlayers = 4;
  static const int kSafePoints = 12; 
  static const int kBoxNum = 68;
  
  static const int safePoint[kSafePoints];
  static const int startPoint[kMaxPlayers];
  static const int finishPoint[kMaxPlayers];

  int EntryBox(int player_index) const override;
  int ExitBox(int player_index) const override;
  int PiecesAtHome(int player_index) const override;
  int PiecesAtEnd(int player_index) const override;
  bool IsBoxSafe(int box_index) const override;
  int CountPiecesOnBox(int box) const override;
  Color ColorofPiece(int box_index, int box_piece_index) const override;
  
  Movement ApplyMovement(int piece_index, int player_index, int count) override { return Movement::NoMoves; }
  bool PauParcheesi::IsThereABridge(int current_box, int target_box, int finish_yellow_point);
  void SendPieceHome(int piece_index, int player_index) override;

  //----------------
  void MovePiece(int piecendex, int target_box, int player_index);
  void EatPiece(int pieceIndex, int target_box, int player_index);
  int AdjustPlayerPosition(int target_box, int current_box);
  void HandleFinalSectionEntry(int target_box, int pieceIndex, int player_index);

  virtual PauParcheesi* Clone() const;

  
  private:
  PlayerData player_[kMaxPlayers];
};


#endif // MOCK_PARCHEESI_H