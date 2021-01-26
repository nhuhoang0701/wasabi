#!/usr/bin/env bash

# exit on error
set -e

export WASABI_ROOT_DIR=$(pwd)

./scripts/check_env.sh

./scripts/wasabi_external.sh

./scripts/wasabi_src.sh
