// $Id: index_node.h,v 1.1 2017/02/17 16:02:31 david Exp $ -*- c++ -*-
#ifndef __XPL_INDEX_NODE_H__
#define __XPL_INDEX_NODE_H__

#include <cdk/ast/lvalue_node.h>
#include <cdk/ast/expression_node.h>

namespace xpl {

  /**
   * Class for describing index nodes.
   */
  class index_node: public cdk::lvalue_node {
    cdk::lvalue_node *_variable;
    cdk::expression_node *_offset;

  public:
    inline index_node(int lineno, cdk::lvalue_node *var, cdk::expression_node *offset) :
        cdk::lvalue_node(lineno), _variable(var), _offset(offset) {
    }

  public:
    inline cdk::expression_node *variable() {
      return _variable;
    }
    inline cdk::basic_node *offset() {
      return _offset;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

} // xpl

#endif