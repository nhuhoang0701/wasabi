#!/usr/bin/env bash

export PYTHON=python3
export MAKE=make
export CMAKE=cmake

echo -----------------------------------
echo ----- check $PYTHON install  ------
dpkg -s $PYTHON &> /dev/null 

if [ $? -ne 0 ]

        then
            echo "$PYTHON not installed. Installing..."  
            sudo apt-get update
            sudo apt-get install $PYTHON

        else
            echo    "$PYTHON installed."
    fi

echo -----------------------------------
echo ------ check $MAKE install  -------
dpkg -s $MAKE &> /dev/null 

if [ $? -ne 0 ]

        then
            echo "$MAKE not installed. Installing..."  
            sudo apt-get update
            sudo apt-get install $MAKE

        else
            echo    "$MAKE installed."
    fi

echo -----------------------------------
echo ------ check $CMAKE install  ------
dpkg -s $CMAKE &> /dev/null 

if [ $? -ne 0 ]

        then
            echo "$CMAKE not installed. InstalLing..."  
            sudo apt-get update
            sudo apt-get install $CMAKE

        else
            echo    "$CMAKE installed."
    fi