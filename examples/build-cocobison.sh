../src/Coco -frames ../src bison.atg
g++ -g -Wall -o cocobison Parser.cpp Scanner.cpp cocobison.cpp
#./cocobison "postgresql-13.3/src/backend/parser/gram.y"

