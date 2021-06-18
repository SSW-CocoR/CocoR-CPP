#!/bin/sh
#myvalgrind --leak-check=full
../Coco -frames .. $1.ATG > output.txt

if cmp trace.txt $1_Trace.txt
then
	echo $1_Trace passed
fi

if cmp output.txt $1_Output.txt
then
	echo $1_Compilation passed
fi
