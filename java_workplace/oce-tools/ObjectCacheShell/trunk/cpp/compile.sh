#!/bin/bash

g++ -Wall -g reloadcache.cpp -I$OCE_HOME -I/usr/local/xce/include -I$OCE_HOME/release-newarch/tmp/UtilSlice/cpp -I$OCE_HOME/util/cpp/ -I/opt/Ice-3.3/include -I$OCE_HOME/UtilCxx/src -I$OCE_HOME/IceExt/src/ -I$OCE_HOME/release-newarch/tmp/OceSlice/cpp -I$OCE_HOME/DbCxxPool/src -I/usr/include/mysql -L$OCE_HOME/release-newarch -L/usr/lib64 -L./Ice-3.3.1/lib64 -L./lib64 -L/usr/local/xce/lib -lIce -lIceUtil -lOceSlice -lUtilCxx -lUtilSlice -lcrypto -lIceBox -lIceGrid -lGlacier2 -lboost_regex -lIceExt -Wl,-rpath=Ice-3.3.1/lib64,-rpath-link=Ice-3.3.1/lib64 -o reloadcache
