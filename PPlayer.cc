#include "PPlayer.h"
#include "PauParcheesi.h"
#include "game.h"

#include "PPlayer.h"
#include "PauParcheesi.h"
#include "game.h"

int PPlayer::DecideMove(const IParcheesi& parcheesi, int current_player_index, int dice_roll) const {
  PauParcheesi base_game_state = static_cast<const PauParcheesi&>(parcheesi);

  if (dice_roll == 5) {
    for (int i = 0; i < PPlayer::kPieceNum; i++) {
      if (base_game_state.players[current_player_index].piece[i].position == 0) {
        
        PauParcheesi temp_board = base_game_state; 
        PauParcheesi::Movement move = temp_board.ApplyMovement(i, current_player_index, dice_roll);
        
        if (move < PauParcheesi::Movement::IllegalPass) {
          return i;
        }
      }
    }
  }

  for (int i = 0; i < PPlayer::kPieceNum; i++) {
    if (base_game_state.players[current_player_index].piece[i].position == 0) continue;

    PauParcheesi temp_board = base_game_state;
    PauParcheesi::Movement move = temp_board.ApplyMovement(i, current_player_index, dice_roll);
    
    if (move == PauParcheesi::Movement::Eat) {
      return i;
    }
  }

  for (int i = 0; i < PPlayer::kPieceNum; i++) {
    if (base_game_state.players[current_player_index].piece[i].position == 0) continue;

    PauParcheesi temp_board = base_game_state;
    PauParcheesi::Movement move = temp_board.ApplyMovement(i, current_player_index, dice_roll);

    if (move < PauParcheesi::Movement::IllegalPass && move != PauParcheesi::Movement::NoMoves) {
      return i;
    }
  }

  return -1;
}