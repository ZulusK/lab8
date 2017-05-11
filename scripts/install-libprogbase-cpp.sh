#!/bin/sh

wget -O libprogbase-cpp.zip https://github.com/PublicHadyniak/libprogbase-cpp/archive/master.zip
unzip libprogbase-cpp.zip -d libprogbase-cpp
cd libprogbase-cpp/libprogbase-cpp-master ; cmake . ; sudo make install
cd ../../ ; rm libprogbase-cpp/ libprogbase-cpp.zip -rf