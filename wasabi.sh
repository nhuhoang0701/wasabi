#!/usr/bin/env bash
[[ "${BASH_SOURCE[0]}" != "${0}" ]] && echo "*** ERROR *** source does not work with wasabi.sh" && return 0


# exit on error
set -e

source ./scripts/check_env.sh
./scripts/wasabi_external.sh
./scripts/wasabi_src.sh

echo
echo "-----------------------------------"
echo "run:"
echo "source ./scripts/set_env.sh"
echo "to set your environment variables"
