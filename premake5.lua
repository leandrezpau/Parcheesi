-- premake5.lua
workspace "Parcheesi"
   architecture "x64"
   configurations { "Debug", "Release" }
   startproject "Parcheesi"
   location "build"
   libdirs {"."}

project "PPlayer"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   files { "PPlayer.h", "PPlayer.cc"}

project "ParcheesiBoard"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"

   files { "PauParcheesi.cc", "PauParcheesi.h" }

project "Parcheesi"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   links {"PPlayer","ParcheesiBoard","AlejPlayer.lib"}

   files
   {
      "interface/iparcheesi.h",
      "interface/iplayer.h",
      "mock_parcheesi.h",
      "game.h", "game.cc",
      "main.cc"
   }
