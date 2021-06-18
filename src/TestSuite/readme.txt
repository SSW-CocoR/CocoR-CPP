                          Test Suite for Coco/R
                                  Hanspeter Mössenböck

This directory contains a simple test suite, which checks if changes to Coco/R still
lead to the same results. It works as follows:

compile.bat Sample
runs Coco/R for the grammar Sample.ATG, which has all trace switches enabled so that
Coco/R dumps its internal data structures to the file trace.txt. It then copies the
following files:
   Scanner.cs       =>   TestSample_Scanner.cs
   Parser.cs        =>   TestSample_Parser.cs
   console output   =>   TestSample_Output.txt
   trace.txt        =>   TestSample_Trace.txt


compileall.bat
runs compile.bat for all ATG files in this directory. You should run this command
once before you start making changes to Coco/R.

check.bat Sample
runs Coco/R again on the file Sample.ATG and compares the generated scanner, parser,
trace.txt and console output to the previously saved files. If the files are identical the
command prints the message
   ++ passed TestSample_Compilation

otherwise it prints the message
   -- failed TestSample_Compilation


checkall.bat
runs check.bat for all ATG files in this directory. You should run this command after
every modification to Coco/R. The generated ++ passed or -- failed messages
indicate if the modification led to any differences in the output or in the internal data
structures of Coco/R.
