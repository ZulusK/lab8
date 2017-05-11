#!/bin/sh

wget -O libprogbase.zip https://github.com/PublicHadyniak/libprogbase-cpp/archive/master.zip
unzip libprogbase-cpp.zip -d libprogbase-cpp
cd libprogbase/libprogbase-cpp-master ; sudo make install
cd ../../ ; rm libprogbase-cpp/ libprogbase-cpp.zip -rf
wget -O libprogbase.zip https://github.com/PublicHadyniak/libprogbase/archive/master.zip
unzip libprogbase.zip -d libprogbase
cd libprogbase/libprogbase-master ; sudo make install
cd ../../ ; rm libprogbase/ libprogbase.zip -rf
