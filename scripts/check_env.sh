#!/usr/bin/env bash

export OS_RELEASE=`uname -r`

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


set +e # Disable error
echo "$OS_RELEASE" | grep -E "Microsoft|microsoft-standard"  > /dev/null 2>&1
if [ $? -eq 0 ]
then
	# We are using WindowsSubsystemLinux
	echo "$OS_RELEASE" | grep "Microsoft" > /dev/null 2>&1
	if [ $? -eq 0 ]
	then
		# Only WSL 1 supported
		echo "--------- Installing for WSL 1 ----------"
		set -e # Enable error
		Install_Steps
	else
		echo -e "Project Wasabi, 2021\n"
		echo -e "Sorry, this version of WindowsSubsystemLinux (WSL) is not supported yet.\n"
		set -e # Enable error
		return 1
	fi
else
	# Other OS : Linux, IOS, ...
	echo "--------- Installing for Other OS [$OS_RELEASE] ----------"
	set -e # Enable error
	Install_Steps
fi

if [ "$WASABI_USE_WASM" = "yes" ] || [ "$WASABI_USE_WASM" = "no" ]
then
	echo "WASABI_USE_WASM='$WASABI_USE_WASM'"  
else
	echo "Error: WASABI_USE_WASM variable should have 'yes'/'no' value only, WASABI_USE_WASM='$WASABI_USE_WASM'"  
	return 1
fi