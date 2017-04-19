// $Id: read_node.h,v 1.1 2017/02/17 16:02:31 david Exp $ -*- c++ -*-
#ifndef __XPL_READNODE_H__
#define __XPL_READNODE_H__

#include <cdk/ast/lvalue_node.h>

namespace xpl {

  /**
   * Class for describing read nodes.
   */
  class read_node: public cdk::basic_node {
    cdk::lvalue_node *_argument;

  public:
    inline read_node(int lineno, cdk::lvalue_node *argument) :
        cdk::basic_node(lineno), _argument(argument) {
    }

  public:
    inline cdk::lvalue_node *argument() {
      return _argument;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_read_node(this, level);
    }

  };

} // xpl

#endif
