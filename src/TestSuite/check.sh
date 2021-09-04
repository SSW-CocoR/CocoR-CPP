#!/bin/sh
#myvalgrind --leak-check=full
../Coco -frames .. $1.ATG > output.txt

#cp  trace.txt $1_Trace.txt
if cmp trace.txt $1_Trace.txt
then
	echo $1_Trace passed
fi

if cmp output.txt $1_Output.txt
then
	echo $1_Compilation passed
fi

#cp Parser.cpp $1_Parser.cpp
if cmp Parser.cpp $1_Parser.cpp
then
	echo $1_Parser passed
fi

#cp Scanner.cpp $1_Scanner.cpp
if cmp Scanner.cpp $1_Scanner.cpp
then
	echo $1_Scanner passed
fi
