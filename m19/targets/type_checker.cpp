#include <string>
#include <vector>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//---------------------------------------------------------------------------

void m19::type_checker::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------
void m19::type_checker::do_map_node(cdk::nil_node * const node, int lvl) {
  const std::string &id = node->identifier();
  std::shared_ptr<m19::symbol> symbol = _symtab.find(id);

  if (symbol == nullptr) {
    throw std::string("function doesn't exist");
  } else if (!symbol->is_function()) {
    throw std::string(id + "is not a function.");
  } else {
    if (symbol->arguments().size() == 1) {
      throw std::string("function needs to have only one argument.");
    }
    if (symbol->arguments()[0]->name() != symbol->type()->name()) {
      throw std::string("function arguments need to be the same type as the function.");
    }
  }

  node->vector()->accept(this, lvl);
  if (node->vector()->type()->name() != basic_type::TYPE_POINTER) {
    throw std::string("vector is not a pointer");
  }
  if (node->vector()->type()->subtype()->name() != symbol->type()->name()) {
    throw std::string("vector element needs to be the same type as the function.");
  }

  node->low()->accept(this, lvl);
  if (node->low()->type()->name() != basic_type::TYPE_INT) {
    throw std::string("index low must be integer.");
  }

  node->high()->accept(this, lvl);
  if (node->high()->type()->name() != basic_type::TYPE_INT) {
    throw std::string("index high must be integer.");
  }
}

void m19::type_checker::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void m19::type_checker::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void m19::type_checker::do_block_node(m19::block_node *const node, int lvl) {
  // EMPTY
}
void m19::type_checker::do_body_node(m19::body_node *const node, int lvl) {
  // EMPTY
}
void m19::type_checker::do_section_initial_node(m19::section_initial_node *const node, int lvl) {
  // EMPTY
}
void m19::type_checker::do_section_final_node(m19::section_final_node *const node, int lvl) {
  // EMPTY
}
void m19::type_checker::do_break_node(m19::break_node *const node, int lvl) {
  //EMPTY
}
void m19::type_checker::do_continue_node(m19::continue_node *const node, int lvl) {
  //EMPTY
}
void m19::type_checker::do_return_node(m19::return_node *const node, int lvl) {
  //EMPTY
}

void m19::type_checker::do_not_node(cdk::not_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->argument()->type(new basic_type(4, basic_type::TYPE_INT));
  } else if (node->argument()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in argument on NOT");

  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void m19::type_checker::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->left()->type(new basic_type(4, basic_type::TYPE_INT));
  } else if (node->left()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in left argument on AND");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->right()->type(new basic_type(4, basic_type::TYPE_INT));
  } else if (node->right()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in right argument on AND");

  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void m19::type_checker::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->left()->type(new basic_type(4, basic_type::TYPE_INT));
  } else if (node->left()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in left argument on OR");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->right()->type(new basic_type(4, basic_type::TYPE_INT));
  } else if (node->right()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in right argument on OR");

  node->type(new basic_type(4, basic_type::TYPE_INT));
}

//---------------------------------------------------------------------------

void m19::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void m19::type_checker::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}

void m19::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

//---------------------------------------------------------------------------

void m19::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT &&
      node->argument()->type()->name() != basic_type::TYPE_DOUBLE)
    throw std::string("wrong type in argument of unary expression");

  node->type(node->argument()->type());
}

void m19::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void m19::type_checker::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT &&
      node->left()->type()->name() != basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() != basic_type::TYPE_UNSPEC)
    throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT &&
      node->right()->type()->name() != basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() != basic_type::TYPE_UNSPEC)
    throw std::string("wrong type in right argument of binary expression");

  if (node->left()->type()->name() == basic_type::TYPE_UNSPEC &&
      node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->left()->type(new basic_type(4, basic_type::TYPE_INT));
    node->right()->type(new basic_type(4, basic_type::TYPE_INT));
    node->type(new basic_type(4, basic_type::TYPE_INT));
  } else if (node->left()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->left()->type(node->right()->type());
    node->type(node->right()->type());
  } else if (node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->right()->type(node->left()->type());
    node->type(node->left()->type());
  } else if (node->left()->type()->name() == node->right()->type()->name())
    node->type(node->left()->type());
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE ||
           node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  else
    throw std::string("wrong type in binary expression");
}

void m19::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void m19::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

void m19::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void m19::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void m19::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

void m19::type_checker::processComparative(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT &&
      node->left()->type()->name() != basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() != basic_type::TYPE_UNSPEC)
    throw std::string("wrong type in left argument of comparative expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT &&
      node->right()->type()->name() != basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() != basic_type::TYPE_UNSPEC)
    throw std::string("wrong type in right argument of comparative expression");

  if (node->right()->type()->name() == basic_type::TYPE_UNSPEC &&
      node->left()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->right()->type(new basic_type(4, basic_type::TYPE_INT));
    node->left()->type(new basic_type(4, basic_type::TYPE_INT));
  } else if (node->right()->type()->name() == basic_type::TYPE_UNSPEC)
    node->right()->type(node->left()->type());
  else if (node->left()->type()->name() == basic_type::TYPE_UNSPEC)
    node->left()->type(node->right()->type());

  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void m19::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  processComparative(node, lvl);
}
void m19::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  processComparative(node, lvl);
}
void m19::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  processComparative(node, lvl);
}
void m19::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  processComparative(node, lvl);
}

void m19::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processComparative(node, lvl);
}
void m19::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processComparative(node, lvl);
}

//---------------------------------------------------------------------------

void m19::type_checker::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  if (id != "@") {
    std::shared_ptr<m19::symbol> symbol = _symtab.find(id);

    if (symbol != nullptr) {
      node->type(symbol->type());
    } else {
      throw id;
    }
  } else {
    node->type(_function->type());
  }
}

void m19::type_checker::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void m19::type_checker::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
  } catch (const std::string &id) {
    throw std::string("undeclared variable '" + id + "'");
  }

  node->rvalue()->accept(this, lvl);
  if (node->lvalue()->type()->name() != basic_type::TYPE_POINTER) {
    if (node->rvalue()->type()->name() == basic_type::TYPE_UNSPEC) {
      node->rvalue()->type(node->lvalue()->type());
      node->type(node->lvalue()->type());
    } else if (node->lvalue()->type()->name() == node->rvalue()->type()->name()) {
      node->type(node->lvalue()->type());
    } else if (node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE &&
               node->rvalue()->type()->name() == basic_type::TYPE_INT) {
      node->type(node->lvalue()->type());
    } else {
      throw std::string("assignment types don't match");
    }
  } else {
    if (node->rvalue()->type()->name() == basic_type::TYPE_POINTER) {
      if (node->rvalue()->type()->subtype()->name() == basic_type::TYPE_UNSPEC) {
        node->rvalue()->type()->_subtype = node->lvalue()->type()->subtype();
      } else if (node->rvalue()->type()->subtype()->name() !=
                 node->lvalue()->type()->subtype()->name()) {
        throw std::string("pointer subtypes need to match");
      }
      node->type(new basic_type(4, basic_type::TYPE_POINTER));
      node->type()->_subtype = node->lvalue()->type()->subtype();
    } else if (node->rvalue()->type()->name() == basic_type::TYPE_INT) {
      cdk::integer_node *literal = dynamic_cast<cdk::integer_node *>(node->rvalue());
      if (literal == nullptr || literal->value() != 0)
        throw std::string("nullptr can only be assigned literal 0");
      node->type(new basic_type(4, basic_type::TYPE_POINTER));
      node->type()->_subtype = new basic_type(0, basic_type::TYPE_VOID);
    } else {
      throw std::string("pointers can only be assigned matching pointers or 0");
    }
  }
}

//---------------------------------------------------------------------------

void m19::type_checker::do_evaluation_node(m19::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void m19::type_checker::do_print_node(m19::print_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() == basic_type::TYPE_UNSPEC)
    node->argument()->type(new basic_type(4, basic_type::TYPE_INT));
}

//---------------------------------------------------------------------------

void m19::type_checker::do_read_node(m19::read_node * const node, int lvl) {
  ASSERT_UNSPEC;

  if (node->type() == nullptr)
    node->type(new basic_type(0, basic_type::TYPE_UNSPEC));
}

//---------------------------------------------------------------------------

void m19::type_checker::do_for_node(m19::for_node * const node, int lvl) {
  node->init()->accept(this, lvl + 4);
  for (size_t i = 0; i < node->condition()->size(); i++) {
    cdk::expression_node* cond = (cdk::expression_node*) node->condition()->node(i);
    cond->accept(this, lvl + 4);
    if (cond->type()->name() != basic_type::TYPE_INT)
      throw "for condition needs to be integer";
  }
  node->incr()->accept(this, lvl + 4);
  node->block()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void m19::type_checker::do_if_node(m19::if_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (node->condition()->type()->name() != basic_type::TYPE_INT)
    throw "if condition needs to be integer";
}

void m19::type_checker::do_if_else_node(m19::if_else_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (node->condition()->type()->name() != basic_type::TYPE_INT)
    throw "if condition needs to be integer";
}

//---------------------------------------------------------------------------

void m19::type_checker::do_mem_alloc_node(m19::mem_alloc_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT)
    throw "mem alloc argument type must be integer";
  node->type(new basic_type(4, basic_type::TYPE_POINTER));
  node->type()->_subtype = new basic_type(0, basic_type::TYPE_UNSPEC);
}
void m19::type_checker::do_address_of_node(m19::address_of_node *const node, int lvl) {
  ASSERT_UNSPEC;
  try{
    node->arg()->accept(this, lvl + 2);
    auto my_type = new basic_type(4, basic_type::TYPE_POINTER);
    my_type->_subtype = new basic_type(node->arg()->type()->size(), node->arg()->type()->name());
    node->type(my_type);
  } catch (const std::string &id) {
    throw std::string("undeclared variable '" + id + "'");
  }
}

void m19::type_checker::do_index_node(m19::index_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->base()->accept(this, lvl+2);
  if (node->base()->type()->name() != basic_type::TYPE_POINTER)
    throw "index base not a pointer.";

  node->offset()->accept(this, lvl+2);
  if (node->offset()->type()->name() != basic_type::TYPE_INT)
    throw "index offset is not an integer.";

  node->type(node->base()->type()->subtype());
}

void m19::type_checker::do_section_conditional_node(m19::section_conditional_node *const node, int lvl) {
  if (node->expression()) {
    node->expression()->accept(this, lvl + 2);
    if (node->expression()->type()->name() != basic_type::TYPE_INT)
      throw "condition is not an integer.";
  }
}
void m19::type_checker::do_identity_node(m19::identity_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void m19::type_checker::do_var_decl_node(m19::var_decl_node *const node, int lvl) {
  if (node->initializer() != nullptr) {
    node->initializer()->accept(this, lvl);
    if (node->initializer()->type()->name() != node->varType()->name())
      if (!(node->varType()->name() == basic_type::TYPE_DOUBLE &&
          node->initializer()->type()->name() == basic_type::TYPE_INT))
        throw std::string("initializer type not compatible with declared variable");
    if (node->qualifier() == 2)
      throw std::string("cannot initialize imported variables");
  }

  const std::string &id = node->identifier();
  std::shared_ptr<m19::symbol> symbol = std::make_shared<m19::symbol>(node->varType(), id, node->qualifier());
  symbol->defined_function(node->initializer() != nullptr);
  if (_symtab.insert(id, symbol))
    _parent->set_new_symbol(symbol);
  else
    throw std::string("variable '" + id + "' redeclared");
}

//---------------------------------------------------------------------------
//----------------------------   FUNCTIONS   --------------------------------
//---------------------------------------------------------------------------

void m19::type_checker::do_function_call_node(m19::function_call_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->identifier();
  if (id == "argc") {
    if (node->arguments()->size() != 0)
      throw std::string("argc doesn't have arguments");
    node->type(new basic_type(4, basic_type::TYPE_INT));
  } else if (id == "argv") {
    if (node->arguments()->size() != 1)
      throw std::string("argv takes one index");
    cdk::expression_node* arg = (cdk::expression_node*) node->arguments()->node(0);
    arg->accept(this, lvl + 2);
    if (arg->type()->name() == basic_type::TYPE_UNSPEC)
      arg->type(new basic_type(4, basic_type::TYPE_INT));
    else if (arg->type()->name() != basic_type::TYPE_INT)
      throw std::string("argv index must be integer");
    node->type(new basic_type(4, basic_type::TYPE_STRING));
  } else if (id == "envp") {
    if (node->arguments()->size() != 1)
      throw std::string("envp takes one index");
    cdk::expression_node* arg = (cdk::expression_node*) node->arguments()->node(0);
    arg->accept(this, lvl + 2);
    if (arg->type()->name() == basic_type::TYPE_UNSPEC)
      arg->type(new basic_type(4, basic_type::TYPE_INT));
    else if (arg->type()->name() != basic_type::TYPE_INT)
      throw std::string("envp index must be integer");
    node->type(new basic_type(4, basic_type::TYPE_STRING));
  } else if (id == "@") {
    if (node->arguments()->size() != _function->arguments().size())
      throw std::string("recursive call with inconsistent arguments");

    for (size_t i = 0; i < node->arguments()->size(); i++) {
      cdk::expression_node* arg = (cdk::expression_node*) node->arguments()->node(i);
      arg->accept(this, lvl + 4);
      if (arg->type()->name() == basic_type::TYPE_UNSPEC)
        arg->type(_function->arguments()[i]);
      else if (arg->type()->name() != _function->arguments()[i]->name())
        throw std::string("argument number " + std::to_string(i) + " is not of the expected type on recursive call");
    }
    node->type(_function->type());
  } else {
    std::shared_ptr<m19::symbol> symbol = _symtab.find(id);
    if (symbol == nullptr) {
      throw std::string("symbol '" + id + "' is undeclared");
    } else if (!symbol->is_function()) {
      throw std::string("'" + id + "' is not a function.");
    } else {
      if (node->arguments()->size() != symbol->arguments().size())
        throw std::string("call arguments number are not the same as the function '" + id + "'");

      for (size_t i = 0; i < node->arguments()->size(); i++) {
        cdk::expression_node* arg = (cdk::expression_node*) node->arguments()->node(i);
        arg->accept(this, lvl + 4);
        if (arg->type()->name() == basic_type::TYPE_UNSPEC)
          arg->type(symbol->arguments()[i]);
        else if (arg->type()->name() != symbol->arguments()[i]->name())
          throw std::string("argument number " + std::to_string(i) + " is not of the expected type");
      }
      node->type(symbol->type());
    }
  }
}

//---------------------------------------------------------------------------

void m19::type_checker::do_function_declaration_node(m19::function_declaration_node *const node, int lvl) {
  const std::string &id = node->identifier();
  std::shared_ptr<m19::symbol> symbol = _symtab.find(id);
  if (symbol != nullptr) {
    if (symbol->is_function()) {
      if (symbol->arguments().size() != node->parameters()->size())
        throw std::string("conflicting declaration of '" + id + "'");
      for (size_t i = 0; i < node->parameters()->size(); i++) {
        m19::var_decl_node* param = (m19::var_decl_node*) node->parameters()->node(i);
        param->accept(this, lvl + 2);
        if (param->qualifier() != 0)
          throw std::string("parameter number " + std::to_string(i) + " of function '" + id + "' can't have special qualifiers");
        if (param->initializer() != nullptr)
          throw std::string("parameter number " + std::to_string(i) + " of function '" + id + "' can't be initialized");
        if (param->varType()->name() != symbol->arguments()[i]->name())
          throw std::string("parameter number " + std::to_string(i) + "is inconsistent with declared function");
      }
    } else
      throw std::string("'" + id + "' is not a function");
  } else {
    std::vector<basic_type*> param_types = std::vector<basic_type*>();
    _symtab.push();
    for (size_t i = 0; i < node->parameters()->size(); i++) {
      m19::var_decl_node* param = (m19::var_decl_node*) node->parameters()->node(i);
      param->accept(this, lvl + 2);
      if (param->qualifier() != 0)
        throw std::string("parameter number " + std::to_string(i) + " of function '" + id + "' can't have special qualifiers");
      if (param->initializer() != nullptr)
        throw std::string("parameter number " + std::to_string(i) + " of function '" + id + "' can't be initialized");
      param_types.push_back(param->varType());
    }
    _symtab.pop();

    symbol = std::make_shared<m19::symbol>(node->type(), id, node->qualifier(), param_types);
    if (!_symtab.insert(id, symbol))
      throw std::string("could not create symbol '" + id + "'");
    _parent->set_new_symbol(symbol);
  }
}

//---------------------------------------------------------------------------

void m19::type_checker::do_function_definition_node(m19::function_definition_node *const node, int lvl) {
  std::string id;
  // "fix" naming issues...
  if (node->identifier() == "m19")
    id = "_main";
  else if (node->identifier() == "_main")
    id = "._main";
  else
    id = node->identifier();

  if (node->default_ret() != nullptr) {
    node->default_ret()->accept(this, lvl + 2);
    if (node->default_ret()->type()->name() != node->type()->name())
      throw "the return type must be the same as the declared type";
  }

  std::vector<basic_type*> param_types = std::vector<basic_type*>();
  _symtab.push();
  for (size_t i = 0; i < node->parameters()->size(); i++) {
    m19::var_decl_node* param = (m19::var_decl_node*) node->parameters()->node(i);
    param->accept(this, lvl + 2);
    param_types.push_back(param->varType());
  }
  _symtab.pop();

  std::shared_ptr<m19::symbol> symbol = _symtab.find(id);
  if (symbol == nullptr) { // function is being declared as well
    symbol = std::make_shared<m19::symbol>(node->type(), id, node->qualifier(), param_types);
    symbol->defined_function(true);
    if (!_symtab.insert(id, symbol))
      throw "could not create identifier '" + id + "'";
    _parent->set_new_symbol(symbol);
  } else if (symbol->defined_function())
    throw "function '" + id + "' is already defined";
  else {
    if (symbol->type()->name() != node->type()->name())
      throw "defined type does not match";
    if (symbol->value() != node->qualifier())
      throw std::string("qualifier not the same as defined one");
    if (symbol->arguments().size() != param_types.size())
      throw "different number of arguments to declared function";
    for (size_t i = 0; i < param_types.size(); i++)
      if (symbol->arguments()[i]->name() != param_types[i]->name())
        throw "type of parameter number " + std::to_string(i) + " does not match the defined function";
    symbol->defined_function(true);
    _symtab.replace(symbol->name(), symbol);
    _parent->set_new_symbol(symbol);
  }
}

//---------------------------------------------------------------------------
//-----------------------     T H E    E N D     ----------------------------
//---------------------------------------------------------------------------
