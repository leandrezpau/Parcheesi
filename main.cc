#include "PauParcheesi.h"
#include "game.h"
#include "windows.h"
#include "aleJPlayer.h"

int main(){
  system("chcp 65001");
  PauParcheesi myp;

  AlejPlayer player1;
  AlejPlayer player2;
  AlejPlayer player3;

  const IPlayer* players[4]={&player1,&player2,&player3,nullptr};
  Game g{myp,players};

  while(g.IsGameOver() == IParcheesi::Color::None){
    g.PlayTurn();
  }

  return 0;
}

