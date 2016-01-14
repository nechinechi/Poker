#!/usr/local/bin/zsh

../../sys/BinCompile.sh ../../src/ 7 5 PokerExec.c

if [ $? -eq 1 ]
then
  echo "compile error"
  exit $?
fi

./PokerExec result 10000 Deck_02.txt 0
