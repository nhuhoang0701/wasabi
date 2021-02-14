
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

