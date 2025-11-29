-- premake5.lua
workspace "Parcheesi"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Parcheesi"
    location "../build"
    includedirs { "../src", "../interface" }
    libdirs { "../libs" }

project "PauPlayer"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   includedirs { "../src", "../interface" }
   files {
      "../src/Pau_Player.h",
      "../src/Pau_Player.cc"
   }

project "ParcheesiBoard"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    includedirs { "../src", "../interface" }

    files {
        "../src/Pau_Parcheesi.h",
        "../src/Pau_Parcheesi.cc"
    }

project "Parcheesi"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    includedirs { "../src", "../interface" }

    links {
        "PauPlayer",
        "ParcheesiBoard"
    }

    files {
        "../interface/iparcheesi.h",
        "../interface/iplayer.h",
        "../interface/game.h",
        "../interface/game.cc",
        "../src/main.cc"
    }
