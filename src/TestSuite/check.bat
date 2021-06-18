@..\Coco -frames .. %1.ATG > output.txt
@Compare trace.txt %1_Trace.txt %1_Trace
@Compare output.txt %1_Output.txt %1_Compilation 22
@Compare Parser.cs %1_Parser.cs %1_Parser
@Compare Scanner.cs %1_Scanner.cs %1_Scanner
