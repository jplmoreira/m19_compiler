#ifndef __M19_MAP_NODE_H__
#define __M19_MAP_NODE_H__

#include <cdk/ast/basic_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  /**
   * Class for describing map nodes.
   */
  class map_node: public cdk::basic_node {
    std::string _funcName;
    cdk::lvalue_node *_vector;
    cdk::expression_node *_low, *_high;

  public:
    map_node(int lineno, const std::string &function_name, cdk::lvalue_node *vector, cdk::expression_node *low, cdk::expression_node *high) : cdk::basic_node(lineno), _funcName(function_name), _vector(vector), _low(low), _high(high) {
    }

  public:
    std::string &function_name() {
      return _funcName;
    }
    cdk::lvalue_node *vector() {
      return _vector;
    }

    cdk::expression_node *low() {
      return _low;
    }

    cdk::expression_node *high() {
      return _high;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_map_node(this, level);
    }

  };

} // m19

#endif
