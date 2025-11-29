#ifndef PAUPARCHEESI_CC
#define PAUPARCHEESI_CC

#include "Pau_Parcheesi.h"
#include "game.h"
#include <cassert>

const int PauParcheesi::safePoint[PauParcheesi::kSafePoints] =
{5, 12, 17, 22, 29, 34, 39, 46, 51, 56, 63, 68};
const int PauParcheesi::startPoint[PauParcheesi::kMaxPlayers] =
{5, 22, 39, 56};
const int PauParcheesi::finishPoint[PauParcheesi::kMaxPlayers] =
{68, 17, 34, 51};

IParcheesi* PauParcheesi::Clone() const {
    return new PauParcheesi(*this);
}
int PauParcheesi::EntryBox(int player_index) const { return safePoint[player_index]; }

int PauParcheesi::ExitBox(int player_index) const { return finishPoint[player_index]; }

int PauParcheesi::PiecesAtHome(int player_index) const { 
  int playersAtHome = 0;
  for(int i = 0; i < PauPlayer::kPieceNum; i++){
    if(players[player_index].piece[i].position == 0) playersAtHome++;
  }
  return playersAtHome;
}

int PauParcheesi::PiecesAtEnd(int player_index) const { 
  int playersAtEnd = 0;
  int endBox = finishPoint[player_index] + 8;

  for(int i = 0; i < PauPlayer::kPieceNum; i++){
    if(players[player_index].piece[i].position == endBox) playersAtEnd++;
  }
  return playersAtEnd; 
}

bool PauParcheesi::IsBoxSafe(int box_index) const { 
  for(int i = 0; i < PauParcheesi::kSafePoints; i++){
    if(box_index == PauParcheesi::safePoint[i]) return true;
  }
  return false;
}
PauParcheesi::Color PauParcheesi::ColorofPiece(int box_index, int piece_index) const {
  for(int i = 0; i < kMaxPlayers; i++){
    for(int e = 0; e < PauPlayer::kPieceNum; e++){
      if(players[i].piece[e].position == box_index)
        return players[i].color;
    }
  }
  return IParcheesi::Color::None;
}

int PauParcheesi::CountPiecesOnBox(int box_index) const {
  int numPieces = 0;
  for(int i = 0; i < PauParcheesi::kMaxPlayers; i++){
    for(int e = 0; e < PauPlayer::kPieceNum; e++){
      if(players[i].piece[e].position == box_index) numPieces++;
    }
  }
  return numPieces;
}

PauParcheesi::Movement PauParcheesi::ApplyMovement(int piece_index, int player_index, int count){
  if (piece_index == -1) {
    for (int i = 0; i < PauPlayer::kPieceNum; i++) {
        PauParcheesi temp_board = *this;
        if (temp_board.ApplyMovement(i, player_index, count) < Movement::IllegalPass) {
            return Movement::IllegalPass;
        }
    }
    return Movement::NoMoves;
  }


  PauPlayer::Piece* currentPiece = &players[player_index].piece[piece_index];
  int currentPos = currentPiece->position;

  if (currentPos == 0) {
    if (count == 5) {
      int nextPos = startPoint[player_index];

      if (CountPiecesOnBox(nextPos) == 2) {
        if (ColorofPiece(nextPos, 0) != static_cast<IParcheesi::Color>(player_index)) {
          return Movement::IllegalEntryBlocked;
        }
      }
      
      currentPiece->position = nextPos;
      return Movement::Normal;

    } else {
      return Movement::IllegalPieceAtHome;
    }
  }

  int nextPos = currentPiece->position + count;

  if(nextPos > finishPoint[player_index] + 8){
    return Movement::IllegalPastEnd;
  }else if(nextPos == finishPoint[player_index] + 8){
    currentPiece->position = nextPos;
    return Movement::End;
  }else if(nextPos >= finishPoint[player_index]){
    currentPiece->position = nextPos;
    return Movement::ReachExit;
  }

  for(int i = 0; i < PauParcheesi::kMaxPlayers; i++){
    for(int e = 0; e < PauPlayer::kPieceNum; e++){
      if(nextPos == players[i].piece[e].position){
        if(players[i].color != players[player_index].color && !IsBoxSafe(nextPos)) {
          currentPiece->position = nextPos;
          return Movement::Eat;
        }
      }
    }
  }

  for(int i = 0; i < kSafePoints; i++){
    if(nextPos == PauParcheesi::safePoint[i]){
      if(CountPiecesOnBox(nextPos) == 2) return Movement::IllegalBridge;
    }
  }

  currentPiece->position = nextPos;
  return Movement::Normal;
}

void PauParcheesi::SendPieceHome(int piece_index, int player_index){
  players[player_index].piece[piece_index].position = 0;
  players[player_index].piece[piece_index].turn = 0;
}

#endif // PAUPARCHEESI_CC