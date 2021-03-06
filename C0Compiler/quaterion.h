#include "stdafx.h"

class BasicBlock;

class Quaterion {
public:
    static FILE* dump_file;
    enum operators {
        ADD, MULT,                      
        SUB, DIV,
        LT, LTE, GT, GTE,
        NE, EQ,

        TO,  // a[i] = b, a = b TO i
        AT,  // c = a[i], c = A AT i

        NONE, // a = b
        MINUS, // a = -b
        CAST_INT, // a = (int) b
        CAST_CHAR, // a = (char) b

        LABEL,
        PROLOG,

        PUSH,    // push parameter
        CALL,

        PRINT_CHAR,
        PRINT_INT,
        PRINT_STR,

        BEQZ,   // branch on zero
        BNEZ,   // branch on non-zero
        BEQ,    // branch on equal
        BNE,    // branch on not-equal

        GET,    // get returned value
        READ_CHAR,   // read char
        READ_INT,    // read integer

        _LINK,   // directive, link to a label, to avoid jump
        GOTO,    // unconditional jump
                
        EPILOG,
        RET,

		SELF,

        END      // sentinel
    };
    static const int OP_COUNT = END;

    
    string get_name(Symbol* sym);

    Symbol* dst;
    Symbol* left;
    Symbol* right;

    Symbol* label;
    int op;
	int idx;

    void init(int op_name);
    Quaterion(int op_name, Symbol*, Symbol*, Symbol*);
    Quaterion(int op_name, Symbol* dst, Symbol* left);
    Quaterion(int op_name, Symbol* left);
	Quaterion();

    Quaterion(Symbol* dst, Symbol* left); // a = b
    void emit(bool comment=false);
    void emit(string str);
	void emit_debug();
    // comm with mips gen
    bool is_endblock();
    bool is_commutative();
    bool is_incommutative();
    bool is_branch();
	bool is_unconditional_jump();
	bool is_jump();
    bool is_print();
	bool is_action();
	bool is_simple();
	// for optimization
	bool beginning;
	BasicBlock* basic_block;
};

static char* op_names[Quaterion::OP_COUNT] =
{
    "+", "*",
    "-", "/",
    "<", "<=", ">", ">=",
    "!=", "==",

    "[]=",
    "=[]",

    "",
    "-",
    "(int)",
    "(char)",

    ":",
    "PROLOG",

    "PUSH",
    "CALL",

    "PRINT_CHAR",
    "PRINT_INT",
    "PRINT_STR",

    "if == 0 go to:",
    "if != 0 go to:",
    "if == go to:",
    "if != go to:",

    "GET",
    "READ_CHAR",
    "READ_INT",

    "_LINK",
    "GOTO",

    "EPILOG",
    "RET",

	"SELF"
};

