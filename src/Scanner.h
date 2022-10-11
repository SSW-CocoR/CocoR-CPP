/*----------------------------------------------------------------------
Compiler Generator Coco/R,
Copyright (c) 1990, 2004 Hanspeter Moessenboeck, University of Linz
extended by M. Loeberbauer & A. Woess, Univ. of Linz
ported to C++ by Csaba Balazs, University of Szeged
with improvements by Pat Terry, Rhodes University

This program is free software; you can redistribute it and/or modify it 
under the terms of the GNU General Public License as published by the 
Free Software Foundation; either version 2, or (at your option) any 
later version.

This program is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
for more details.

You should have received a copy of the GNU General Public License along 
with this program; if not, write to the Free Software Foundation, Inc., 
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

As an exception, it is allowed to write an extension of Coco/R that is
used as a plugin in non-free software.

If not otherwise stated, any source code generated by Coco/R (other than 
Coco/R itself) does not fall under the GNU General Public License.
-----------------------------------------------------------------------*/


#if !defined(Coco_COCO_SCANNER_H__)
#define Coco_COCO_SCANNER_H__

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// io.h and fcntl are used to ensure binary read from streams on windows
#if _MSC_VER >= 1300
#include <io.h>
#include <fcntl.h>
#endif

#define WITHOUT_WCHAR

#ifdef WITHOUT_WCHAR
#define wchar_t char
#define _CHFMT "c"
#define _SFMT "s"
#define _SC(s) s
#define fputws fputs
#define wprintf printf
#define swprintf snprintf
#define fwprintf fprintf
#define fwscanf fscanf
#define swscanf sscanf
#define wcslen strlen
#define wcscpy strcpy
#define wcsncpy strncpy
#define wcscmp strcmp
#define wcsncmp strncmp
#define wcschr strchr
#define wcsrchr strrchr
#define wcscasecmp strcasecmp
#define wcsncasecmp strncasecmp

#if _MSC_VER >= 1400
#define coco_swprintf snprintf_s
#elif _MSC_VER >= 1300
#define coco_swprintf _snprintf
#elif defined __MINGW32__
#define coco_swprintf _snprintf
#else
// assume every other compiler knows sprintf
#define coco_swprintf snprintf
#endif

#define COCO_WCHAR_MAX 255

#else
#include <wchar.h>
#define _CHFMT L"lc"
#define _SFMT L"ls"
#define _SC(s) L##s

#if _MSC_VER >= 1400
#define coco_swprintf swprintf_s
#elif _MSC_VER >= 1300
#define coco_swprintf _snwprintf
#elif defined __MINGW32__
#define coco_swprintf _snwprintf
#else
// assume every other compiler knows swprintf
#define coco_swprintf swprintf
#endif

#define COCO_WCHAR_MAX 65535

#endif

#define COCO_MIN_BUFFER_LENGTH 1024
#define COCO_MAX_BUFFER_LENGTH (64*COCO_MIN_BUFFER_LENGTH)
#define COCO_HEAP_BLOCK_SIZE (64*1024)
#define COCO_CPP_NAMESPACE_SEPARATOR _SC(':')

namespace Coco {


// string handling, wide character
wchar_t* coco_string_create(const wchar_t *value);
wchar_t* coco_string_create(const wchar_t *value, int startIndex);
wchar_t* coco_string_create(const wchar_t *value, int startIndex, int length);
wchar_t* coco_string_create_upper(const wchar_t* data);
wchar_t* coco_string_create_lower(const wchar_t* data);
wchar_t* coco_string_create_lower(const wchar_t* data, int startIndex, int dataLen);
wchar_t* coco_string_create_append(const wchar_t* data1, const wchar_t* data2);
wchar_t* coco_string_create_append(const wchar_t* data, const int value);
void  coco_string_delete(wchar_t* &data);
int   coco_string_length(const wchar_t* data);
bool  coco_string_endswith(const wchar_t* data, const wchar_t *value);
int   coco_string_indexof(const wchar_t* data, const int value);
int   coco_string_lastindexof(const wchar_t* data, const int value);
void  coco_string_merge(wchar_t* &data, const wchar_t* value);
bool  coco_string_equal(const wchar_t* data1, const wchar_t* data2);
bool  coco_string_equal_nocase(const wchar_t* data1, const wchar_t* data2);
bool  coco_string_equal_n(const wchar_t* data1, const wchar_t* data2, size_t size);
bool  coco_string_equal_nocase_n(const wchar_t* data1, const wchar_t* data2, size_t size);
int   coco_string_compareto(const wchar_t* data1, const wchar_t* data2);
int   coco_string_compareto_nocase(const wchar_t* data1, const wchar_t* data2);
unsigned int coco_string_hash(const wchar_t* data);
unsigned int coco_string_hash(const wchar_t* data, size_t size);

#ifndef WITHOUT_WCHAR
// string handling, ascii character
wchar_t* coco_string_create(const char *value);
void  coco_string_delete(char* &data);
#endif
char* coco_string_create_char(const wchar_t *value);

template<typename T>
class TArrayList
{
	T *Data;
public:
	typedef int tsize_t;
	tsize_t Count;
	tsize_t Capacity;

	TArrayList() {
                Count = 0;
                Capacity = 10;
                Data = new T[ Capacity ];
        }
	virtual ~TArrayList() {
                delete [] Data;
        }

	void Add(T value) {
                if (Count < Capacity) {
                        Data[Count] = value;
                        Count++;
                } else {
                        Capacity *= 2;
                        T* newData = new T[Capacity];
                        for (tsize_t i=0; i<Count; i++) {
                                newData[i] = Data[i];		// copy
                        }
                        newData[Count] = value;
                        Count++;
                        delete [] Data;
                        Data = newData;
                }
        }

        //return the previous value
	T Set(tsize_t index, T value) {
                if (0<=index && index<Count) {
                        T *rv = Data[index];
                        Data[index] = value;
                        return rv;
                }
                return NULL;
        }

	void Remove(T value) {
                for (tsize_t i=0; i<Count; i++) {
                        if (Data[i] == value) {
                                for (tsize_t j=i+1; j<Count; j++)
                                        Data[j-1] = Data[j];
                                Count--;
                                break;
                        }
                }
        }

	T Pop() {
                if(Count == 0) return NULL;
                return Data[--Count];
        }

	T Top() {
                if(Count == 0) return NULL;
                return Data[Count-1];
        }

	void Clear() {
                memset(Data, 0, Capacity*sizeof(T));
                Count = 0;
        }

	T operator[](tsize_t index) {
                if (0<=index && index<Count)
                        return Data[index];
		wprintf(_SC("--- index out of bounds access, position: %d\n"), index);
		exit(1);
        }
};

class Token
{
public:
	int kind;     // token kind
	int pos;      // token position in bytes in the source text (starting at 0)
	int charPos;  // token position in characters in the source text (starting at 0)
	int col;      // token column (starting at 1)
	int line;     // token line (starting at 1)
	wchar_t* val; // token value
	Token *next;  // ML 2005-03-11 Peek tokens are kept in linked list

	Token();
        Token *Clone();
	~Token();
};

class Buffer {
// This Buffer supports the following cases:
// 1) seekable stream (file)
//    a) whole stream in buffer
//    b) part of stream in buffer
// 2) non seekable stream (network, console)
private:
	unsigned char *buf; // input buffer
	int bufCapacity;    // capacity of buf
	int bufStart;       // position of first byte in buffer relative to input stream
	int bufLen;         // length of buffer
	int fileLen;        // length of input stream (may change if the stream is no file)
	int bufPos;         // current position in buffer
	FILE* stream;       // input stream (seekable)
	bool isUserStream;  // was the stream opened by the user?

	int ReadNextStreamChunk();
	bool CanSeek();     // true if stream can be seeked otherwise false

public:
	static const int EoF = COCO_WCHAR_MAX + 1;

	Buffer(FILE* s, bool isUserStream);
	Buffer(const unsigned char* buf, int len);
	Buffer(Buffer *b);
	virtual ~Buffer();

	virtual void Close();
	virtual int Read();
	virtual int Peek();
	virtual wchar_t* GetString(int beg, int end);
	virtual int GetPos();
	virtual void SetPos(int value);
};

class UTF8Buffer : public Buffer {
public:
	UTF8Buffer(Buffer *b) : Buffer(b) {};
	virtual int Read();
};

//-----------------------------------------------------------------------------------
// StartStates  -- maps characters to start states of tokens
//-----------------------------------------------------------------------------------
class StartStates {
private:
	class Elem {
	public:
		int key, val;
		Elem *next;
		Elem(int key, int val) { this->key = key; this->val = val; next = NULL; }
	};

	Elem **tab;

public:
	StartStates() { tab = new Elem*[128]; memset(tab, 0, 128 * sizeof(Elem*)); }
	virtual ~StartStates() {
		for (int i = 0; i < 128; ++i) {
			Elem *e = tab[i];
			while (e != NULL) {
				Elem *next = e->next;
				delete e;
				e = next;
			}
		}
		delete [] tab;
	}

	void set(int key, int val) {
		Elem *e = new Elem(key, val);
		int k = ((unsigned int) key) % 128;
		e->next = tab[k]; tab[k] = e;
	}

	int state(int key) {
		Elem *e = tab[((unsigned int) key) % 128];
		while (e != NULL && e->key != key) e = e->next;
		return e == NULL ? 0 : e->val;
	}
};

//-------------------------------------------------------------------------------------------
// KeywordMap  -- maps strings to integers (identifiers to keyword kinds)
//-------------------------------------------------------------------------------------------
class KeywordMap {
private:
	class Elem {
	public:
		wchar_t *key;
		int val;
		Elem *next;
		Elem(const wchar_t *key, int val) { this->key = coco_string_create(key); this->val = val; next = NULL; }
		virtual ~Elem() { coco_string_delete(key); }
	};

	Elem **tab;

public:
	KeywordMap() { tab = new Elem*[128]; memset(tab, 0, 128 * sizeof(Elem*)); }
	virtual ~KeywordMap() {
		for (int i = 0; i < 128; ++i) {
			Elem *e = tab[i];
			while (e != NULL) {
				Elem *next = e->next;
				delete e;
				e = next;
			}
		}
		delete [] tab;
	}

	void set(const wchar_t *key, int val) {
		Elem *e = new Elem(key, val);
		int k = coco_string_hash(key) % 128;
		e->next = tab[k]; tab[k] = e;
	}

	int get(const wchar_t *key, size_t size, int defaultVal, bool ignoreCase) {
		Elem *e = tab[coco_string_hash(key, size) % 128];
                if(ignoreCase) {
		    while (e != NULL && !coco_string_equal_nocase_n(e->key, key, size)) e = e->next;
                }
                else {
		    while (e != NULL && !coco_string_equal_n(e->key, key, size)) e = e->next;
                }
		return e == NULL ? defaultVal : e->val;
	}
};

class Scanner {
private:
	void *firstHeap;
	void *heap;
	void *heapTop;
	void **heapEnd;

	unsigned char EOL;
	int eofSym;
	int noSym;
	int maxT;
	StartStates start;
	KeywordMap keywords;

	Token *t;         // current token
	wchar_t *tval;    // text of current token
	int tvalLength;   // length of text of current token
	int tlen;         // length of current token

	Token *tokens;    // list of tokens already peeked (first token is a dummy)
	Token *pt;        // current peek token

	int ch;           // current input character

	int pos;          // byte position of current character
	int charPos;      // position by unicode characters starting with 0
	int line;         // line number of current character
	int col;          // column number of current character
	int oldEols;      // EOLs that appeared in a comment;

	char *parseFileName;

	void CreateHeapBlock();
	Token* CreateToken();
	void AppendVal(Token *t);
	void SetScannerBehindT();

	void Init();
	void NextCh();
	void AddCh();
	bool Comment0();
	bool Comment1();

	Token* NextToken();

public:
	Buffer *buffer;   // scanner buffer

	Scanner(const unsigned char* buf, int len);
	Scanner(const wchar_t* fileName);
	Scanner(FILE* s);
	~Scanner();
	Token* Scan();
	Token* Peek();
	void ResetPeek();
	const char *GetParserFileName() {
            return parseFileName ? parseFileName : "unknown";
        };

}; // end Scanner

} // namespace


#endif

