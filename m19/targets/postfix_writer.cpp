#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/frame_size_calculator.h"
#include "targets/postfix_writer.h"
#include "targets/symbol.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------

void m19::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void m19::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}

void m19::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  if(_inFunctionBody) {
    _pf.DOUBLE(node->value()); //load to stack
  } else {
    _pf.SDOUBLE(node->value()); //static: double on the data segment
  }
}

void m19::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl + 2);
  _pf.INT(0);
  _pf.EQ();
}

void m19::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl)); //if the left argument is 0, no point in checking the right.
  node->right()->accept(this, lvl + 2);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

void m19::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl)); //if the left argument is 1, no point in checking the right.
  node->right()->accept(this, lvl + 2);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  if (_inFunctionBody) {
    _pf.INT(node->value()); // on the stack: push an integer
  } else {
    _pf.SINT(node->value()); // integer literal is on the DATA segment
  }
}

void m19::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value()); // output string characters

  if (_function) {
    //local variable initializer
    _pf.TEXT(); // return to the TEXT segment
    _pf.ADDR(mklbl(lbl1)); // the string to be printed
  } else {
    //global variable initializers
    _pf.DATA();
    _pf.SADDR(mklbl(lbl1));
  }
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE && node->left()->type()->name() == basic_type::TYPE_INT) {
    _pf.I2D();
  } /* else if (node->type()->name() == basic_type::TYPE_POINTER && node->left()->type()->name() == basic_type::TYPE_INT) {
    _pf.INT();
    _pf.SHTL();
  } */

  node->right()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT) {
    _pf.I2D();
  } /* else if (node->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_INT) {
    _pf.INT();
    _pf.SHTL();
  }*/

  if (node->type()->name() == basic_type::TYPE_DOUBLE)
  _pf.DADD(); //add of doubles
  else
  _pf.ADD(); //add of integers
}

void m19::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE && node->left()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT) {
    _pf.I2D();
  } /* else if (node->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_INT) {
    _pf.INT();
    _pf.SHTL();
  } */

  if (node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DSUB();
  else
    _pf.SUB();
}

void m19::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE && node->left()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();

  if(node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DMUL();
  else
    _pf.MUL();
}

void m19::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE && node->left()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();

  if(node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DDIV();
  else
    _pf.DIV();
}

void m19::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  _pf.MOD();
}

void m19::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  if (node->left()->type()->name() == basic_type::TYPE_DOUBLE ||  node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.LT();

}

void m19::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  if (node->left()->type()->name() == basic_type::TYPE_DOUBLE ||  node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.LE();
}

void m19::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  if (node->left()->type()->name() == basic_type::TYPE_DOUBLE ||  node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.GE();
}

void m19::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  if (node->left()->type()->name() == basic_type::TYPE_DOUBLE ||  node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.GT();
}

void m19::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  if (node->left()->type()->name() == basic_type::TYPE_DOUBLE ||  node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.NE();
}

void m19::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();

  if (node->left()->type()->name() == basic_type::TYPE_DOUBLE ||  node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.EQ();
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  const std::string &id = node->name();
  if (id == "@") {
    _pf.LOCAL(-_function->type()->size());
  } else {
    std::shared_ptr<m19::symbol> symbol = _symtab.find(id);

    if (symbol->global()) {
      _pf.ADDR(symbol->name());
    } else {
      _pf.LOCAL(symbol->offset());
    }
  }
}

void m19::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->lvalue()->accept(this, lvl);
  if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.LDDOUBLE();
  } else {
    _pf.LDINT();
  }
}

void m19::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->rvalue()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    if (node->rvalue()->type()->name() == basic_type::TYPE_INT) {
      _pf.I2D();
    }
    _pf.DUP64();
  } else {
    _pf.DUP32();
  }

  node->lvalue()->accept(this, lvl);
  if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.STDOUBLE();
  } else {
    _pf.STINT();
  }
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_evaluation_node(m19::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  basic_type *type = node->argument()->type();
  node->argument()->accept(this, lvl);
  _pf.TRASH(type->size());
}

void m19::postfix_writer::do_print_node(m19::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _functions_to_declare.insert("printi");
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _functions_to_declare.insert("prints");
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _functions_to_declare.insert("printd");
    _pf.CALL("printd");
    _pf.TRASH(8); // trash double
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  if (node->ln()) {
    _functions_to_declare.insert("println");
    _pf.CALL("println"); // print a newline
  }
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_read_node(m19::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->type()->name() == basic_type::TYPE_INT) {
    _functions_to_declare.insert("readi");
    _pf.CALL("readi");
    _pf.LDFVAL32();
  } else if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    _functions_to_declare.insert("readd");
    _pf.CALL("readd");
    _pf.LDFVAL64();
  } else {
    error(node->lineno(), "cannot read type");
  }
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_for_node(m19::for_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _symtab.push();

  int condition = ++_lbl;
  int end = ++_lbl;
  int increment = ++_lbl; //so we know where to jump in case of a continue

  _break.push(end);
  _continue.push(increment);

  //initialization
  node->init()->accept(this, lvl);

  //stopping conditions
  _pf.LABEL(mklbl(condition));
  for (size_t i = 0; i < node->condition()->size(); i++) {
    cdk::expression_node* cond = (cdk::expression_node*) node->condition()->node(i);
    cond->accept(this, lvl);
    _pf.JZ(mklbl(end));
  }

  //the actual code
  node->block()->accept(this, lvl + 2);

  //incrementing the cycle
  _pf.LABEL(mklbl(increment));
  node->incr()->accept(this, lvl);
  _pf.JMP(mklbl(condition));

  //end of cycle
  _pf.LABEL(mklbl(end));

  //pops
  _break.pop();
  _continue.pop();

  _symtab.pop();
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_if_node(m19::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl_endif;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl_endif = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl_endif));
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_if_else_node(m19::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl_else, lbl_endif;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl_else = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl_endif = ++_lbl));
  _pf.LABEL(mklbl(lbl_else));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl_else = lbl_endif));
}  //check if there's no else block?
  //then why would we be using the if_else_node...

//---------------------------------------------------------------------------

void m19::postfix_writer::do_mem_alloc_node(m19::mem_alloc_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl);
  _pf.INT(node->type()->subtype()->size());
  _pf.MUL();
  _pf.ALLOC();
  _pf.SP();
}
void m19::postfix_writer::do_address_of_node(m19::address_of_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // since the argument is an lvalue, it is already an address
  node->arg()->accept(this, lvl + 2);
}

void m19::postfix_writer::do_block_node(m19::block_node *const node, int lvl) {
  _symtab.push(); // for block-local vars

  if (node->declarations()->size() > 0)
    node->declarations()->accept(this, lvl + 2);

  if (node->instructions()->size() > 0)
    node->instructions()->accept(this, lvl + 2);

  _symtab.pop();
}

void m19::postfix_writer::do_break_node(m19::break_node *const node, int lvl) {
  int break_lbl = _break.top();
  _pf.JMP(mklbl(break_lbl));
}
void m19::postfix_writer::do_continue_node(m19::continue_node *const node, int lvl) {
  int continue_lbl = _continue.top();
  _pf.JMP(mklbl(continue_lbl));
}

void m19::postfix_writer::do_index_node(m19::index_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
    node->base()->accept(this, lvl);
    node->offset()->accept(this, lvl);

    _pf.INT(node->offset()->type()->subtype()->size()); //get the pointer's subtybe
    _pf.MUL(); //subtype size times the offset to get how much we have to "travel"
    _pf.ADD(); //go to the desired index
}

void m19::postfix_writer::do_return_node(m19::return_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (_inFinalSection) {
    //salta para retorno (fim do body)
    _pf.JMP(mklbl(_body_lbl));

  } else if (_inFunctionBody) {
    //salta para final (fim das conditional sections)
    _pf.JMP(mklbl(_sections_lbl));
  }
}

void m19::postfix_writer::do_identity_node(m19::identity_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}
void m19::postfix_writer::do_var_decl_node(m19::var_decl_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto id = node->identifier();
  int offset = 0, typesize = node->varType()->size();// in bytes

  if (_inFunctionBody) {
    _offset -= typesize;
    offset = _offset;
  } else if (_inFunctionArgs) {
    offset = _offset;
    _offset += typesize;
  } else {
    offset = 0; // global variable
  }

  std::shared_ptr<m19::symbol> symbol = new_symbol();
  if (symbol) {
    symbol->set_offset(offset);
    reset_new_symbol();
  }

  if (_inFunctionBody) {
    // if we are dealing with local variables, then no action is needed
    // unless an initializer exists
    if (node->initializer()) {
      node->initializer()->accept(this, lvl);
      if (node->varType()->name() == basic_type::TYPE_INT || node->varType()->name() == basic_type::TYPE_STRING
          || node->varType()->name() == basic_type::TYPE_POINTER) {
        _pf.LOCAL(symbol->offset());
        _pf.STINT();
      } else if (node->varType()->name() == basic_type::TYPE_DOUBLE) {
        _pf.LOCAL(symbol->offset());
        _pf.STDOUBLE();
      } else {
        std::cerr << "cannot initialize" << std::endl;
      }
    }

  } else {

    if (!_function) {
      if (node->initializer() == nullptr) {
        if (node->qualifier() != 2) {
          _pf.BSS();
          _pf.ALIGN();
          if (node->qualifier() == 1) _pf.GLOBAL(id, _pf.OBJ());
          _pf.LABEL(id);
          _pf.SALLOC(typesize);
        } else {
          _functions_to_declare.insert(node->identifier());
        }
      } else {
        if (node->varType()->name() == basic_type::TYPE_INT || node->varType()->name() == basic_type::TYPE_DOUBLE ||
            node->varType()->name() == basic_type::TYPE_POINTER || node->varType()->name() == basic_type::TYPE_STRING) {
          _pf.DATA();
          _pf.ALIGN();
          if (node->qualifier() == 1) _pf.GLOBAL(id, _pf.OBJ());
          _pf.LABEL(id);

          if (node->varType()->name() == basic_type::TYPE_DOUBLE &&
              node->initializer()->type()->name() == basic_type::TYPE_INT) {
            cdk::integer_node *dclini = dynamic_cast<cdk::integer_node *>(node->initializer());
            cdk::double_node ddi(dclini->lineno(), dclini->value());
            ddi.accept(this, lvl);
          } else
            node->initializer()->accept(this, lvl);
        } else {
          error(node->lineno(), ": '" + id + "' has unexpected initializer\n");
        }
      }
    }

  }
}

//---------------------------------------------------------------------------
//----------------------------   FUNCTIONS   --------------------------------
//---------------------------------------------------------------------------

void m19::postfix_writer::do_section_conditional_node(m19::section_conditional_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int lbl1;
  if (node->expression()) {
    node->expression()->accept(this, lvl);
    _pf.JZ(mklbl(lbl1 = ++_lbl));
  }

  node->block()->accept(this, lvl + 2);

  if(node->qualifier() == 1)
    _pf.JMP(mklbl(_sections_lbl));
  if (node->expression()) {
    _pf.LABEL(mklbl(lbl1));
  }
}
void m19::postfix_writer::do_section_initial_node(m19::section_initial_node *const node, int lvl) {
  node->block()->declarations()->accept(this, lvl + 4);
  node->block()->instructions()->accept(this, lvl + 4);
}
void m19::postfix_writer::do_section_final_node(m19::section_final_node *const node, int lvl) {
  _inFinalSection = true;
  node->block()->accept(this, lvl + 2);
  _inFinalSection = false;
}
void m19::postfix_writer::do_body_node(m19::body_node *const node, int lvl) {
  if (node->initial()) {
    _symtab.push();
    node->initial()->accept(this, lvl + 2);
  }

  node->sections()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(_sections_lbl = ++_lbl));

  if (node->fin())
    node->fin()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(_body_lbl = ++_lbl));

  if (node->initial())
    _symtab.pop();
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_function_call_node(m19::function_call_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (node->identifier() == "argc")
    _functions_to_declare.insert("argc");
  else if (node->identifier() == "argv")
    _functions_to_declare.insert("argv");
  else if (node->identifier() == "envp")
    _functions_to_declare.insert("envp");

  size_t argsSize = 0;
  if (node->arguments()) {
    for (int ax = node->arguments()->size(); ax > 0; ax--) {
      cdk::expression_node *arg = dynamic_cast<cdk::expression_node*>(node->arguments()->node(ax - 1));
      arg->accept(this, lvl + 2);
      argsSize += arg->type()->size();
    }
  }

  basic_type *type;
  if (node->identifier() == "@") {
    type = _function->type();
    _pf.CALL(_function->name());
  } else {
    std::shared_ptr<m19::symbol> symbol = _symtab.find(node->identifier());
    type = symbol->type();
    _pf.CALL(node->identifier());
  }

  if (argsSize != 0) {
    _pf.TRASH(argsSize);
  }

  if (type->name() == basic_type::TYPE_INT || type->name() == basic_type::TYPE_POINTER || type->name() == basic_type::TYPE_STRING) {
    _pf.LDFVAL32();
  }
  else if (type->name() == basic_type::TYPE_DOUBLE) {
    _pf.LDFVAL64();
  }
  else {
    // cannot happer!
  }
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_function_declaration_node(m19::function_declaration_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (_inFunctionBody) {
    error(node->lineno(), "cannot declare function in body");
    return;
  }

  if (!new_symbol()) return;

  std::shared_ptr<m19::symbol> function = new_symbol();
  reset_new_symbol();
  if (function->value() == 2)
    _functions_to_declare.insert(function->name());
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_function_definition_node(m19::function_definition_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if(_inFunctionBody) {
    error(node->lineno(), "cannot define function in body");
    return;
  }

  _function = new_symbol();
  reset_new_symbol();

  _offset = 8;
  _symtab.push();
  if (node->parameters()->size() > 0) {
    _inFunctionArgs = true;
    for (size_t i = 0; i < node->parameters()->size(); i++) {
      node->parameters()->node(i)->accept(this, 0);
    }
    _inFunctionArgs = false;
  }

  _pf.TEXT();
  _pf.ALIGN();
  if (_function->value() == 1) _pf.GLOBAL(_function->name(), _pf.FUNC());
  _pf.LABEL(_function->name());

  // compute stack size to be reserved for local variables
  frame_size_calculator lsc(_compiler, _symtab);
  node->accept(&lsc, lvl);
  _pf.ENTER(lsc.localsize()); // total stack size reserved for local variables

  _inFunctionBody = true;

  if (node->default_ret()) {
    node->default_ret()->accept(this, lvl);
  } else if (_function->type()->name() == basic_type::TYPE_INT || 
             _function->type()->name() == basic_type::TYPE_POINTER) {
    _pf.INT(0);
  }

  if (node->default_ret() != nullptr &&
      node->default_ret()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.LOCAL(-8);
    _pf.STDOUBLE();
  } else if (node->default_ret() != nullptr) {
    _pf.LOCA(-4);
  } else if (_function->type()->name() != basic_type::TYPE_VOID &&
             _function->type()->name() !=basic_type::TYPE_STRING) {
    _pf.LOCA(-4);
  }

  _offset = -_function->type()->size();
  os() << "        ;; before body " << std::endl;
  node->body()->accept(this, lvl + 4); // block has its own scope
  os() << "        ;; after body " << std::endl;
  _inFunctionBody = false;
  _symtab.pop(); // scope of arguments

  if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.LOCAL(-8);
    _pf.LDDOUBLE();
    _pf.STFVAL64();
  } else if (node->type()->name() != basic_type::TYPE_VOID){
    _pf.LOCV(-4);
    _pf.STFVAL32();
  }
  _pf.LEAVE();
  _pf.RET();

  if (node->identifier() == "m19") {
    for (std::string s : _functions_to_declare)
      _pf.EXTERN(s);
  }
}

//---------------------------------------------------------------------------
//-----------------------     T H E    E N D     ----------------------------
//---------------------------------------------------------------------------
