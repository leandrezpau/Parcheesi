#ifndef PAUPARCHEESI_CC
#define PAUPARCHEESI_CC

#include "Pau_Parcheesi.h"
#include "game.h"
#include <stdio.h>
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

PauParcheesi::Movement PauParcheesi::ApplyMovement(int piece_index, int player_index, int count) {
  // 1. Verificación recursiva (IA/Simulación)
  if (piece_index == -1) {
    for (int i = 0; i < PauPlayer::kPieceNum; i++) {
      PauParcheesi temp_board = *this;
      // Si al menos una pieza puede moverse legalmente, no es "NoMoves"
      if (temp_board.ApplyMovement(i, player_index, count) < Movement::IllegalPass) {
        return Movement::IllegalPass;
      }
    }
    return Movement::NoMoves;
  }

  PauPlayer::Piece* currentPiece = &players[player_index].piece[piece_index];
  int currentPos = currentPiece->position;
  int goalEntrance = finishPoint[player_index]; // La casilla donde comienza el pasillo final
  int finalGoalSquare = goalEntrance + 8;       // La casilla exacta de victoria

  // 2. Salida de Casa (Posición 0)
  if (currentPos == 0) {
    if (count == 5) {
      int startPos = startPoint[player_index];
      if (CountPiecesOnBox(startPos) == 2) {
        // Si hay barrera de otro color, no podemos salir
        if (ColorofPiece(startPos, 0) != static_cast<IParcheesi::Color>(player_index)) {
          return Movement::IllegalEntryBlocked;
        }
      }
      // Salida exitosa
      currentPiece->position = startPos;
      // Comprobamos si comemos a alguien al salir (opcional, depende reglas, aquí asumimos Normal)
      return Movement::Normal; 
    } else {
      return Movement::IllegalPieceAtHome;
    }
  }

  // 3. Determinar si estamos YA en el pasillo de meta (tramo final)
  bool isInGoalPath = (currentPos > goalEntrance) && (currentPos <= finalGoalSquare);

  // Si ya estamos dentro del pasillo, solo avanzamos hacia el final
  if (isInGoalPath) {
    int nextPos = currentPos + count;
    if (nextPos == finalGoalSquare) {
      currentPiece->position = nextPos;
      return Movement::End; // ¡Victoria con esta ficha!
    } else if (nextPos > finalGoalSquare) {
      return Movement::IllegalPastEnd; // Se pasó, no mueve
    } else {
      currentPiece->position = nextPos;
      return Movement::Normal;
    }
  }

  // 4. Lógica de Movimiento en el Tablero Principal (Wrap y Entrada a Meta)
  
  // Calculamos la distancia hasta la entrada de la meta considerando el ciclo 68->1
  int distanceToEntrance = 0;
  if (currentPos <= goalEntrance) {
    distanceToEntrance = goalEntrance - currentPos;
  } else {
    // Ejemplo: Estoy en 66, meta en 17. Distancia = (68 - 66) + 17 = 19
    distanceToEntrance = (68 - currentPos) + goalEntrance;
  }

  int nextPos;

  // ¿La tirada es suficiente para entrar al pasillo de meta?
  if (count > distanceToEntrance) {
    // Calculamos cuánto avanzamos DENTRO del pasillo
    int stepsInside = count - distanceToEntrance;
    nextPos = goalEntrance + stepsInside;

    if (nextPos == finalGoalSquare) {
      currentPiece->position = nextPos;
      return Movement::End;
    } else if (nextPos > finalGoalSquare) {
      return Movement::IllegalPastEnd;
    } else {
      // Entramos al pasillo, pero no llegamos al final todavía
      currentPiece->position = nextPos;
      return Movement::Normal;
    }
  } else {
      // 5. Movimiento normal por el tablero (sin entrar a meta aún)
      nextPos = currentPos + count;
      
      // Manejo del ciclo del tablero (68 -> 1)
      if (nextPos > 68) {
        nextPos -= 68;
      }


      for (int i = 0; i < PauParcheesi::kMaxPlayers; i++) {
        for (int e = 0; e < PauPlayer::kPieceNum; e++) {
          if (nextPos == players[i].piece[e].position) {
            // Si es otro color y no es seguro, COMER
            if (players[i].color != players[player_index].color && !IsBoxSafe(nextPos)) {
              currentPiece->position = nextPos;
              return Movement::Eat;
            }
          }
        }
      }

      // 7. Barreras / Puentes
      for (int i = 0; i < kSafePoints; i++) {
        if (nextPos == PauParcheesi::safePoint[i]) {
          if (CountPiecesOnBox(nextPos) == 2) return Movement::IllegalBridge;
        }
      }

      // Movimiento válido normal
      currentPiece->position = nextPos;
      return Movement::Normal;
  }
}

void PauParcheesi::SendPieceHome(int piece_index, int player_index){
  players[player_index].piece[piece_index].position = 0;
  players[player_index].piece[piece_index].turn = 0;
}

#endif // PAUPARCHEESI_CC