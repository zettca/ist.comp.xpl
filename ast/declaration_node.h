#ifndef __XPL_DECLARATION_NODE_H__
#define __XPL_DECLARATION_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/lvalue_node.h>
#include <cdk/basic_type.h>

namespace xpl {

  /**
   * Class for describing variable declaration nodes.
   */
  class declaration_node: public cdk::basic_node {
    std::string *_qualifier;
    basic_type *_type;
    std::string *_identifier;

  public:
    inline declaration_node(int lineno, std::string *qual, basic_type *type, std::string *id) :
        cdk::basic_node(lineno), _qualifier(qual), _type(type), _identifier(id) {
    }

  public:
    inline std::string *qualifier() {
      return _qualifier;
    }
    inline basic_type *type() {
      return _type;
    }
    inline std::string *identifier() {
      return _identifier;
    }
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_declaration_node(this, level);
    }

  };

} // xpl

#endif
