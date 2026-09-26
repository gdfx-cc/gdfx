#!/usr/bin/env bash
set -euo pipefail
#
# Usage:
# $ gdfx/scripts/create-game.sh MyGame [--no-git]
# $ cd MyGame
# $ ./scripts/build.sh debug

usage() {
	echo "Usage:"
	echo "  gdfx/scripts/create-game.sh MyGame [--no-git]"
	echo "  cd MyGame"
	echo "  ./scripts/build.sh debug"
}

if [ $# -lt 1 ] || [ $# -gt 2 ]; then
	usage
	exit 1
fi

APP_NAME="$1"
INIT_GIT=true

if [ $# -eq 2 ]; then
	if [ "$2" != "--no-git" ]; then
		usage
		exit 1
	fi
	INIT_GIT=false
fi

if [[ ! "${APP_NAME}" =~ ^[A-Za-z_][A-Za-z0-9_]*$ ]]; then
	echo "App name must be a valid C++ class name, such as MyGame or SpaceShooter2."
	exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GDFX_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
DEST_DIR="${PWD}/${APP_NAME}"

if [ -e "${DEST_DIR}" ]; then
	echo "Destination already exists: ${DEST_DIR}"
	exit 1
fi

cp -R "${GDFX_DIR}/template" "${DEST_DIR}"

replace_in_file() {
	local file="$1"
	local search="$2"
	local replace="$3"
	local escaped_replace

	escaped_replace="$(printf '%s' "${replace}" | sed 's/[&|]/\\&/g')"
	sed -i.bak "s|${search}|${escaped_replace}|g" "${file}"
	rm -f "${file}.bak"
}

cmake_path() {
	local path="$1"

	if command -v cygpath >/dev/null 2>&1; then
		cygpath -m "${path}"
	else
		printf '%s\n' "${path}"
	fi
}

find "${DEST_DIR}" -type f \( \
	-name "CMakeLists.txt" -o \
	-name "CMakePresets.json" -o \
	-name "*.cpp" -o \
	-name "*.hpp" -o \
	-name "*.h" -o \
	-name "*.rc" -o \
	-name "*.desktop" -o \
	-name "*.plist" -o \
	-name "*.sh" -o \
	-name "*.txt" -o \
	-name "*.md" \
\) -print0 | while IFS= read -r -d '' file; do
	replace_in_file "${file}" "GDFXApp" "${APP_NAME}"
	replace_in_file "${file}" "GDFX_APP" "${APP_NAME}"
done

replace_in_file "${DEST_DIR}/CMakeLists.txt" "\${CMAKE_CURRENT_LIST_DIR}/../gdfx" "$(cmake_path "${GDFX_DIR}")"

# rename things
mv "${DEST_DIR}/src/GDFXApp.cpp" "${DEST_DIR}/src/${APP_NAME}.cpp"
mv "${DEST_DIR}/src/GDFXApp.hpp" "${DEST_DIR}/src/${APP_NAME}.hpp"
mv "${DEST_DIR}/app/GDFXApp.desktop" "${DEST_DIR}/app/${APP_NAME}.desktop"

if [ "${INIT_GIT}" = true ]; then
	#if ! git -C "${DEST_DIR}" init -b main; then
	#	git -C "${DEST_DIR}" init
	#	git -C "${DEST_DIR}" branch -M main
	#fi
	git -C "${DEST_DIR}" init
	git -C "${DEST_DIR}" add .
	git -C "${DEST_DIR}" commit -m "first commit of ${APP_NAME}"
	git -C "${DEST_DIR}" branch -M main
fi

echo "Created ${DEST_DIR}"
echo "Build it with: cd \"${DEST_DIR}\" && ./scripts/build.sh debug"
