#ifndef __XPL_FUNCTION_DEFINITION_NODE_H__
#define __XPL_FUNCTION_DEFINITION_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace xpl {

  /**
   * Class for describing function definition nodes.
   */
  class function_definition_node: public cdk::basic_node {
    std::string *_qualifier;
    std::string *_type;
    std::string *_identifier;
    cdk::sequence_node *_arguments;
    cdk::basic_node *_block;

  public:
    inline function_definition_node(int lineno, std::string *qual, std::string *type, std::string *id, cdk::sequence_node *args, cdk::basic_node *block) :
        cdk::basic_node(lineno), _qualifier(qual), _type(type), _identifier(id), _arguments(args), _block(block) {
    }
  public:
    inline std::string *qualifier() {
      return _qualifier;
    }
    inline std::string *type() {
      return _type;
    }
    inline std::string *identifier() {
      return _identifier;
    }
    inline cdk::sequence_node *arguments() {
      return _arguments;
    }
    inline cdk::basic_node *block() {
      return _block;
    }
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_definition_node(this, level);
    }

  };

} // xpl

#endif
