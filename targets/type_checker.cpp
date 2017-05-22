#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//---------------------------------------------------------------------------

void xpl::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void xpl::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

//---------------------------------------------------------------------------

inline void xpl::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in argument of unary expression");

  // in XPL, expressions are always int
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void xpl::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

inline void xpl::type_checker::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in right argument of binary expression");

  // in XPL, expressions are always int
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void xpl::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<xpl::symbol> symbol = _symtab.find(id);

  if (symbol != nullptr) {
    node->type(symbol->type());
  }
  else {
    throw id;
  }
}

void xpl::type_checker::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void xpl::type_checker::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;

  try {
    node->lvalue()->accept(this, lvl);
  } catch (const std::string &id) {
    std::shared_ptr<xpl::symbol> symbol = std::make_shared<xpl::symbol>(new basic_type(4, basic_type::TYPE_INT), id, 0);
    _symtab.insert(id, symbol);
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
    node->lvalue()->accept(this, lvl);  //DAVID: bah!
  }

  if (node->lvalue()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in left argument of assignment expression");

  node->rvalue()->accept(this, lvl + 2);
  if (node->rvalue()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in right argument of assignment expression");

  // in XPL, expressions are always int
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void xpl::type_checker::do_print_node(xpl::print_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_read_node(xpl::read_node * const node, int lvl) {
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_while_node(xpl::while_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  node->block()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_if_node(xpl::if_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  node->block()->accept(this, lvl + 4);
}

void xpl::type_checker::do_if_else_node(xpl::if_else_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  node->thenblock()->accept(this, lvl + 4);
  node->elseblock()->accept(this, lvl + 4);
}

/* XPL */

void xpl::type_checker::do_if_elsif_else_node(xpl::if_elsif_else_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  node->thenblock()->accept(this, lvl + 4);
  /*for (size_t i = 0; i < node->elsifs()->size(); i++) {
    node->elsifs()->node(i)->accept(this, lvl + 4)
  }*/
  node->elseblock()->accept(this, lvl + 4);
}

void xpl::type_checker::do_sweep_node(xpl::sweep_node * const node, int lvl) {
  node->iterable()->accept(this, lvl + 4);
  if (node->iterable()->type()->name() != basic_type::TYPE_INT)
    throw std::string("iterable must be an integer");

  node->initial_value()->accept(this, lvl + 4);
  if (node->initial_value()->type()->name() != basic_type::TYPE_INT)
    throw std::string("initial value must be an integer");

  node->final_value()->accept(this, lvl + 4);
  if (node->final_value()->type()->name() != basic_type::TYPE_INT)
    throw std::string("final value must be an integer");

  node->increment()->accept(this, lvl + 4);
  if (node->final_value()->type()->name() != basic_type::TYPE_INT)
    throw std::string("increment value must be an integer");

  node->block()->accept(this, lvl + 4);
}

void xpl::type_checker::do_stop_node(xpl::stop_node * const node, int lvl) { /* empty */ }

void xpl::type_checker::do_next_node(xpl::next_node * const node, int lvl) { /* empty */ }

void xpl::type_checker::do_return_node(xpl::return_node * const node, int lvl) { /* empty */ }

void check_function_types(basic_type * const type, basic_type * const ret) {
  if (type->name() == basic_type::TYPE_DOUBLE && ret->name() == basic_type::TYPE_INT)
    return; // valid exception
  if (type->name() != ret->name())
    throw std::string("function type and return are incompatible");
}

void check_function_arguments(cdk::sequence_node * const args) {
  if (args != nullptr) {
    for (size_t i = 0; i < args->size(); i++) {
      xpl::declaration_node *arg = (xpl::declaration_node*) args->node(i);
      if (arg->qualifier() != nullptr)
        throw std::string("function arguments cannot have public/use modifiers");
    }
  }
}

void xpl::type_checker::do_declaration_node(xpl::declaration_node * const node, int lvl) {
  if (node->type()->name() == basic_type::TYPE_VOID)
    throw std::string("variable type cannot be void/procedure");

  std::string id = *node->identifier();
  if (_symtab.find_local(id) == nullptr) {
    _symtab.insert(id, std::make_shared<xpl::symbol>(node->type(), id, 0));
  } else {
    throw std::string("attempted to redefine local symbol: " + id);
  }

  if (node->value() != nullptr) {
    node->value()->accept(this, lvl + 2);
    if (node->type()->name() != node->value()->type()->name())
      throw std::string("variable type and return value are incompatible");
  }
}

void xpl::type_checker::do_function_declaration_node(xpl::function_declaration_node * const node, int lvl) {
  std::string id = *node->identifier();
  if (_symtab.find(id) != nullptr)
    throw std::string("function " + id + " is already defined");

  check_function_arguments(node->arguments());
}

void xpl::type_checker::do_function_definition_node(xpl::function_definition_node * const node, int lvl) {
  std::string id = *node->identifier();
  if (_symtab.find(id) != nullptr)
    throw std::string("function " + id + " is already defined");

  check_function_arguments(node->arguments());

  if (node->ret_val() != nullptr) {
    node->ret_val()->accept(this, lvl);
    check_function_types(node->type(), node->ret_val()->type());
  }
}

void xpl::type_checker::do_function_call_node(xpl::function_call_node * const node, int lvl) {
  ASSERT_UNSPEC;
}

void xpl::type_checker::do_block_node(xpl::block_node * const node, int lvl) {
  if (node->declarations() != nullptr) node->declarations()->accept(this, lvl);
  if (node->instructions() != nullptr) node->instructions()->accept(this, lvl);
}

void xpl::type_checker::do_memory_allocation_node(xpl::memory_allocation_node * const node, int lvl) {}

void xpl::type_checker::do_identity_node(xpl::identity_node * const node, int lvl) {
  if (node->argument()->type()->name() != basic_type::TYPE_INT &&
      node->argument()->type()->name() != basic_type::TYPE_DOUBLE)
    throw std::string("identity operator is only valid for numbers");
}

void xpl::type_checker::do_symmetry_node(xpl::symmetry_node * const node, int lvl) {
  if (node->argument()->type()->name() != basic_type::TYPE_INT &&
      node->argument()->type()->name() != basic_type::TYPE_DOUBLE)
      throw std::string("symmetry operator is only valid for numbers");
}

void xpl::type_checker::do_index_node(xpl::index_node * const node, int lvl) {
  ASSERT_UNSPEC;
  std::string id = *node->identifier();
  if (_symtab.find(id) == nullptr)
    throw std::string("variable " + id + " is not defined");

  if (node->offset()->type()->name() != basic_type::TYPE_INT)
    throw std::string("offset expression must be an integer");
}
