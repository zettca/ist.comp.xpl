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
    std::string *_identifier;
    cdk::expression_node *_offset;

  public:
    inline index_node(int lineno, std::string *id, cdk::expression_node *offset) :
        cdk::lvalue_node(lineno), _identifier(id), _offset(offset) {
    }

  public:
    inline std::string *identifier() {
      return _identifier;
    }
    inline cdk::expression_node *offset() {
      return _offset;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

} // xpl

#endif
