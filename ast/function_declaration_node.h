#ifndef __XPL_FUNCTION_DECLARATION_NODE_H__
#define __XPL_FUNCTION_DECLARATION_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/basic_type.h>

namespace xpl {

  /**
   * Class for describing function declaration nodes.
   */
  class function_declaration_node: public cdk::basic_node {
    std::string *_qualifier;
    basic_type *_type;
    std::string *_identifier;
    cdk::sequence_node *_arguments;

  public:
    inline function_declaration_node(int lineno, std::string *qual, basic_type *type, std::string *id, cdk::sequence_node *args) :
        cdk::basic_node(lineno), _qualifier(qual), _type(type), _identifier(id), _arguments(args) {
    }
  public:
    inline std::string *qualifier() { return _qualifier; }
    inline basic_type *type() { return _type; }
    inline std::string *identifier() { return _identifier; }
    inline cdk::sequence_node *arguments() { return _arguments; }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_declaration_node(this, level);
    }

  };

} // xpl

#endif
