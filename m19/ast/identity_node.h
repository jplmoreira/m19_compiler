#ifndef __M19_AST_IDENTITY_NODE_H__
#define __M19_AST_IDENTITY_NODE_H__


namespace m19 {

  /**
   * Class for describing identity nodes.
   */
  class identity_node: public cdk::unary_expression_node {

  public:
    inline identity_node(int lineno, cdk::expression_node *argument) :
      cdk::unary_expression_node(lineno, argument) {
    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identity_node(this, level);
    }

  };
  
} //m19

#endif
