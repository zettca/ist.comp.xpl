#ifndef __XPL_MEMORY_ALLOCATION_NODE_H__
#define __XPL_MEMORY_ALLOCATION_NODE_H__

#include <cdk/ast/basic_node.h>
#include "declaration_node.h"

namespace xpl {

  /**
   * Class for describing memory allocation nodes.
   */
  class memory_allocation_node: public xpl::declaration_node {
    int _size;

  public:
    inline memory_allocation_node(int lineno, std::string *qual, cdk::lvalue_node *id, int size) :
        xpl::declaration_node(lineno, qual, id), _size(size) {
    }
    inline int size() {
      return _size;
    }
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_memory_allocation_node(this, level);
    }

  };

} // xpl

#endif
