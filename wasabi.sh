#!/usr/bin/env bash
[[ "${BASH_SOURCE[0]}" != "${0}" ]] && echo "*** ERROR *** source does not work with wasabi.sh" && return 1


# exit on error
set -e

./scripts/external.sh

source ./scripts/check_env.sh

echo
echo "-----------------------------------"
echo "run:"
echo "source ./scripts/set_env.sh"
echo "to set your environment variables"
