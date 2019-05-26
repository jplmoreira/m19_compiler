#ifndef __M19_ADDRESS_OF_NODE_H__
#define __M19_ADDRESS_OF_NODE_H__

#include <cdk/ast/unary_expression_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  /**
   * Class for describing address of left-value nodes.
   */
  class address_of_node: public cdk::expression_node {

    cdk::lvalue_node *_arg;

  public:
    address_of_node(int lineno, cdk::lvalue_node *arg) :
      cdk::expression_node(lineno), _arg(arg) {
    }

  public:
    inline cdk::lvalue_node *arg() { return _arg; }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_address_of_node(this, level);
    }

  };

} // m19

#endif
