#include "Scanner.h"
#include "Parser.h"

using namespace CocoBison;

int main (int argc, char *argv[]) {

	if (argc == 2) {
		wchar_t *fileName = coco_string_create(argv[1]);
		CocoBison::Scanner scanner(fileName);
		CocoBison::Parser parser(&scanner);
		parser.Parse();
                if(parser.errors->count == 0) {
#ifdef PARSER_WITH_AST
                    if(parser.ast_root) {
                        parser.ast_root->dump_all();
                        //parser.ast_root->dump_pruned();
                    }
#endif
                }

		coco_string_delete(fileName);
	} else
		wprintf(_SC("-- No source file specified\n"));

	return 0;

}
