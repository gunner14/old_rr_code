#!/bin/sh
echo ""
echo "----stage2: make base"
echo ""
echo ">> cmake ."
cmake .
if [ "$?" -eq 0 ]; then
	echo ">> make jars"
	make jars
	if [ "$?" -eq 0 ]; then
		return 0
	else
		return 2
	fi
else
        return 1
fi

