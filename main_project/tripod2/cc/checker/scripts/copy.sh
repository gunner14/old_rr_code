#!/bin/bash
source vars

if [ -f ./trash ];then
  rm -rf ./trash
fi

Length=${#Aa[@]}

for((i=1;i<$Length;i++));do
  Bb=${Aa[$i]}
  BB=${AA[$i]}
  bb=${aa[$i]}
  b_b=${a_a[$i]}
  Xx=${Aa[0]}
  XX=${AA[0]}
  xx=${aa[0]}
  x_x=${a_a[0]}
  echo $Bb;
  hb=$b_b
  hx=$x_x
  dirx=$hx".checker"
  dirb=$hb".checker"
  f1x="get_"$x_x"_changedids_onehour.sh"
  f1b="get_"$b_b"_changedids_onehour.sh"
  f2x=$x_x".checker.sh"
  f2b=$b_b".checker.sh"
  echo "cp -r $dirx $dirb"
  if [ -d $dirb ];then
    mkdir -p ./trash
    rm -rf ./trash/$dirb
    mv -f $dirb ./trash
  fi
  cp -r $dirx $dirb
  rm $dirb/result/* $dirb/log/* $dirb/input/*
  mv $dirb/$f1x $dirb/$f1b
  mv $dirb/$f2x $dirb/$f2b
  SED_Aa="sed -i s/$Xx/$Bb/g $dirb/*.sh"
  SED_a_a="sed -i s/$x_x/$b_b/g $dirb/*.sh"
  echo $SED_Aa && eval $SED_Aa
  echo $SED_a_a && eval $SED_a_a
  
done
