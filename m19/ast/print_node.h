// $Id: print_node.h,v 1.4 2019/05/22 19:41:55 ist170281 Exp $ -*- c++ -*-
#ifndef __M19_PRINT_NODE_H__
#define __M19_PRINT_NODE_H__

#include <cdk/ast/expression_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  /**
   * Class for describing print and println nodes.
   */
  class print_node: public cdk::basic_node {
    cdk::expression_node *_argument;
    bool _ln;

  public:
    inline print_node(int lineno, cdk::expression_node *argument, bool line_flag) :
        cdk::basic_node(lineno), _argument(argument), _ln(line_flag) {
    }

  public:
    inline cdk::expression_node *argument() {
      return _argument;
    }

    inline bool ln() {
      return _ln;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_print_node(this, level);
    }

  };

} // m19

#endif
