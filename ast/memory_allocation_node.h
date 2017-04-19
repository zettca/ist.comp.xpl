#ifndef __XPL_MEMORY_ALLOCATION_NODE_H__
#define __XPL_MEMORY_ALLOCATION_NODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace xpl {

  /**
   * Class for describing memory allocation nodes.
   */
  class memory_allocation_node: public cdk::unary_expression_node {

  public:
    inline memory_allocation_node(int lineno, cdk::expression_node *size) :
        cdk::unary_expression_node(lineno, size) {
    }
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_memory_allocation_node(this, level);
    }

  };

} // xpl

#endif
