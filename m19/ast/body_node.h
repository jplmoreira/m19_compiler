#ifndef __M19_BODY_NODE_H__
#define __M19_BODY_NODE_H__

#include <cdk/ast/basic_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  /**
   * Class for describing body nodes.
   */
  class body_node: public cdk::basic_node {
    m19::section_initial_node *_initial;
    cdk::sequence_node *_sections;
    m19::section_final_node *_fin;

  public:
    body_node(int lineno, m19::section_initial_node *initial, cdk::sequence_node *sections, m19::section_final_node *fin) :
        cdk::basic_node(lineno), _initial(initial), _sections(sections), _fin(fin) {
    }

  public:
    cdk::sequence_node *sections() {
      return _sections;
    }

    m19::section_initial_node *initial() {
      return _initial;
    }

    m19::section_final_node *fin() {
      return _fin;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_body_node(this, level);
    }

  };

} // m19

#endif
