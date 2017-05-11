#!/bin/sh

wget -O libprogbase.zip https://github.com/PublicHadyniak/libprogbase/archive/master.zip
unzip libprogbase.zip -d libprogbase
cd libprogbase/libprogbase-master ; sudo make install
cd ../../ ; rm libprogbase/ libprogbase.zip -rf

wget -O libprogbase.zip https://codeload.github.com/PublicHadyniak/libprogbase-cpp/zip/master
unzip libprogbase-cpp.zip -d libprogbase-cpp
cd libprogbase/libprogbase-cpp-master ; sudo make install
cd ../../ ; rm libprogbase-cpp/ libprogbase-cpp.zip -rf