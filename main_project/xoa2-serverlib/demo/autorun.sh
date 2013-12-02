#! /bin/bash

cd ../serverlib;
make clean;
make;
make install;

cd ../demo;
make clean;
make;
