C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat
@echo ---------------------------------------------------
@echo ---------------     Piece.cc      -----------------
cl.exe /c piece.cc
@echo -------------     End Piece.cc      ---------------
@echo ---------------------------------------------------
@echo ---------------     Player.cc      ----------------
cl.exe /c player.cc
@echo -------------     End Player.cc      --------------
@echo ---------------------------------------------------
@echo --------------     Parcheesi.cc      --------------
cl.exe /c parcheesi.cc
@echo ------------     End Parcheesi.cc      ------------
@echo ---------------------------------------------------
@echo ---------------     Main.cc      ------------------
cl.exe /c main.cc
@echo --------------    End Main.cc      ----------------
@echo ---------------------------------------------------


@echo ---------------------------------------------------
@echo ----------------  Main Go File  -------------------
cl.exe /oparcheesi.exe *.obj
del *.obj