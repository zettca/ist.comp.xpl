#include <string>
#include "targets/size_checker.h"
#include "ast/all.h"  // automatically generated

void xpl::size_checker::do_block_node(cdk::block_node * const node, int lvl) {
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

void xpl::size_checker::do_function_definition_node(cdk::function_definition_node * const node, int lvl) {
  node->block()->accept(this, lvl);
}

void xpl::size_checker::do_declaration_node(cdk::declaration_node * const node, int lvl) {
  _size += node->type()->name() == basic_type::TYPE_INT ? 4 : 8;
}
