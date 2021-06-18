#!/bin/sh
../Coco -frames .. $1.ATG > $1_Output.txt
cp trace.txt $1_Trace.txt
cp Parser.cpp $1_Parser.cpp
cp Scanner.cpp $1_Scanner.cpp
