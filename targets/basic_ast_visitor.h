#ifndef __XPL_BASIC_AST_VISITOR_H__
#define __XPL_BASIC_AST_VISITOR_H__

#include <string>
#include <memory>
#include <iostream>
#include <cdk/compiler.h>
#include "symbol.h"

/* include node forward declarations */
#define __NODE_DECLARATIONS_ONLY__
#include "ast/all.h"  // automatically generated
#undef __NODE_DECLARATIONS_ONLY__

//!
//! Print nodes as XML elements to the output stream.
//!
class basic_ast_visitor {
protected:
  //! The owner compiler
  std::shared_ptr<cdk::compiler> _compiler;

private:

  // last symbol inserted in symbol table
  std::shared_ptr<xpl::symbol> _new_symbol;

protected:
  basic_ast_visitor(std::shared_ptr<cdk::compiler> compiler) :
      _compiler(compiler) {
  }

  inline bool debug() {
    return _compiler->debug();
  }

  inline std::ostream &os() {
    return *_compiler->ostream();
  }

public:
  virtual ~basic_ast_visitor() {
  }

public:
  std::shared_ptr<xpl::symbol> new_symbol() {
    return _new_symbol;
  }

  void set_new_symbol(std::shared_ptr<xpl::symbol> symbol) {
    _new_symbol = symbol;
  }

  void reset_new_symbol() {
    _new_symbol = nullptr;
  }

public:
  // CDK nodes (general)
  virtual void do_nil_node(cdk::nil_node * const node, int lvl) {
    // XPL does not use this node
  }
  virtual void do_data_node(cdk::data_node * const node, int lvl) {
    // XPL does not use this node
  }
  virtual void do_sequence_node(cdk::sequence_node * const node, int lvl) = 0;

public:
  virtual void do_integer_node(cdk::integer_node * const node, int lvl) = 0;
  virtual void do_double_node(cdk::double_node * const node, int lvl) {
    // XPL does not use this node
  }
  virtual void do_string_node(cdk::string_node * const node, int lvl) = 0;

public:
  virtual void do_neg_node(cdk::neg_node * const node, int lvl) = 0;
  virtual void do_not_node(cdk::not_node * const node, int lvl) {
    // not used
  }

public:
  virtual void do_add_node(cdk::add_node * const node, int lvl) = 0;
  virtual void do_sub_node(cdk::sub_node * const node, int lvl) = 0;
  virtual void do_mul_node(cdk::mul_node * const node, int lvl) = 0;
  virtual void do_div_node(cdk::div_node * const node, int lvl) = 0;
  virtual void do_mod_node(cdk::mod_node * const node, int lvl) = 0;
  virtual void do_lt_node(cdk::lt_node * const node, int lvl) = 0;
  virtual void do_le_node(cdk::le_node * const node, int lvl) = 0;
  virtual void do_ge_node(cdk::ge_node * const node, int lvl) = 0;
  virtual void do_gt_node(cdk::gt_node * const node, int lvl) = 0;
  virtual void do_ne_node(cdk::ne_node * const node, int lvl) = 0;
  virtual void do_eq_node(cdk::eq_node * const node, int lvl) = 0;
  virtual void do_and_node(cdk::and_node * const node, int lvl) {
    // not used
  }
  virtual void do_or_node(cdk::or_node * const node, int lvl) {
    // not used
  }

public:
  virtual void do_identifier_node(cdk::identifier_node * const node, int lvl) = 0;
  virtual void do_rvalue_node(cdk::rvalue_node * const node, int lvl) = 0;
  virtual void do_assignment_node(cdk::assignment_node * const node, int lvl) = 0;

public:
  virtual void do_evaluation_node(xpl::evaluation_node * const node, int lvl) = 0;
  virtual void do_print_node(xpl::print_node * const node, int lvl) = 0;
  virtual void do_read_node(xpl::read_node * const node, int lvl) = 0;

public:
  virtual void do_while_node(xpl::while_node * const node, int lvl) = 0;
  virtual void do_if_node(xpl::if_node * const node, int lvl) = 0;
  virtual void do_if_else_node(xpl::if_else_node * const node, int lvl) = 0;
  virtual void do_if_elsif_else_node(xpl::if_elsif_else_node * const node, int lvl) = 0;

public: // XPL
  virtual void do_stop_node(xpl::stop_node * const node, int lvl) = 0;
  virtual void do_next_node(xpl::next_node * const node, int lvl) = 0;
  virtual void do_return_node(xpl::return_node * const node, int lvl) = 0;
  virtual void do_sweep_node(xpl::sweep_node * const node, int lvl) = 0;
  virtual void do_declaration_node(xpl::declaration_node * const node, int lvl) = 0;
  virtual void do_function_declaration_node(xpl::function_declaration_node * const node, int lvl) = 0;
  virtual void do_function_definition_node(xpl::function_definition_node * const node, int lvl) = 0;
  virtual void do_function_call_node(xpl::function_call_node * const node, int lvl) = 0;
  virtual void do_block_node(xpl::block_node * const node, int lvl) = 0;
  virtual void do_memory_allocation_node(xpl::memory_allocation_node * const node, int lvl) = 0;
  virtual void do_identity_node(xpl::identity_node * const node, int lvl) = 0;
  virtual void do_symmetry_node(xpl::symmetry_node * const node, int lvl) = 0;
  virtual void do_index_node(xpl::index_node * const node, int lvl) = 0;

};

#endif
