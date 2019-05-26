#ifndef __M19_AST_MEM_ALLOC_H__
#define __M19_AST_MEM_ALLOC_H__

#include <cdk/ast/unary_expression_node.h>
#include <cdk/ast/expression_node.h>

namespace m19 {

  class mem_alloc_node: public cdk::unary_expression_node {

  public:
    mem_alloc_node(int lineno, cdk::expression_node *arg) :
        cdk::unary_expression_node(lineno, arg) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_mem_alloc_node(this, level);
    }

  };

} // m19

#endif
