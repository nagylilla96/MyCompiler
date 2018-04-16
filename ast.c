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
		// printf("Trying to delete literal\n");
		mCc_ast_delete_literal(expression->literal);
		// printf("Deleted literal\n");
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
		// printf("Trying to delete identifier and arguments\n");
		mCc_ast_delete_identifier(expression->idf);
		// printf("Deleted identifier\n");
		mCc_ast_delete_arguments(expression->args);
		// printf("Deleted identifier and arguments\n");
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
		// printf("Trying to delete expression %d\n", i);
		mCc_ast_delete_expression(args[i]);
		// printf("Deleted expression %d\n", i);
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
		// printf("Deleting statement %d\n", i);
		mCc_ast_delete_statement(stats[i]);
		// printf("Deleted statement %d\n", i);
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
			// printf("Trying to delete expression\n");
			mCc_ast_delete_expression(statement->expression);
			// printf("Deleted expression\n");
			break;

		case MCC_AST_STATEMENT_TYPE_COMP:
			// printf("Trying to delete statements\n");
			mCc_ast_delete_statements(statement->stats);
			// printf("Deleted statements\n");
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
	if (!pars) {
		return NULL;
	}

	return pars;
}

void mCc_ast_delete_parameters(parameters pars)
{
	assert(pars);
	int i = 0;

	while (pars[i] != NULL) {
		// printf("Trying to delete statement (declaration) %d\n", i);
		mCc_ast_delete_statement(pars[i]);
		// printf("Deleted statement (declaration) %d\n", i);
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
	printf("Delete entered\n");
	mCc_ast_delete_identifier(def->id);
	printf("Identifier deleted\n");
	mCc_ast_delete_parameters(def->pars);
	mCc_ast_delete_statement(def->compound);
	printf("Parameters deleted\n");
	
	free(def);
}

// /* ---------------------------------------------------------------- Program */

// arguments mCc_ast_new_arguments(struct mCc_ast_expression** expressions) 
// {
// 	// arguments args = malloc(sizeof(args));
// 	arguments args = expressions;
// 	if (!args) {
// 		return NULL;
// 	}

// 	return args;
// }

// void mCc_ast_delete_arguments(arguments args)
// {
// 	assert(args);
// 	int i = 0;

// 	while (args[i] != NULL) {
// 		printf("Trying to delete expression %d\n", i);
// 		mCc_ast_delete_expression(args[i]);
// 		printf("Deleted expression %d\n", i);
// 		i++;
// 	}
// 	free(args);
// }

/* ---------------------------------------------------------------- Tester */

int main() {
	char *var1 = "x";
	char *var2 = "y";
	char *var3 = "xx";
	char *var4 = "yy";
	char *func = "func";

	// printf("Before anything\n");

	// struct mCc_ast_expression *expression = mCc_ast_new_expression_binary_op(MCC_AST_BINARY_OP_EQ, mCc_ast_new_expression_identifier(var1, NULL), 
		// mCc_ast_new_expression_literal(mCc_ast_new_literal_int(5)));
	// struct mCc_ast_expression *expression = mCc_ast_new_expression_identifier(var1, NULL);
	// printf("%s == %d\n", expression->lhs->id, expression->rhs->literal->i_value);
	// printf("After expression\n");
	// struct mCc_ast_statement *statement = mCc_ast_new_statement_assignment(mCc_ast_new_identifier(var2), NULL,
		// mCc_ast_new_expression_literal(mCc_ast_new_literal_int(3)));
	// printf("%s = %d\n", statement->asid, statement->rexp->literal->i_value);
	// printf("After statement\n");
	struct mCc_ast_expression** expressions = malloc(sizeof(expressions) * 2);
	expressions[0] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(1));
	
	expressions[1] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(2));

	struct mCc_ast_expression** exprs = malloc(sizeof(exprs) * 2);
	exprs[0] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(1));
	
	exprs[1] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(2));
	
	arguments args1 = mCc_ast_new_arguments(expressions);
	arguments args2 = mCc_ast_new_arguments(exprs);
	// printf("Expression 0: %d\n", args[0]->literal->i_value);
	// printf("Expression 1: %d\n", args[1]->literal->i_value);

	struct mCc_ast_statement** stats = malloc(sizeof(stats) * 2);
	stats[0] = mCc_ast_new_statement_expression(mCc_ast_new_expression_function(var1, args1));
	stats[1] = mCc_ast_new_statement_expression(mCc_ast_new_expression_function(var2, args2));

	struct mCc_ast_expression** e1 = malloc(sizeof(e1) * 2);
	e1[0] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(1));
	
	e1[1] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(2));

	struct mCc_ast_expression** e2 = malloc(sizeof(e2) * 2);
	e2[0] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(1));
	
	e2[1] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(2));
	
	arguments a1 = mCc_ast_new_arguments(e1);
	arguments a2 = mCc_ast_new_arguments(e2);
	// printf("Expression 0: %d\n", args[0]->literal->i_value);
	// printf("Expression 1: %d\n", args[1]->literal->i_value);

	struct mCc_ast_statement** s = malloc(sizeof(stats) * 2);
	s[0] = mCc_ast_new_statement_expression(mCc_ast_new_expression_function(var1, a1));
	s[1] = mCc_ast_new_statement_expression(mCc_ast_new_expression_function(var2, a2));
	
	// expressions[1] = mCc_ast_new_expression_literal(mCc_ast_new_literal_int(2));
	// struct mCc_ast_statement *fullstatement = mCc_ast_new_statement_expression(mCc_ast_new_expression_function(var1, args));
	struct mCc_ast_statement *stat = mCc_ast_new_statement_compound(stats);
	struct mCc_ast_statement *comp = mCc_ast_new_statement_compound(s);

	// printf("%s(%d, %d);\n", stat->stats[0]->expression->idf, stat->stats[0]->expression->args[0]->literal->i_value, stat->stats[0]->expression->args[1]->literal->i_value);
	// printf("%s(%d, %d);\n", stat->stats[1]->expression->idf, stat->stats[1]->expression->args[0]->literal->i_value, stat->stats[1]->expression->args[1]->literal->i_value);

	// printf("while (%s == %d) { %s = %d; }\n", fullstatement->ifexp->lhs->id, fullstatement->ifexp->rhs->literal->i_value,
		// fullstatement->ifstat->asid, fullstatement->ifstat->rexp->literal->i_value);
	mCc_ast_delete_statement(stat);

	struct mCc_ast_statement **statements = malloc(sizeof(statements) * 2);
	statements[0] = mCc_ast_new_statement_declaration(MCC_AST_LITERAL_TYPE_INT, mCc_ast_new_literal_int(1), mCc_ast_new_identifier(var3));
	statements[1] = mCc_ast_new_statement_declaration(MCC_AST_LITERAL_TYPE_INT, mCc_ast_new_literal_int(2), mCc_ast_new_identifier(var4));

	parameters pars = mCc_ast_new_parameters(statements);

	struct mCc_ast_function_def *fdef = mCc_ast_new_function_def(MCC_AST_LITERAL_TYPE_INT, mCc_ast_new_identifier(func), pars, s);

	printf("Function defined\n");

	printf("%s = %d\n", fdef->pars[0]->declid, fdef->pars[0]->decllit->i_value);
	printf("%s = %d\n", fdef->pars[1]->declid, fdef->pars[1]->decllit->i_value);

	mCc_ast_delete_function_def(fdef);
	// mCc_ast_delete_statements(fullstatement);
	return 0;
}