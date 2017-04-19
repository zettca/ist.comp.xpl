#ifndef __XPL_SWEEPUPNODE_H__
#define __XPL_SWEEPUPNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>

namespace xpl {

  /**
   * Class for describing sweep-up-cycle nodes.
   */
  class sweep_up_node: public cdk::basic_node {
    cdk::lvalue_node *_iterable;
    cdk::expression_node *_initial_value;
    cdk::expression_node *_final_value;
    cdk::expression_node *_increment;
    cdk::basic_node *_block;

  public:
    inline sweep_up_node(int lineno, cdk::lvalue_node *iter, cdk::expression_node *init, cdk::expression_node *final, cdk::expression_node *inc, cdk::basic_node *block) :
        cdk::basic_node(lineno), _iterable(iter), _initial_value(init), _final_value(final), _increment(inc), _block(block) {
    }

  public:
    inline cdk::lvalue_node *iterable() { return _iterable; }
    inline cdk::expression_node *initial_value() { return _initial_value; }
    inline cdk::expression_node *final_value() { return _final_value; }
    inline cdk::expression_node *increment() { return _increment; }
    inline cdk::basic_node *block() { return _block; }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_sweep_up_node(this, level);
    }

  };

} // xpl

#endif
