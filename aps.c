#include <stdio.h>
#include <stdlib.h>
#include "prolog_gen.h"
#include "aps.tab.h"
#include "ast.h"
#include "aps.h"
#include "eval.h"

int main(int argc, char* argv[]) {
  FILE *fp;
  if (argc > 1) {
    // Open program
    if (! (fp = fopen(argv[1], "r"))) {
      fprintf(stderr, "Error, can't open %s\n", argv[1]);
      exit(1);
    }
    // AST!
    Program* prog = ast_from_program(fp);
    // Eval!
    fprintf(stdout, "Evaluating..\n");
    eval_program(prog);
  }
  else {
    fprintf(stderr, "Error, no file specified\n");
    exit(1);
  }
  return 0;
}
