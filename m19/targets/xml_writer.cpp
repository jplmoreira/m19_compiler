#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

std::string global_qualifier(int qualifier) {
  switch(qualifier) {
    case 0: return "private";

    case 1: return "public";

    case 2: return "import";

    default: return "error";
  }
}

std::string section_qualifier(int qualifier) {
  switch(qualifier) {
    case 0: return "inclusive";

    case 1: return "exclusive";

    default: return "error";
  }
}

//---------------------------------------------------------------------------

void m19::xml_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}
void m19::xml_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void m19::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  process_literal(node, lvl);
}
void m19::xml_writer::do_not_node(cdk::not_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}
void m19::xml_writer::do_and_node(cdk::and_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_or_node(cdk::or_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void m19::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void m19::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  process_literal(node, lvl);
}

void m19::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  process_literal(node, lvl);
}

//---------------------------------------------------------------------------

void m19::xml_writer::do_unary_expression(cdk::unary_expression_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void m19::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void m19::xml_writer::do_binary_expression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void m19::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void m19::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void m19::xml_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">" << std::endl;
}

void m19::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

void m19::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  node->lvalue()->accept(this, lvl);
  reset_new_symbol();

  node->rvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

// void m19::xml_writer::do_program_node(m19::program_node * const node, int lvl) {
//   openTag(node, lvl);
//   node->statements()->accept(this, lvl + 4);
//   closeTag(node, lvl);
// }

//---------------------------------------------------------------------------

void m19::xml_writer::do_evaluation_node(m19::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void m19::xml_writer::do_print_node(m19::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void m19::xml_writer::do_read_node(m19::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void m19::xml_writer::do_for_node(m19::for_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("init", lvl + 2);
  node->init()->accept(this, lvl + 4);
  closeTag("init", lvl + 2);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("incr", lvl + 2);
  node->incr()->accept(this, lvl + 4);
  closeTag("incr", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void m19::xml_writer::do_if_node(m19::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void m19::xml_writer::do_if_else_node(m19::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void m19::xml_writer::do_mem_alloc_node(m19::mem_alloc_node *const node, int lvl) {
  do_unary_expression(node, lvl);
}
void m19::xml_writer::do_address_of_node(m19::address_of_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->arg()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_block_node(m19::block_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("declarations", lvl + 2);
  node->declarations()->accept(this, lvl + 4);
  closeTag("declarations", lvl + 2);
  openTag("instructions", lvl + 2);
  node->instructions()->accept(this, lvl + 4);
  closeTag("instructions", lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_break_node(m19::break_node *const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}
void m19::xml_writer::do_continue_node(m19::continue_node *const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}
void m19::xml_writer::do_function_call_node(m19::function_call_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("identifier", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->identifier() << std::endl;
  closeTag("identifier", lvl + 2);
  openTag("arguments", lvl + 2);
  node->arguments()->accept(this, lvl + 4);
  closeTag("arguments", lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_function_declaration_node(m19::function_declaration_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("qualifier", lvl + 2);
  os() << std::string(lvl + 4, ' ') << global_qualifier(node->qualifier()) << std::endl;
  closeTag("qualifier", lvl + 2);
  openTag("type", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->type() << std::endl;
  closeTag("type", lvl + 2);
  openTag("identifier", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->identifier() << std::endl;
  closeTag("identifier", lvl + 2);
  openTag("parameters", lvl + 2);
  node->parameters()->accept(this, lvl + 4);
  closeTag("parameters", lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_function_definition_node(m19::function_definition_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("qualifier", lvl + 2);
  os() << std::string(lvl + 4, ' ') << global_qualifier(node->qualifier()) << std::endl;
  closeTag("qualifier", lvl + 2);
  openTag("type", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->type() << std::endl;
  closeTag("type", lvl + 2);
  openTag("identifier", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->identifier() << std::endl;
  closeTag("identifier", lvl + 2);
  openTag("parameters", lvl + 2);
  node->parameters()->accept(this, lvl + 4);
  closeTag("parameters", lvl + 2);
  openTag("body", lvl + 2);
  node->body()->accept(this, lvl + 4);
  closeTag("body", lvl + 2);
  if (node->default_ret() != nullptr) {
    openTag("default_ret", lvl + 2);
    node->default_ret()->accept(this, lvl + 4);
    closeTag("default_ret", lvl + 2);
  }
  closeTag(node, lvl);
}
void m19::xml_writer::do_index_node(m19::index_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("base", lvl + 2);
  node->base()->accept(this, lvl + 4);
  closeTag("base", lvl + 2);
  openTag("offset", lvl + 2);
  node->offset()->accept(this, lvl + 4);
  closeTag("offset", lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_section_conditional_node(m19::section_conditional_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("qualifier", lvl + 2);
  os() << std::string(lvl + 4, ' ') << section_qualifier(node->qualifier()) << std::endl;
  closeTag("qualifier", lvl + 2);
  if(node->expression() != nullptr) {
    openTag("expression", lvl + 2);
    node->expression()->accept(this, lvl + 4);
    closeTag("expression", lvl + 2);
  }
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_section_initial_node(m19::section_initial_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_section_final_node(m19::section_final_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_return_node(m19::return_node *const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}
void m19::xml_writer::do_identity_node(m19::identity_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("argument", lvl+2);
  node->argument()->accept(this, lvl + 4);
  closeTag("argument", lvl+2);
  closeTag(node, lvl);
}
void m19::xml_writer::do_var_decl_node(m19::var_decl_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("qualifier", lvl + 2);
  os() << std::string(lvl + 4, ' ') << global_qualifier(node->qualifier()) << std::endl;
  closeTag("qualifier", lvl + 2);
  openTag("varType", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->varType() << std::endl;
  closeTag("varType", lvl + 2);
  openTag("identifier", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->identifier() << std::endl;
  closeTag("identifier", lvl + 2);
  if(node->initializer() != nullptr) {
    openTag("initializer", lvl + 2);
    node->initializer()->accept(this, lvl + 4);
    closeTag("initializer", lvl + 2);
  }
  closeTag(node, lvl);
}
void m19::xml_writer::do_body_node(m19::body_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  if(node->initial() != nullptr) {
    openTag("initial", lvl + 2);
    node->initial()->accept(this, lvl + 4);
    closeTag("initial", lvl + 2);
  }
  openTag("sections", lvl + 2);
  node->sections()->accept(this, lvl + 4);
  closeTag("sections", lvl + 2);
  if (node->fin() != nullptr) {
    openTag("fin", lvl + 2);
    node->fin()->accept(this, lvl + 4);
    closeTag("fin", lvl + 2);
  }
  closeTag(node, lvl);
}
