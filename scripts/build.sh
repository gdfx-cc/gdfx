#!/usr/bin/env bash
set -euo pipefail

CONFIG="${1:-debug}"

case "${CONFIG}" in
	debug|Debug)
		PRESET="debug"
		;;
	release|Release)
		PRESET="release"
		;;
	*)
		echo "Usage: $0 [debug|release]"
		exit 1
		;;
esac

cmake --preset "${PRESET}"
cmake --build --preset "${PRESET}"

