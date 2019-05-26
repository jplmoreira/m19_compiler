#ifndef __M19_FUNCTION_DEFINITION_NODE_H__
#define __M19_FUNCTION_DEFINITION_NODE_H__

#include <string>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  /**
   * Class for describing function definition nodes.
   */
  class function_definition_node: public cdk::basic_node {
    int _qualifier;
    basic_type *_type;
    std::string _identifier;
    cdk::sequence_node *_parameters;
    body_node *_body;
    cdk::expression_node *_default_ret;

  public:
    function_definition_node(int lineno, int qualifier, const std::string &identifier, cdk::sequence_node *parameters, body_node *body) :
        cdk::basic_node(lineno), _qualifier(qualifier),
        _type(new basic_type(0, basic_type::TYPE_VOID)),
        _identifier(identifier), _parameters(parameters),
        _body(body), _default_ret(nullptr) {
    }

    function_definition_node(int lineno, int qualifier, basic_type *type, const std::string &identifier,
                             cdk::sequence_node *parameters, body_node *body, cdk::expression_node *default_ret) :
        cdk::basic_node(lineno), _qualifier(qualifier), _type(type),
        _identifier(identifier), _parameters(parameters),
        _body(body), _default_ret(default_ret) {
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
    body_node *body() {
      return _body;
    }
    cdk::expression_node *default_ret() {
      return _default_ret;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_definition_node(this, level);
    }

  };

} // m19

#endif
