#ifndef __XPL_SYMMETRY_NODE_H__
#define __XPL_SYMMETRY_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/unary_expression_node.h>

namespace xpl {

  /**
   * Class for describing symmetry nodes.
   */
  class symmetry_node: public cdk::unary_expression_node {

  public:
    inline symmetry_node(int lineno, cdk::expression_node *arg) :
        cdk::unary_expression_node(lineno, arg) {
    }
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_symmetry_node(this, level);
    }

  };

} // xpl

#endif
