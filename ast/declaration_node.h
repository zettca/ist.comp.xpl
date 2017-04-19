#ifndef __XPL_DECLARATION_NODE_H__
#define __XPL_DECLARATION_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/lvalue_node.h>

namespace xpl {

  /**
   * Class for describing variable declaration nodes.
   */
  class declaration_node: public cdk::basic_node {
    std::string *_qualifier;
    cdk::lvalue_node *_identifier;

  public:
    inline declaration_node(int lineno, std::string *qual, cdk::lvalue_node *id) :
        cdk::basic_node(lineno), _qualifier(qual), _identifier(id) {
    }

  public:
    inline std::string *qualifier() {
      return _qualifier;
    }
    inline cdk::lvalue_node *identifier() {
      return _identifier;
    }
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_declaration_node(this, level);
    }

  };

} // xpl

#endif
