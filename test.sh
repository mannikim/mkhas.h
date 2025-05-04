#!/bin/sh
# see LICENSE file for copyright and license information

USAGE="Usage: $0 <SRC_DIR> <BUILD_DIR> [OPTIONS]"

test "$1" = "-h" &&
	echo $USAGE &&
	exit 0

test -z "$1" && 
	echo $USAGE &&
	exit 1

test -z "$2" && 
	echo $USAGE &&
	exit 1

SRC_DIR="$1"
BUILD_DIR="$2"

VERBOSITY=1
DRY=""

if ! [ -z $3 ]; then
	while [[ ! -z "$3" ]]; do
		case "$3" in
			-d)
				DRY=1
			;;
			-s) 
				VERBOSITY=0
			;;
			-v) 
				VERBOSITY=2
			;;
			*) 
				echo "Unrecognized option: $3"
				exit 1
			;;
		esac
		shift
	done
fi

ERR=()

SRC_FILES=$(find "$SRC_DIR" -type f -name "*.c")
BUILD_FILES=$()

test -z "$SRC_FILES" &&
	echo No C source file was found in directory ./$SRC_DIR &&
	exit 1

test -z "$DRY" || {
	echo "$SRC_FILES" &&
	exit 0
}

_echo() {
	test "$VERBOSITY" = "0" && return
	echo $@
}

_echov() {
	test "$VERBOSITY" = "2" || return
	echo $@
}

_echo "RUNNING TESTS"
_echo

for script in $SRC_FILES; do
	relative="$BUILD_DIR/${script%.c}"
	test -f "$relative" || { 
		echo Could not find executable $relative &&
		exit 1
	}
	_echo $relative from $script
	tmp=$(./$relative 2>&1)
	out=$(echo $?)
	_echov "$tmp"
	if [ 0 -ne $out ]; then
		ERR+=($script)
	fi
done

if ! [ -z "$ERR" ]; then
	_echo
	_echo "FAILED"
	_echo
	for file in "$ERR"; do
		_echo $file
		tmp=$(grep "##DSC " $file | sed -e "s/##DSC //")
		test -z "$tmp" || {
			_echo DESCRIPTION && _echo $tmp
		}
		tmp=$(grep "##ERR " $file | sed -e "s/##ERR //")
		test -z "$tmp" || {
			_echo ERROR && _echo $tmp
		}
		_echo
	done
	exit 2
fi

_echo
_echo "OK"
exit 0
