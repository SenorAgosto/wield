#!/bin/sh

export SOURCES=`find wield/ -name *.cpp -or -name *.c | grep -v unit_test`
export HEADERS=`find wield/ -name *.hpp -or -name *.h | grep -v unit_test`

export CXXOPTS='--std=c++11 -I. -I/Volumes/austirg/homebrew/include'

# HTML documentation
cldoc generate $CXXOPTS --  --report --type html --output _docs --merge . --language c++ --basedir wield $SOURCES $HEADERS 
