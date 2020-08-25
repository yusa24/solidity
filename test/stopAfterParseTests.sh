#! /usr/bin/env bash

REPO_ROOT=$(readlink -f "$(dirname "$0")"/..)
SOLIDITY_BUILD_DIR=${SOLIDITY_BUILD_DIR:-${REPO_ROOT}/build}
SOLC=${SOLIDITY_BUILD_DIR}/solc/solc
SPLITSOURCES=${REPO_ROOT}/scripts/splitSources.py

FILETMP=$(mktemp -d)
cd "$FILETMP" || exit 1


function testFile()
{
	ALLOUTPUT=$($SOLC --combined-json ast,compact-format --pretty-json "$@" --stop-after parsing 2>&1)
	if test $? -ne 0; then
		# solc returned failure. Compilation errors and unimplemented features
		# are okay, everything else is a failed test (segfault)
		if ! echo "$ALLOUTPUT" | grep -e "Unimplemented feature:" -e "Error:" -q; then
			echo -n "Test failed on ";
			echo "$@"
			echo "$ALLOUTPUT"
			exit 1;
		fi
	else
		echo -n .
	fi
}

while read -r file; do
	OUTPUT=$($SPLITSOURCES "$file")
	if [ $? -ne 1 ]
	then
		testFile $OUTPUT
		rm "${FILETMP:?}/"* -r
	else
		testFile "$file"
	fi
done < <(find "${REPO_ROOT}/test" -iname "*.sol")
echo
