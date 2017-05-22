#include <string>
#include "targets/size_checker.h"
#include "ast/all.h"  // automatically generated

void xpl::size_checker::do_block_node(xpl::block_node * const node, int lvl) {
  if (node->declarations() != nullptr)
    node->declarations()->accept(this, lvl);
  if (node->instructions() != nullptr)
    node->instructions()->accept(this, lvl);
}

void xpl::size_checker::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

void xpl::size_checker::do_function_definition_node(xpl::function_definition_node * const node, int lvl) {
  _size += node->type()->name() == basic_type::TYPE_INT ? 4 : 8;
  node->block()->accept(this, lvl);
}

void xpl::size_checker::do_declaration_node(xpl::declaration_node * const node, int lvl) {
  _size += node->type()->name() == basic_type::TYPE_INT ? 4 : 8;
}

void xpl::size_checker::do_integer_node(cdk::integer_node * const node, int lvl) {}
void xpl::size_checker::do_string_node(cdk::string_node * const node, int lvl) {}
void xpl::size_checker::do_neg_node(cdk::neg_node * const node, int lvl) {}
void xpl::size_checker::do_add_node(cdk::add_node * const node, int lvl) {}
void xpl::size_checker::do_sub_node(cdk::sub_node * const node, int lvl) {}
void xpl::size_checker::do_mul_node(cdk::mul_node * const node, int lvl) {}
void xpl::size_checker::do_div_node(cdk::div_node * const node, int lvl) {}
void xpl::size_checker::do_mod_node(cdk::mod_node * const node, int lvl) {}
void xpl::size_checker::do_lt_node(cdk::lt_node * const node, int lvl) {}
void xpl::size_checker::do_le_node(cdk::le_node * const node, int lvl) {}
void xpl::size_checker::do_ge_node(cdk::ge_node * const node, int lvl) {}
void xpl::size_checker::do_gt_node(cdk::gt_node * const node, int lvl) {}
void xpl::size_checker::do_ne_node(cdk::ne_node * const node, int lvl) {}
void xpl::size_checker::do_eq_node(cdk::eq_node * const node, int lvl) {}

void xpl::size_checker::do_identifier_node(cdk::identifier_node * const node, int lvl) {}
void xpl::size_checker::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {}
void xpl::size_checker::do_assignment_node(cdk::assignment_node * const node, int lvl) {}
void xpl::size_checker::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {}
void xpl::size_checker::do_print_node(xpl::print_node * const node, int lvl) {}
void xpl::size_checker::do_read_node(xpl::read_node * const node, int lvl) {}
void xpl::size_checker::do_while_node(xpl::while_node * const node, int lvl) {}
void xpl::size_checker::do_if_node(xpl::if_node * const node, int lvl) {}
void xpl::size_checker::do_if_else_node(xpl::if_else_node * const node, int lvl) {}
void xpl::size_checker::do_if_elsif_else_node(xpl::if_elsif_else_node * const node, int lvl) {}

void xpl::size_checker::do_stop_node(xpl::stop_node * const node, int lvl) {}
void xpl::size_checker::do_next_node(xpl::next_node * const node, int lvl) {}
void xpl::size_checker::do_return_node(xpl::return_node * const node, int lvl) {}
void xpl::size_checker::do_sweep_node(xpl::sweep_node * const node, int lvl) {}
void xpl::size_checker::do_function_declaration_node(xpl::function_declaration_node * const node, int lvl) {}
void xpl::size_checker::do_function_call_node(xpl::function_call_node * const node, int lvl) {}
void xpl::size_checker::do_memory_allocation_node(xpl::memory_allocation_node * const node, int lvl) {}
void xpl::size_checker::do_identity_node(xpl::identity_node * const node, int lvl) {}
void xpl::size_checker::do_symmetry_node(xpl::identity_node * const node, int lvl) {}
void xpl::size_checker::do_index_node(xpl::index_node * const node, int lvl) {}
