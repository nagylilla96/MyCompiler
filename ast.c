#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

#include <assert.h>
#include <stdlib.h>

/* ------------------------------------------------------------- Expressions */

struct mCc_ast_expression *
mCc_ast_new_expression_literal(struct mCc_ast_literal *literal)
{
	assert(literal);

	struct mCc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_LITERAL;
	expr->literal = literal;
	return expr;
}

struct mCc_ast_expression *
mCc_ast_new_expression_identifier(char *id, struct mCc_ast_expression *expression)
{
	assert(id);

	struct mCc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_LITERAL;
	expr->id = mCc_ast_new_identifier(id);
	expr->idexp = expression;

	return expr;
}

struct mCc_ast_expression *
mCc_ast_new_expression_unary_op(enum mCc_ast_unary_op op,
								 struct mCc_ast_expression *hs) 
{
	assert(hs);

	struct mCc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_UNARY_OP;
	expr->unop = op;
	expr->hs = hs;
	return expr;
}

struct mCc_ast_expression *
mCc_ast_new_expression_binary_op(enum mCc_ast_binary_op op,
                                 struct mCc_ast_expression *lhs,
                                 struct mCc_ast_expression *rhs)
{
	assert(lhs);
	assert(rhs);

	struct mCc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_BINARY_OP;
	expr->binop = op;
	expr->lhs = lhs;
	expr->rhs = rhs;
	return expr;
}

struct mCc_ast_expression *
mCc_ast_new_expression_parenth(struct mCc_ast_expression *expression)
{
	assert(expression);

	struct mCc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_PARENTH;
	expr->parexp = expression;
	return expr;
}

struct mCc_ast_expression *
mCc_ast_new_expression_bracket(struct mCc_ast_expression *expression)
{
	assert(expression);

	struct mCc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_BRACKET;
	expr->brexp = expression;
	return expr;
}

struct mCc_ast_expression *
mCc_ast_new_expression_function(char *id, struct mCc_ast_expression** expressions)
{
	assert(id);

	struct mCc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_FUNCT;
	expr->idf = mCc_ast_new_identifier(id);
	expr->args = mCc_ast_new_arguments(expressions);
	return expr;
}

void mCc_ast_delete_expression(struct mCc_ast_expression *expression)
{
	if (expression == NULL) return;

	switch (expression->type) {
	case MCC_AST_EXPRESSION_TYPE_LITERAL:
		mCc_ast_delete_literal(expression->literal);
		break;

	case MCC_AST_EXPRESSION_TYPE_UNARY_OP:
		mCc_ast_delete_expression(expression->hs);
		break;

	case MCC_AST_EXPRESSION_TYPE_BINARY_OP:
		mCc_ast_delete_expression(expression->lhs);
		mCc_ast_delete_expression(expression->rhs);
		break;

	case MCC_AST_EXPRESSION_TYPE_PARENTH:
		mCc_ast_delete_expression(expression->parexp);
		break;

	case MCC_AST_EXPRESSION_TYPE_BRACKET:
		mCc_ast_delete_expression(expression->brexp);
		break;

	case MCC_AST_EXPRESSION_TYPE_IDENTIFIER:
		mCc_ast_delete_identifier(expression->id);
		mCc_ast_delete_expression(expression->idexp);
		break;

	case MCC_AST_EXPRESSION_TYPE_FUNCT:
		mCc_ast_delete_identifier(expression->idf);
		mCc_ast_delete_arguments(expression->args);
		break;
	}

	free(expression);
}

/* ---------------------------------------------------------------- Literals */

struct mCc_ast_literal *mCc_ast_new_literal_bool(bool value) 
{
	struct mCc_ast_literal *lit = malloc(sizeof(*lit));
	if (!lit) {
		return NULL;
	}

	lit->type = MCC_AST_LITERAL_TYPE_BOOL;
	lit->i_value = value;
	return lit;
}

struct mCc_ast_literal *mCc_ast_new_literal_int(long value)
{
	struct mCc_ast_literal *lit = malloc(sizeof(*lit));
	if (!lit) {
		return NULL;
	}

	lit->type = MCC_AST_LITERAL_TYPE_INT;
	lit->i_value = value;
	return lit;
}

struct mCc_ast_literal *mCc_ast_new_literal_float(double value)
{
	struct mCc_ast_literal *lit = malloc(sizeof(*lit));
	if (!lit) {
		return NULL;
	}

	lit->type = MCC_AST_LITERAL_TYPE_FLOAT;
	lit->f_value = value;
	return lit;
}

struct mCc_ast_literal *mCc_ast_new_literal_string(string value)
{
	struct mCc_ast_literal *lit = malloc(sizeof(*lit));
	if (!lit) {
		return NULL;
	}

	lit->type = MCC_AST_LITERAL_TYPE_STRING;
	lit->s_value = value;
	return lit;
}

void mCc_ast_delete_literal(struct mCc_ast_literal *literal)
{
	assert(literal);
	free(literal);
}

/* ---------------------------------------------------------------- Identifier */

identifier mCc_ast_new_identifier(char *name) 
{
	identifier id = malloc(sizeof(name));
	if (!id) {
		return NULL;
	}

	strcpy(id, name);
	return id;
}

void mCc_ast_delete_identifier(identifier id)
{
	assert(id);
	free(id);
}

/* ---------------------------------------------------------------- Arguments */

arguments mCc_ast_new_arguments(struct mCc_ast_expression** expressions) 
{
	// arguments args = malloc(sizeof(args));
	arguments args = expressions;
	if (!args) {
		return NULL;
	}

	return args;
}

void mCc_ast_delete_arguments(arguments args)
{
	assert(args);
	int i = 0;

	while (args[i] != NULL) {
		mCc_ast_delete_expression(args[i]);
		i++;
	}
	free(args);
}

/* ---------------------------------------------------------------- Statements */

struct mCc_ast_statement *mCc_ast_new_statement_if(struct mCc_ast_expression *expression, struct mCc_ast_statement *statement, struct mCc_ast_statement *opstat) 
{
	assert(expression);
	assert(statement);

	struct mCc_ast_statement *stat = malloc(sizeof(*stat));
	if (!stat) {
		return NULL;
	}

	stat->type = MCC_AST_STATEMENT_TYPE_IF;
	stat->ifexp = expression;
	stat->ifstat = statement;
	stat->elsestat = opstat;
	return stat;
}

struct mCc_ast_statement *mCc_ast_new_statement_while(struct mCc_ast_expression *expression, struct mCc_ast_statement *statement)
{
	assert(expression);
	assert(statement);

	struct mCc_ast_statement *stat = malloc(sizeof(*stat));
	if (!stat) {
		return NULL;
	}

	stat->type = MCC_AST_STATEMENT_TYPE_WHILE;
	stat->whileexp = expression;
	stat->whilestat = statement;
	return stat;
}

struct mCc_ast_statement *mCc_ast_new_statement_return(struct mCc_ast_expression *expression)
{
	assert(expression);

	struct mCc_ast_statement *stat = malloc(sizeof(*stat));
	if (!stat) {
		return NULL;
	}

	stat->type = MCC_AST_STATEMENT_TYPE_RET;
	stat->retexp = expression;
	return stat;
}

struct mCc_ast_statement *mCc_ast_new_statement_declaration(enum mCc_ast_literal_type type, struct mCc_ast_literal *lit, 
			identifier id) 
{
	assert(id);

	struct mCc_ast_statement *stat = malloc(sizeof(*stat));
	if (!stat) {
		return NULL;
	}

	stat->type = MCC_AST_STATEMENT_TYPE_DECL;
	stat->decltype = type;
	stat->decllit = lit;
	stat->declid = id;
	return stat;
}

struct mCc_ast_statement *mCc_ast_new_statement_assignment(identifier id, struct mCc_ast_expression *left, struct mCc_ast_expression *right) 
{
	assert(id);
	assert(right);

	struct mCc_ast_statement *stat = malloc(sizeof(*stat));
	if (!stat) {
		return NULL;
	}

	stat->type = MCC_AST_STATEMENT_TYPE_ASSIGN;
	stat->asid = id;
	stat->lexp = left;
	stat->rexp = right;
	return stat;
}

struct mCc_ast_statement *mCc_ast_new_statement_expression(struct mCc_ast_expression *expression) 
{
	assert(expression);

	struct mCc_ast_statement *stat = malloc(sizeof(*stat));
	if (!stat) {
		return NULL;
	}

	stat->type = MCC_AST_STATEMENT_TYPE_EXPR;
	stat->expression = expression;
	return stat;
}

statements mCc_ast_new_statements(struct mCc_ast_statement** stats) 
{
	statements sts = stats;
	if (!sts) {
		return NULL;
	}

	return sts;
}

void mCc_ast_delete_statements(statements stats)
{
	assert(stats);
	int i = 0;

	while (stats[i] != NULL) {
		mCc_ast_delete_statement(stats[i]);
		i++;
	}
	free(stats);
}

struct mCc_ast_statement *mCc_ast_new_statement_compound(statements stats)
{
	assert(stats);

	struct mCc_ast_statement *stat = malloc(sizeof(*stat));
	if (!stat) {
		return NULL;
	}

	stat->type = MCC_AST_STATEMENT_TYPE_COMP;
	stat->stats = stats;
	return stat;
}

void mCc_ast_delete_statement(struct mCc_ast_statement *statement)
{
	if (!statement) return;

	switch (statement->type) {

		case MCC_AST_STATEMENT_TYPE_IF:
			mCc_ast_delete_expression(statement->ifexp);
			mCc_ast_delete_statement(statement->ifstat);
			mCc_ast_delete_statement(statement->elsestat);
			break;		

		case MCC_AST_STATEMENT_TYPE_WHILE:
			mCc_ast_delete_expression(statement->whileexp);
			mCc_ast_delete_statement(statement->whilestat);
			break;

		case MCC_AST_STATEMENT_TYPE_RET:
			mCc_ast_delete_expression(statement->retexp);
			break;

		case MCC_AST_STATEMENT_TYPE_DECL:
			mCc_ast_delete_literal(statement->decllit);
			mCc_ast_delete_identifier(statement->declid);
			break;

		case MCC_AST_STATEMENT_TYPE_ASSIGN:
			mCc_ast_delete_identifier(statement->asid);
			mCc_ast_delete_expression(statement->lexp);
			mCc_ast_delete_expression(statement->rexp);
			break;	

		case MCC_AST_STATEMENT_TYPE_EXPR:
			mCc_ast_delete_expression(statement->expression);
			break;

		case MCC_AST_STATEMENT_TYPE_COMP:
			mCc_ast_delete_statements(statement->stats);
			break;
	}

	free(statement);
}

/* ---------------------------------------------------------------- Function definition */

parameters mCc_ast_new_parameters(struct mCc_ast_statement** decls) 
{
	int i = 0;

	while(decls[i]) {
		if (decls[i]->type != MCC_AST_STATEMENT_TYPE_DECL) {
			return NULL;
		}
		i++;
	}
	
	parameters pars = decls;

	return pars;
}

void mCc_ast_delete_parameters(parameters pars)
{
	assert(pars);
	int i = 0;

	while (pars[i] != NULL) {
		mCc_ast_delete_statement(pars[i]);
		i++;
	}
	free(pars);
}

struct mCc_ast_function_def *mCc_ast_new_function_def(enum mCc_ast_literal_type type, identifier id, parameters pars, 
	struct mCc_ast_statement *compound) 
{
	assert(id);
	assert(compound);
	assert(compound->type == MCC_AST_STATEMENT_TYPE_COMP);

	struct mCc_ast_function_def *def = malloc(sizeof(*def));
	if (!def) {
		return NULL;
	}

	def->type = type;
	def->id = id;
	def->pars = pars;
	def->compound = compound;
	return def;
}

void mCc_ast_delete_function_def(struct mCc_ast_function_def *def)
{
	assert(def);
	mCc_ast_delete_identifier(def->id);
	mCc_ast_delete_parameters(def->pars);
	mCc_ast_delete_statement(def->compound);
	free(def);
}

// /* ---------------------------------------------------------------- Program */

definitions mCc_ast_new_definitions(struct mCc_ast_function_def** def) {
	assert(def);
	definitions defs = def;
	return defs;
}

void mCc_ast_delete_definitions(definitions defs) {
	assert(defs);
	int i = 0;

	while (defs[i] != NULL) {
		mCc_ast_delete_function_def(defs[i]);
		i++;
	}
	free(defs);
}



struct mCc_ast_program *mCc_ast_new_program(definitions defs) {
	assert(defs);

	struct mCc_ast_program *prog = malloc(sizeof(*prog));
	if (!prog) {
		return NULL;
	}

	prog->defs = defs;
	return prog;
}

void mCc_ast_delete_program(struct mCc_ast_program *prog) {
	assert(prog);
	mCc_ast_delete_definitions(prog->defs);
	free(prog);
}

/* ---------------------------------------------------------------- Tester */

int main() {
	char *var1 = "x";
	char *var2 = "y";
	char *var3 = "xx";
	char *var4 = "yy";
	char *var5 = "xxx";
	char *var6 = "yyy";
	char *var7 = "xxxx";
	char *var8 = "yyyy";
	char *func1 = "func1";
	char *func2 = "func2";

	struct mCc_ast_expression** e1 = malloc(sizeof(e1) * 2);
	e1[0] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(1));

	e1[1] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(2));

	struct mCc_ast_expression** e2 = malloc(sizeof(e2) * 2);
	e2[0] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(1));

	e2[1] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(2));

	arguments a1 = mCc_ast_new_arguments(e1);
	arguments a2 = mCc_ast_new_arguments(e2);

	struct mCc_ast_statement** s = malloc(sizeof(struct mCc_ast_statement*) * 2);
	s[0] = mCc_ast_new_statement_expression(mCc_ast_new_expression_function(var1, a1));
	s[1] = mCc_ast_new_statement_expression(mCc_ast_new_expression_function(var2, a2));

	struct mCc_ast_statement *comp = mCc_ast_new_statement_compound(s);

	// PARS
	struct mCc_ast_statement **statements = malloc(sizeof(*statements) * 2);
	statements[0] = mCc_ast_new_statement_declaration(MCC_AST_LITERAL_TYPE_INT, mCc_ast_new_literal_int(1), mCc_ast_new_identifier(var3));
	statements[1] = mCc_ast_new_statement_declaration(MCC_AST_LITERAL_TYPE_INT, mCc_ast_new_literal_int(2), mCc_ast_new_identifier(var4));

	parameters pars = mCc_ast_new_parameters(statements);

	struct mCc_ast_function_def *fdef1 = mCc_ast_new_function_def(MCC_AST_LITERAL_TYPE_INT, mCc_ast_new_identifier(func1), pars, comp);

	struct mCc_ast_expression** e3 = malloc(sizeof(e3) * 2);
	e3[0] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(1));

	e3[1] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(2));

	struct mCc_ast_expression** e4 = malloc(sizeof(e4) * 2);
	e4[0] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(1));

	e4[1] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(2));

	arguments a3 = mCc_ast_new_arguments(e3);
	arguments a4 = mCc_ast_new_arguments(e4);

	struct mCc_ast_statement** ss = malloc(sizeof(struct mCc_ast_statement*) * 2);
	ss[0] = mCc_ast_new_statement_expression(mCc_ast_new_expression_function(var5, a3));
	ss[1] = mCc_ast_new_statement_expression(mCc_ast_new_expression_function(var6, a4));

	struct mCc_ast_statement *compp = mCc_ast_new_statement_compound(ss);

	// PARS
	struct mCc_ast_statement **statementss = malloc(sizeof(*statementss) * 2);
	statementss[0] = mCc_ast_new_statement_declaration(MCC_AST_LITERAL_TYPE_INT, mCc_ast_new_literal_int(1), mCc_ast_new_identifier(var7));
	statementss[1] = mCc_ast_new_statement_declaration(MCC_AST_LITERAL_TYPE_INT, mCc_ast_new_literal_int(2), mCc_ast_new_identifier(var8));

	parameters parss = mCc_ast_new_parameters(statementss);

	struct mCc_ast_function_def *fdeff = mCc_ast_new_function_def(MCC_AST_LITERAL_TYPE_INT, mCc_ast_new_identifier(func2), parss, compp);

	struct mCc_ast_function_def **defs = malloc(sizeof(*defs) * 1);
	defs[0] = fdef1;
	defs[1] = fdeff;
	struct mCc_ast_program *program = mCc_ast_new_program(defs);
	printf("program created\n");
	mCc_ast_delete_program(program);
	return 0;
}