#!/usr/bin/env bash

export PYTHON=python3
echo -----------------------------------
echo ----- check $PYTHON install  ------
dpkg -s $PYTHON &> /dev/null 
if [ $? -ne 0 ] 
then
	echo "Error: $PYTHON not installed. Please install it:"  
	echo sudo apt-get update
	echo sudo apt-get install $PYTHON
	exit 1
else
	echo    "$PYTHON installed."
fi

export MAKE=make
echo -----------------------------------
echo ------ check $MAKE install  -------
dpkg -s $MAKE &> /dev/null 
if [ $? -ne 0 ]
then
	echo "Error: $MAKE not installed. Please install it:"  
	echo sudo apt-get update
	echo sudo apt-get install $MAKE
	exit 1
else
	echo    "$MAKE installed."
fi

export CMAKE=cmake
echo -----------------------------------
echo ------ check $CMAKE install  ------
dpkg -s $CMAKE &> /dev/null 
if [ $? -ne 0 ]
then
	echo "Error: $CMAKE not installed. Please install it:"  
	echo sudo apt-get update
	echo sudo apt-get install $CMAKE
	exit 1
else
	echo    "$CMAKE installed."
fi
