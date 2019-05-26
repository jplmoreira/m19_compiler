#ifndef __M19_CONTINUE_NODE_H__
#define __M19_CONTINUE_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/integer_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  /**
   * Class for describing continue nodes.
   */
  class continue_node: public cdk::basic_node {

  public:
    inline continue_node(int lineno) : cdk::basic_node(lineno) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_continue_node(this, level);
    }

  };

} // m19

#endif
