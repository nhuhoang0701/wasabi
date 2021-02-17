#!/usr/bin/env bash

# Install procedure
Install_Steps()
{
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
}


uname -r | egrep "Microsoft|microsoft-standard" > /dev/null
if [ $? -eq 0 ]
then
	# We are using WindowsSubsystemLinux
	uname -r | egrep "Microsoft" > /dev/null
	if [ $? -eq 0 ]
	then
		# Only WSL 1 supported
		Install_Steps
	else
		echo -e "Project Wasabi, 2021\n"
		echo -e "Sorry, this version of WindowsSubsystemLinux (WSL) is not supported yet.\n"
	fi
else
	# Other OS : Linux, IOS, ...
	Install_Steps
fi
