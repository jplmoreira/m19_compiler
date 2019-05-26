#ifndef __M19_RETURN_NODE_H__
#define __M19_RETURN_NODE_H__

#include <cdk/ast/expression_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  /**
   * Class for describing return nodes.
   */
  class return_node: public cdk::basic_node {

  public:
    return_node(int lineno) : cdk::basic_node(lineno) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_node(this, level);
    }

  };

} // m19

#endif
