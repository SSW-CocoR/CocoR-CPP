#include "SymbolTable.h"
#include "Parser.h"

namespace Taste {

SymbolTable::SymbolTable(Parser *parser): undef(0), integer(1), boolean(2), var(0), proc(1), scope(2) {
	errors = parser->errors;
	topScope = NULL;
	curLevel = -1;
	undefObj = new Obj();
	undefObj->name  = coco_string_create("undef"); undefObj->type = undef; undefObj->kind = var;
	undefObj->adr = 0; undefObj->level = 0; undefObj->next = NULL;
}

SymbolTable::~SymbolTable() {
	delete undefObj;
	delete topScope;
}

void SymbolTable::Err(const wchar_t* msg) {
	errors->Error(0, 0, msg);
}


// open a new scope and make it the current scope (topScope)
void SymbolTable::OpenScope () {
	Obj *scop = new Obj();
	scop->name = coco_string_create(""); scop->kind = scope;
	scop->locals = NULL; scop->nextAdr = 0;
	scop->next = topScope; topScope = scop;
	curLevel++;
}


// close the current scope
void SymbolTable::CloseScope () {
	Obj *scop = topScope;
	topScope = topScope->next; curLevel--;
	scop->next = NULL;
	delete scop;
}

// create a new object node in the current scope
Obj* SymbolTable::NewObj (const wchar_t* name, int kind, int type) {
	Obj *p, *last, *obj = new Obj();
	obj->name = coco_string_create(name); obj->kind = kind; obj->type = type;
	obj->level = curLevel;
	p = topScope->locals; last = NULL;
	while (p != NULL) {
		if (coco_string_equal(p->name, name)) Err(_SC("name declared twice"));
		last = p; p = p->next;
	}
	if (last == NULL) topScope->locals = obj; else last->next = obj;
	if (kind == var) obj->adr = topScope->nextAdr++;
	return obj;
}


// search the name in all open scopes and return its object node
Obj* SymbolTable::Find (const wchar_t* name) {
	Obj *obj, *scope;
	scope = topScope;
	while (scope != NULL) {  // for all open scopes
		obj = scope->locals;
		while (obj != NULL) {  // for all objects in this scope
			if (coco_string_equal(obj->name, name)) return obj;
			obj = obj->next;
		}
		scope = scope->next;
	}
	wchar_t str[100];
	coco_swprintf(str, 100, _SC("%") _SFMT _SC(" is undeclared"), name);
	Err(str);
	return undefObj;
}

}; // namespace
