#ifndef __M19_INDEX_NODE_H__
#define __M19_INDEX_NODE_H__

#include <cdk/ast/lvalue_node.h>
#include <cdk/ast/expression_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  /**
   * Class for describing indexation nodes.
   */
  class index_node: public cdk::lvalue_node {
    cdk::expression_node *_base, *_offset;

  public:
    inline index_node(int lineno, cdk::expression_node *base, cdk::expression_node *offset) :
      cdk::lvalue_node(lineno), _base(base), _offset(offset) {
    }

  public:
    inline cdk::expression_node *base() {
      return _base;
    }

    inline cdk::expression_node *offset() {
      return _offset;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

} // m19

#endif
