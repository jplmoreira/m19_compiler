#ifndef __M19_FUNCTION_DECLARATION_NODE_H__
#define __M19_FUNCTION_DECLARATION_NODE_H__

#include <string>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  /**
   * Class for describing function declaration nodes.
   */
  class function_declaration_node: public cdk::basic_node {
    int _qualifier;
    basic_type *_type;
    std::string _identifier;
    cdk::sequence_node *_parameters;

  public:
    function_declaration_node(int lineno, int qualifier, const std::string &identifier, cdk::sequence_node *parameters) :
        cdk::basic_node(lineno), _qualifier(qualifier),
        _type(new basic_type(0, basic_type::TYPE_VOID)),
        _identifier(identifier), _parameters(parameters) {
    }

    function_declaration_node(int lineno, int qualifier, basic_type *type, const std::string &identifier, cdk::sequence_node *parameters) :
        cdk::basic_node(lineno), _qualifier(qualifier), _type(type),
        _identifier(identifier), _parameters(parameters) {
    }

  public:
    int qualifier() {
      return _qualifier;
    }
    basic_type *type() {
      return _type;
    }
    const std::string &identifier() const {
      return _identifier;
    }
    cdk::sequence_node *parameters() {
      return _parameters;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_declaration_node(this, level);
    }

  };

} // m19

#endif
