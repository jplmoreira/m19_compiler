#ifndef __M19_SECTION_CONDITIONAL_NODE_H__
#define __M19_SECTION_CONDITIONAL_NODE_H__

#include <cdk/ast/basic_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  /**
   * Class for describing conditional section nodes.
   */
  class section_conditional_node: public cdk::basic_node {
    int _qualifier;
    cdk::expression_node* _expression;
    block_node* _block;

  public:
    section_conditional_node(int lineno, int qualifier, cdk::expression_node *expression, block_node *block) :
      cdk::basic_node(lineno), _qualifier(qualifier), _expression(expression), _block(block) {
    }

  public:
    int qualifier() { return _qualifier; }
    
    cdk::expression_node *expression() { return _expression; }

    block_node *block() { return _block; }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_section_conditional_node(this, level);
    }

  };

} // m19

#endif
