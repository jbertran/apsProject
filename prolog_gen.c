#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "prolog_gen.h"

#ifdef DEBUG_PLG
#define debug_plg(str) {fprintf(stderr, "%s\n", str); fflush(stderr);}
#else
#define debug_plg(str) ;
#endif

// Forward decs
void print_block(FILE* f, AST* block);
void print_cmds(FILE* f, AST* cmds);
void print_seq(FILE* f, Cmds* cmds);
void print_dec(FILE* f, AST* dec);
void print_stat(FILE* f, AST* stat);
void print_expr(FILE* f, AST* expr);
void print_type(FILE* f, AST* type);

void print_program(FILE* f, Program* prog) {
  debug_plg("Program");
  fprintf(f, "type(prog(");
  print_block(f, prog->cmds);
  fprintf(f, "), [], void).");
}

void print_block(FILE* f, AST* block) {
  debug_plg("Block");
  fprintf(f, "[");
  print_cmds(f, block->content.asBlock->cmds);
  fprintf(f, "]");
}

void print_cmds(FILE* f, AST* cmds) {
  debug_plg("Cmds");
  print_seq(f, cmds->content.asCmds);
}

void print_seq(FILE* f, Cmds* cmds) {
  debug_plg("Seq");
  while(cmds) {
    AST* statdec = cmds->statDec;
    switch (statdec->type) {
    case T_STAT:
      if (cmds->next) {
	print_stat(f, statdec);
	fprintf(f, ",");
      }
      else 
	print_stat(f, statdec);
      break;
    case T_DEC:
      if (cmds->next) {
	print_dec(f, statdec);
	fprintf(f, ",");
      }
      else
	print_dec(f, statdec);
      break;
    default:
      fprintf(stderr, "Sequence error - invalid instruction\n");
      exit(EXIT_FAILURE);
    }
    if (!cmds->next)
      break;
    cmds = cmds->next->content.asCmds;
  }
}

void print_dec(FILE* f, AST* dec) {
  debug_plg("Dec");
  Dec* decl = dec->content.asDec;
  switch (decl->type) {
  case T_CONST:
    fprintf(f, "const(%s,", decl->ident);
    print_type(f, decl->decType);
    fprintf(f, ",");
    print_expr(f, decl->expr);
    fprintf(f, ")");
    break;
  case T_VAR:
    fprintf(f, "var(%s,", decl->ident);
    print_type(f, decl->decType);
    fprintf(f, ")");
    break;
  default:
    fprintf(stderr, "Unknown declaration type!\n");
    exit(EXIT_FAILURE);
  }
}

void print_stat(FILE* f, AST* st) {
  debug_plg("Stat");
  Stat* stat = st->content.asStat;
  switch (stat->type) {
  case T_SET:
    fprintf(f, "set(%s,", stat->ident);
    print_expr(f, stat->expr);
    fprintf(f, ")");
    break;
  case T_IF:
    fprintf(f, "if(");
    print_expr(f, stat->expr);
    fprintf(f, ",");
    print_block(f, stat->prog1);
    fprintf(f, ",");
    print_block(f, stat->prog2);
    fprintf(f, ")");
    break;
  case T_WHILE:
    fprintf(f, "while(");
    print_expr(f, stat->expr);
    fprintf(f, ",");
    print_block(f, stat->prog1);
    fprintf(f, ")");
    break;
  case T_ECHO:
    fprintf(f, "echo(");
    print_expr(f, stat->expr);
    fprintf(f, ")");
    break;
  default:
    fprintf(stderr, "Unknown statement type!\n");
    exit(EXIT_FAILURE);
  }
}

void print_expr(FILE* f, AST* exp) {
  debug_plg("Expr");
  Expr* expr = exp->content.asExpr;
  switch (expr->type) {
  case T_E_BOOL:
    if (expr->contents.bool == T_TRUE)
      fprintf(f, "true");
    else
      fprintf(f, "false");
    break;
  case T_NUM:
    fprintf(f, "%d", expr->contents.num);
    break;
  case T_IDENT:
    fprintf(f, "%s", expr->contents.ident);
    break;
  case T_UNOP:
    {
      UnOp* un = expr->contents.unop;
      switch (un->op) {
      case T_NOT:
	fprintf(f, "not(");
	break;
      default:
	fprintf(stderr, "Unknown operand!\n");
	exit(EXIT_FAILURE);
      }
      print_expr(f, un->arg);
      fprintf(f, ")");
      break;
    }
  case T_BINOP:
    {
      BinOp* bin = expr->contents.binop;
      switch (bin->op) {
      case T_AND:
	fprintf(f, "and(");
	break;
      case T_OR:
	fprintf(f, "or(");
	break;
      case T_EQ:
	fprintf(f, "eq(");
	break;
      case T_LT:
	fprintf(f, "lt(");
	break;
      case T_ADD:
	fprintf(f, "add(");
	break;
      case T_SUB:
	fprintf(f, "sub(");
	break;
      case T_MUL:
	fprintf(f, "mul(");
	break;
      case T_DIV:
	fprintf(f, "div(");
	break;
      case T_MOD:
	fprintf(f, "mod(");
	break;
      default:
	fprintf(stderr, "Unknown operand!\n");
	exit(EXIT_FAILURE);
      }
      print_expr(f, bin->arg1);
      fprintf(f, ",");
      print_expr(f, bin->arg2);
      fprintf(f, ")");
    }
    break;
  default:
    fprintf(stderr, "Unknown expression type!\n");
    exit(EXIT_FAILURE);
  }
}

void print_type(FILE* f, AST* type) {
  debug_plg("Type");
  switch (*(type->content.asType)) {
  case T_VOID:
    fprintf(f, "void");
    break;
  case T_BOOL:
    fprintf(f, "bool");
    break;
  case T_INT:
    fprintf(f, "int");
  }
}
