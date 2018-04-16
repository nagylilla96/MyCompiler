#ifndef MCC_AST_H
#define MCC_AST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/* Define max variable name size */

#define MAXLEN 2048

/* Define type string */

typedef char *string;

/* Define type identifier */

typedef char* identifier;

/* Forward Declarations */
struct mCc_ast_expression;
struct mCc_ast_literal;

/* Declare an array of arguments */

typedef struct mCc_ast_expression** arguments;
typedef struct mCc_ast_statement** statements;
typedef struct mCc_ast_statement** 	parameters;
typedef struct mCc_ast_function_def** definitions;

/* ---------------------------------------------------------------- AST Node */

struct mCc_ast_source_location {
	int start_line;
	int start_col;
	int end_line;
	int end_col;
};

/* Data contained by every AST node. */
struct mCc_ast_node {
	struct mCc_ast_source_location sloc;
};

/* --------------------------------------------------------------- Operators */

enum mCc_ast_unary_op{
	MCC_AST_UNARY_OP_NEG,
	MCC_AST_UNARY_OP_NOT
};

enum mCc_ast_binary_op {
	MCC_AST_BINARY_OP_ADD,
	MCC_AST_BINARY_OP_SUB,
	MCC_AST_BINARY_OP_MUL,
	MCC_AST_BINARY_OP_DIV,
	MCC_AST_BINARY_OP_ST,
	MCC_AST_BINARY_OP_GT,
	MCC_AST_BINARY_OP_SE,
	MCC_AST_BINARY_OP_GE,
	MCC_AST_BINARY_OP_AND,
	MCC_AST_BINARY_OP_OR,
	MCC_AST_BINARY_OP_EQ,
	MCC_AST_BINARY_OP_EQUALITY,
	MCC_AST_BINARY_OP_NE
};

/* ------------------------------------------------------------- Expressions */

enum mCc_ast_expression_type {
	MCC_AST_EXPRESSION_TYPE_LITERAL,
	MCC_AST_EXPRESSION_TYPE_IDENTIFIER,
	MCC_AST_EXPRESSION_TYPE_UNARY_OP,
	MCC_AST_EXPRESSION_TYPE_BINARY_OP,
	MCC_AST_EXPRESSION_TYPE_PARENTH,
	MCC_AST_EXPRESSION_TYPE_BRACKET,
	MCC_AST_EXPRESSION_TYPE_FUNCT
};

struct mCc_ast_expression {
	struct mCc_ast_node node;

	enum mCc_ast_expression_type type;
	union {
		/* MCC_AST_EXPRESSION_TYPE_LITERAL */
		struct mCc_ast_literal *literal;

		/* MCC_AST_EXPRESSION_TYPE_IDENTIFIER */
		struct {
			identifier id;

			/* MCC_AST_EXPRESSION_TYPE_BRACKET */
			struct mCc_ast_expression *idexp;
		};

		/* MCC_AST_EXPRESSION_TYPE_UNARY_OP */
		struct {
			enum mCc_ast_unary_op unop;
			struct mCc_ast_expression *hs;
		};

		/* MCC_AST_EXPRESSION_TYPE_BINARY_OP */
		struct {
			enum mCc_ast_binary_op binop;
			struct mCc_ast_expression *lhs;
			struct mCc_ast_expression *rhs;
		};

		/* MCC_AST_EXPRESSION_TYPE_PARENTH */
		struct mCc_ast_expression *parexp;

		/* MCC_AST_EXPRESSION_TYPE_BRACKET */
		struct mCc_ast_expression *brexp;

		/* MCC_AST_EXPRESSION_TYPE_FUNC */
		struct {
			identifier idf;
			arguments args;
		};
	};
};

struct mCc_ast_expression *
mCc_ast_new_expression_literal(struct mCc_ast_literal *literal);

struct mCc_ast_expression *
mCc_ast_new_expression_unary_op(enum mCc_ast_unary_op op,
								 struct mCc_ast_expression *hs);

struct mCc_ast_expression *
mCc_ast_new_expression_binary_op(enum mCc_ast_binary_op op,
                                 struct mCc_ast_expression *lhs,
                                 struct mCc_ast_expression *rhs);

struct mCc_ast_expression *
mCc_ast_new_expression_parenth(struct mCc_ast_expression *expression);

struct mCc_ast_expression *
mCc_ast_new_expression_bracket(struct mCc_ast_expression *expression);

struct mCc_ast_expression *
mCc_ast_new_expression_function(char *id, struct mCc_ast_expression** expressions);

void mCc_ast_delete_expression(struct mCc_ast_expression *expression);

/* ---------------------------------------------------------------- Literals */

enum mCc_ast_literal_type {
	MCC_AST_LITERAL_TYPE_BOOL,
	MCC_AST_LITERAL_TYPE_INT,
	MCC_AST_LITERAL_TYPE_FLOAT,
	MCC_AST_LITERAL_TYPE_STRING,
	MCC_AST_LITERAL_TYPE_VOID
};

struct mCc_ast_literal {
	struct mCc_ast_node node;

	enum mCc_ast_literal_type type;
	union {
		/* MCC_AST_LITERAL_TYPE_BOOL */
		bool b_value;

		/* MCC_AST_LITERAL_TYPE_INT */
		long i_value;

		/* MCC_AST_LITERAL_TYPE_FLOAT */
		double f_value;

		/* MCC_AST_LITERAL_TYPE_STRING */
		string s_value;
	};
};

struct mCc_ast_literal *mCc_ast_new_literal_bool(bool value);

struct mCc_ast_literal *mCc_ast_new_literal_int(long value);

struct mCc_ast_literal *mCc_ast_new_literal_float(double value);

struct mCc_ast_literal *mCc_ast_new_literal_string(string value);

void mCc_ast_delete_literal(struct mCc_ast_literal *literal);

/* ---------------------------------------------------------------- Identifier */

identifier mCc_ast_new_identifier(char *name);

void mCc_ast_delete_identifier(identifier id);

/* ---------------------------------------------------------------- Arguments */

arguments mCc_ast_new_arguments(struct mCc_ast_expression** expressions);

void mCc_ast_delete_arguments(arguments args);


/* ---------------------------------------------------------------- Statements */

enum mCc_ast_statement_type {
	MCC_AST_STATEMENT_TYPE_IF,
	MCC_AST_STATEMENT_TYPE_WHILE,
	MCC_AST_STATEMENT_TYPE_RET,
	MCC_AST_STATEMENT_TYPE_DECL,
	MCC_AST_STATEMENT_TYPE_ASSIGN,
	MCC_AST_STATEMENT_TYPE_EXPR,
	MCC_AST_STATEMENT_TYPE_COMP
};

struct mCc_ast_statement {
	struct mCc_ast_node node;

	enum mCc_ast_statement_type type;
	union {
		/* MCC_AST_STATEMENT_TYPE_IF */
		struct {
			struct mCc_ast_expression *ifexp;
			struct mCc_ast_statement *ifstat;
			struct mCc_ast_statement *elsestat;
		};

		/* MCC_AST_STATEMENT_TYPE_WHILE */
		struct {
			struct mCc_ast_expression *whileexp;
			struct mCc_ast_statement *whilestat;
		};

		/* MCC_AST_STATEMENT_TYPE_RET */
		struct mCc_ast_expression *retexp;

		/* MCC_AST_STATEMENT_TYPE_DECL */
		struct {
			enum mCc_ast_literal_type decltype;
			struct mCc_ast_literal *decllit;
			identifier declid;
		};

		/* MCC_AST_STATEMENT_TYPE_ASSIGN */
		struct {
			identifier asid;
			struct mCc_ast_expression *lexp;
			struct mCc_ast_expression *rexp;
		};

		/* MCC_AST_STATEMENT_TYPE_EXPR */
		struct mCc_ast_expression *expression;

		/* MCC_AST_STATEMENT_TYPE_COMP */
		statements stats;

	};
};

struct mCc_ast_statement *mCc_ast_new_statement_if(struct mCc_ast_expression *expression, struct mCc_ast_statement *statement, struct mCc_ast_statement *opstat);

struct mCc_ast_statement *mCc_ast_new_statement_while(struct mCc_ast_expression *expression, struct mCc_ast_statement *statement);

struct mCc_ast_statement *mCc_ast_new_statement_return(struct mCc_ast_expression *expression);

struct mCc_ast_statement *mCc_ast_new_statement_declaration(enum mCc_ast_literal_type type, struct mCc_ast_literal *lit, identifier id);

struct mCc_ast_statement *mCc_ast_new_statement_assignment(identifier id, struct mCc_ast_expression *left, struct mCc_ast_expression *right);

struct mCc_ast_statement *mCc_ast_new_statement_expression(struct mCc_ast_expression *expression);

void mCc_ast_delete_statement(struct mCc_ast_statement *statement);

/* ---------------------------------------------------------------- Function definition */

struct mCc_ast_function_def {
	struct mCc_ast_node node;

	struct {
		enum mCc_ast_literal_type type;
		identifier id;
		parameters pars;
		struct mCc_ast_statement *compound;
	};
};

parameters mCc_ast_new_parameters(struct mCc_ast_statement** decls);

void mCc_ast_delete_parameters(parameters pars);

struct mCc_ast_function_def *mCc_ast_new_function_def(enum mCc_ast_literal_type type, identifier id, parameters pars, 
	struct mCc_ast_statement *compound);

void mCc_ast_delete_function_def(struct mCc_ast_function_def *def);

/* ---------------------------------------------------------------- Program */

struct mCc_ast_program {
	struct mCc_ast_node node;

	definitions defs;
};

definitions mCc_ast_new_definitions(struct mCc_ast_function_def** defs);

void mCc_ast_delete_definitions(definitions defs);

struct mCc_ast_program *mCc_ast_new_program(definitions defs);

void mCc_ast_delete_program(struct mCc_ast_program *prog);

#ifdef __cplusplus
}
#endif

#endif
