clear

./tools/premake5 --file=tools/premake5.lua gmake
make -C build config=debug Parcheesi

if [ $? -eq 0 ]; then
  echo "\033[0;32mARCHIVO EJECUTABLE COMPILADO\033[0m"
else
  echo "\033[0;31mARCHIVO EJECUTABLE NO COMPILADO\033[0m"
fi