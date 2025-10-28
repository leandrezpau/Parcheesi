#include "player.h"
#include "pauparcheesi.h"
#include <cassert>

const int PauParcheesi::safePoint[PauParcheesi::kSafePoints] =
{5, 12, 17, 22, 29, 34, 39, 46, 51, 56, 63, 68};
const int PauParcheesi::startPoint[PauParcheesi::kMaxPlayers] =
{5, 22, 39, 56};
const int PauParcheesi::finishPoint[PauParcheesi::kMaxPlayers] =
{68, 17, 34, 51};

PauParcheesi* PauParcheesi::Clone() const {
  PauParcheesi* copy = new PauParcheesi(*this);
  return copy;
}

int PauParcheesi::EntryBox(int player_index) const {
  return safePoint[player_index];
}

int PauParcheesi::ExitBox(int player_index) const {
  return finishPoint[player_index];
}

int PauParcheesi::PiecesAtHome(int player_index) const {
  int count = 0;
  for(int i = 0; i < PlayerData::kPieceNum; i++){
    if(player_[player_index].piece_[i].GetPosition() == 0){
      count++;
    }
  }
  return count;
}

int PauParcheesi::PiecesAtEnd(int player_index) const {
  int count = 0;
  for(int i = 0; i < PlayerData::kPieceNum; i++){
    if(player_[player_index].piece_[i].GetPosition() == 76){
      count++;
    }
  }
  return count;
}

bool PauParcheesi::IsBoxSafe(int box_index) const {
  for(int i = 0; i < kSafePoints; i++){
    if(safePoint[i] == box_index){
      return true;
    }
  }
  return false;
}

int PauParcheesi::CountPiecesOnBox(int box) const {
  int total = 0;
  for(int i = 0; i < 4; i++){
    total += player_[i].CountPiecesOnBox(box);
  }
  return total;
}

void PauParcheesi::SendPieceHome(int piece_index, int player_index){
  player_[player_index].piece_[piece_index].SetPosition(0);
}

IParcheesi::Color PauParcheesi::ColorofPiece(int box_index, int box_piece_index) const {
  int player_in_box[2] = {-1, -1};
  int piece_turn[2] = {0, 0};

  for(int i = 0; i < kMaxPlayers; i++){
    for(int j = 0; j < PlayerData::kPieceNum; j++){
      if(player_[i].piece_[j].GetPosition() == box_index){
        if(player_in_box[0] == -1){
          player_in_box[0] = i;
          piece_turn[0] = player_[i].piece_[j].GetTurn();
        }else{
          assert(player_in_box[1] == -1 && "ERROR: MAS DE DOS FICHAS EN UN BOX");
          player_in_box[1] = i;
          piece_turn[1] = player_[i].piece_[j].GetTurn();
        }
      }
    }
  }

  if(player_in_box[1] == -1){
    return (IParcheesi::Color)player_in_box[0];
  }

  if(piece_turn[0] > piece_turn[1]){
    int temp_turn = piece_turn[0];
    piece_turn[0] = piece_turn[1];
    piece_turn[1] = temp_turn;

    int temp_player = player_in_box[0];
    player_in_box[0] = player_in_box[1];
    player_in_box[1] = temp_player;
  }

  return (IParcheesi::Color)player_in_box[box_piece_index];
}

void PauParcheesi::MovePiece(int pieceIndex, int target_box, int player_index){
  player_[player_index].piece_[pieceIndex].SetPosition(target_box);
  int turn = player_[player_index].piece_[pieceIndex].GetTurn();
  player_[player_index].piece_[pieceIndex].SetTurn(turn + 1);
}

void PauParcheesi::EatPiece(int pieceIndex, int target_box, int player_index){
  for(int i = 0; i < kMaxPlayers; i++){
    if(i == player_index) continue;
    for(int j = 0; j < PlayerData::kPieceNum; j++){
      if(player_[i].piece_[j].GetPosition() == target_box){
        player_[i].piece_[j].SetPosition(0);
        MovePiece(pieceIndex, target_box, player_index);
        //ChooseNextPiece(20);
        return;
      }
    }
  }
}

int PauParcheesi::AdjustPlayerPosition(int target_box, int current_box){
  int yellow_finish = finishPoint[(int)IParcheesi::Color::Yellow];
  if(current_box < yellow_finish && target_box > yellow_finish){
    return target_box - yellow_finish;
  }
  return target_box;
}

void PauParcheesi::HandleFinalSectionEntry(int target_box, int pieceIndex, int player_index){
  int diff = target_box - finishPoint[player_index];
  int base = finishPoint[(int)IParcheesi::Color::Yellow];
  int new_pos = base + diff;

  player_[player_index].piece_[pieceIndex].SetPosition(new_pos);

  int turn = player_[player_index].piece_[pieceIndex].GetTurn();
  player_[player_index].piece_[pieceIndex].SetTurn(turn + 1);
}

bool PauParcheesi::IsThereABridge(int current_box, int target_box, int finish_yellow_point){
  int i;
  if(current_box <= finish_yellow_point && target_box >= 1){
    for(i = current_box; i < finish_yellow_point; i++){
      if(CountPiecesOnBox(i) == 2) return true;
    }
    for(i = 1; i < target_box; i++){
      if(CountPiecesOnBox(i) == 2) return true;
    }
  }else{
    for(i = current_box; i < target_box; i++){
      if(CountPiecesOnBox(i) == 2) return true;
    }
  }
  return false;
}
