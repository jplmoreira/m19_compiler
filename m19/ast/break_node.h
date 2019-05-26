#ifndef __M19_BREAK_NODE_H__
#define __M19_BREAK_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/integer_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  /**
   * Class for describing break nodes.
   */
  class break_node: public cdk::basic_node {

  public:
    inline break_node(int lineno) : cdk::basic_node(lineno) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_break_node(this, level);
    }

  };

} // m19

#endif
