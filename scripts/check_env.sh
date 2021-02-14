#!/usr/bin/env bash

export PYTHON=${PYTHON:-python3}
echo -----------------------------------
echo ----- check $PYTHON install  ------
if ! command -v $PYTHON &> /dev/null
then
	echo "Error: $PYTHON not installed. Please install it:"  
	return 1
else
	echo    "$PYTHON installed."
fi

export MAKE=${MAKE:-make}
echo -----------------------------------
echo ------ check $MAKE install  -------
if ! command -v $MAKE &> /dev/null
then
	echo "Error: $MAKE not installed. Please install it:"  
	return 1
else
	echo    "$MAKE installed."
fi

export CMAKE=${CMAKE:-cmake}
echo -----------------------------------
echo ------ check $CMAKE install  ------
if ! command -v $CMAKE &> /dev/null
then
	echo "Error: $CMAKE not installed. Please install it:"  
	return 1
else
	echo    "$CMAKE installed."
fi

export CTEST=${CTEST:-ctest}
echo -----------------------------------
echo ------ check $CTEST install  ------
if ! command -v $CTEST &> /dev/null
then
	echo "Error: $CTEST not installed. Please install it:"  
	return 1
else
	echo    "$CTEST installed."
fi

