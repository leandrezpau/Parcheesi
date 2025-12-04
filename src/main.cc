#include "Pau_Parcheesi.h"
#include "game.h"
#include <windows.h>

int main(){
  system("chcp 65001");

  PauParcheesi myp;
  
  PauPlayer player1;
  PauPlayer player2;
  PauPlayer player3;

  const IPlayer* players[4]={&player1,&player2,&player3,nullptr};
  Game g{myp,players};

  while(g.IsGameOver() == IParcheesi::Color::None){
    g.PlayTurn();
  }

  return 0;
}

