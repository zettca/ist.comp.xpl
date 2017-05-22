#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/size_checker.h"
#include "targets/postfix_writer.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  _pf.INT(node->value()); // push an integer
}

void xpl::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.STR(node->value()); // output string characters

  /* leave the address on the stack */
  _pf.TEXT(); // return to the TEXT segment
  _pf.ADDR(mklbl(lbl1)); // the string to be printed
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.ADD();
}
void xpl::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.SUB();
}
void xpl::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MUL();
}
void xpl::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.DIV();
}
void xpl::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void xpl::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}
void xpl::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}
void xpl::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}
void xpl::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}
void xpl::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void xpl::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // simplified generation: all variables are global
  _pf.ADDR(node->name());
}

void xpl::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  _pf.LOAD(); // depends on type size
}

void xpl::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl); // determine the new value
  _pf.DUP();
  if (new_symbol() == nullptr) {
    node->lvalue()->accept(this, lvl); // where to store the value
  } else {
    _pf.DATA(); // variables are all global and live in DATA
    _pf.ALIGN(); // make sure we are aligned
    _pf.LABEL(new_symbol()->name()); // name variable location
    _pf.CONST(0); // initialize it to 0 (zero)
    _pf.TEXT(); // return to the TEXT segment
    node->lvalue()->accept(this, lvl);  //DAVID: bah!
  }
  _pf.STORE(); // store the value at address
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.TRASH(4); // delete the evaluated value
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.TRASH(4); // delete the evaluated value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void xpl::postfix_writer::do_print_node(xpl::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("printd");
    _pf.TRASH(8); // delete the printed value's address
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }

  if (node->newline()){
    _pf.CALL("println"); // print a newline
    _pf.EXTERN("prints");
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_read_node(xpl::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.CALL("readi");
  _pf.PUSH();
  //node->argument()->accept(this, lvl);
  _pf.STORE();
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_while_node(xpl::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  _pf.LABEL(mklbl(lbl1 = ++_lbl));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl2 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl1));
  _pf.LABEL(mklbl(lbl2));
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_if_node(xpl::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_if_else_node(xpl::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

void xpl::postfix_writer::do_if_elsif_else_node(xpl::if_elsif_else_node * const node, int lvl) {

}

void xpl::postfix_writer::do_sweep_node(xpl::sweep_node * const node, int lvl) {}

void xpl::postfix_writer::do_stop_node(xpl::stop_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // _pf.JMP(mklbl(_pos));
}

void xpl::postfix_writer::do_next_node(xpl::next_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // _pf.JMP(mklbl(_pos));
}

void xpl::postfix_writer::do_return_node(xpl::return_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // _pf.JMP(mklbl(_pos));
}

void xpl::postfix_writer::do_declaration_node(xpl::declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  std::string id = *node->identifier();
  std::string qual = *node->qualifier();
  std::shared_ptr<zu::symbol> symbol = _symtab.find_local(id);

  bool has_value = false;
  if(node->value() != nullptr)
    has_value = true;

  _current_offset -= node->type()->size();
  symbol->offset(_current_offset);

  if (has_value){
    node->value()->accept(this, lvl + 2);
    _pf.LOCAL(symbol->offset());

    if (is_real(node->type())) {
      _pf.DSTORE();
    } else {
      _pf.STORE();
    }
  }
}

void xpl::postfix_writer::do_function_declaration_node(xpl::function_declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  std::string id = parse_id(*node->identifier());
  std::string qual = *node->qualifier();
  if (qual == "public") {
    _pf.GLOBAL(id, _pf.FUNC());
  } else if (qual == "use") {
    _pf.EXTERN(id);
  }
}

void xpl::postfix_writer::do_function_definition_node(xpl::function_definition_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.TEXT();
  _pf.ALIGN();

  std::string id = parse_id(*node->identifier());
  std::string qual = *node->qualifier();
  if (qual == "public")
    _pf.GLOBAL(id, _pf.FUNC());
  _pf.LABEL(id);

  /*xpl::size_checker *visitor = new xpl::size_checker(_compiler);
  node->accept(visitor, 0);
  int body_size = visitor->size();
  delete visitor;*/

  int ret_size = (node->type()->name() == basic_type::TYPE_DOUBLE) ? 8 : 4;

  _pf.ENTER(4 /* body_size */);

  if (node->ret_val() != nullptr) {
    node->ret_val()->accept(this, lvl + 2);
    if (ret_size == 8) {
      _pf.I2D();
      _pf.LOCAL(-8);
      _pf.DSTORE();
    } else if (ret_size == 4) {
      _pf.LOCA(-4);
    }
  } else {
    if (node->type() != nullptr) { // default non-error return
      _pf.INT(0);
      _pf.LOCA(-4);
    }
  }

  _symtab.push();

  node->block()->accept(this, lvl + 2);

  _pf.ALIGN();
  _pf.LABEL(mklbl(++_lbl));

  if( ret_size == 8){
    _pf.LOCAL(-8);
    _pf.DLOAD();
    _pf.DPOP();
  } else if( ret_size == 4){
    _pf.LOCAL(-4);
    _pf.LOAD();
    _pf.POP();
  }

  _pf.LEAVE();
  _pf.RET();

  _symtab.pop();
}

void xpl::postfix_writer::do_function_call_node(xpl::function_call_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int size = 0;
  if (node->arguments() != nullptr) {
    for (int i = node->arguments()->size() - 1; i >= 0; i--) {
      node->arguments()->node(i)->accept(this, lvl);
      size += ((cdk::expression_node *) node->arguments()->node(i))->type()->size();
    }
  }

  std::string id = parse_id(*node->identifier());
  _pf.CALL(id);

  _pf.TRASH(size);
  _pf.PUSH(); // or DPUSH
}

void xpl::postfix_writer::do_block_node(xpl::block_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _symtab.push();
  if (node->declarations() != nullptr)
    node->declarations()->accept(this, lvl + 2);
  if (node->instructions() != nullptr)
    node->instructions()->accept(this, lvl + 2);
  _symtab.pop();
}

void xpl::postfix_writer::do_memory_allocation_node(xpl::memory_allocation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  _pf.INT(4);
  _pf.MUL();
  _pf.ALLOC();
  _pf.SP();
}

void xpl::postfix_writer::do_identity_node(xpl::identity_node * const node, int lvl) {
  if (is_real(node->type())) {
    _pf.CALL("readd");
    _pf.EXTERN("readd");
    _pf.DPUSH();
  } else if (is_int(node->type())){
    _pf.CALL("readi");
    _pf.EXTERN("readi");
    _pf.PUSH();
  } else {
    throw std::string("can only read ints/reals");
  }
}

void xpl::postfix_writer::do_symmetry_node(xpl::symmetry_node * const node, int lvl) {}

void xpl::postfix_writer::do_index_node(xpl::index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int size = (node->type()->name() == basic_type::TYPE_DOUBLE) ? 8 : 4;
  node->offset()->accept(this, lvl); // expr result
  _pf.INT(size);
  _pf.MUL();
}
