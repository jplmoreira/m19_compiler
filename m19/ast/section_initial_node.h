#ifndef __M19_SECTION_INITIAL_NODE_H__
#define __M19_SECTION_INITIAL_NODE_H__

#include <cdk/ast/basic_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  /**
   * Class for describing initial section nodes.
   */
  class section_initial_node: public cdk::basic_node {
    block_node* _block;

  public:
    section_initial_node(int lineno, block_node *block) :
      cdk::basic_node(lineno), _block(block) {
    }

  public:
    block_node *block() { return _block; }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_section_initial_node(this, level);
    }

  };

} // m19

#endif
