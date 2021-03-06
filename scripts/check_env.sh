#!/usr/bin/env bash

export OS_RELEASE=`uname -r`

# Install procedure
Install_Steps()
{
	export PYTHON=${PYTHON:-python3}
	echo -----------------------------------
	echo ----- check 'python' install  ------
	if ! command -v $PYTHON &> /dev/null
	then
		echo "Error: $PYTHON not installed. Please install it:"  
		return 1
	else
		echo    "$PYTHON installed."
	fi

	export CMAKE=${CMAKE:-cmake}
	echo -----------------------------------
	echo ------ check 'cmake' install  ------
	if ! command -v $CMAKE &> /dev/null
	then
		echo "Error: $CMAKE not installed. Please install it:"  
		return 1
	else
		echo    "$CMAKE installed."
	fi

	export CTEST=${CTEST:-ctest}
	echo -----------------------------------
	echo ------ check 'ctest' install  ------
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

if [ "$WASABI_PLATFORM_TARGET" = "linux" ] || [ "$WASABI_PLATFORM_TARGET" = "wasm" ]
then
	echo "WASABI_PLATFORM_TARGET='$WASABI_PLATFORM_TARGET'"  
else
	echo "Error: WASABI_PLATFORM_TARGET variable should have 'wasm'/'linux' value only, WASABI_PLATFORM_TARGET='$WASABI_PLATFORM_TARGET'"  
	return 1
fi