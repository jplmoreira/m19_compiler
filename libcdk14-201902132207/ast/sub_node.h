#ifndef __CDK13_AST_SUB_H__
#define __CDK13_AST_SUB_H__

#include <cdk/ast/binary_expression_node.h>

namespace cdk {

  /** Class for describing the subtraction operator. */
  class sub_node: public binary_expression_node {
  public:
    /**
     * @param lineno source code line number for this node
     * @param left first operand
     * @param right second operand
     */
    sub_node(int lineno, expression_node *left, expression_node *right) :
        binary_expression_node(lineno, left, right) {
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_sub_node(this, level);
    }

  };

} // cdk

#endif
