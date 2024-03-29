#ifndef __XPL_FUNCTION_CALL_NODE_H__
#define __XPL_FUNCTION_CALL_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>

namespace xpl {

  /**
   * Class for describing function call nodes.
   */
  class function_call_node: public cdk::expression_node {
    std::string *_identifier;
    cdk::sequence_node *_arguments;

  public:
    inline function_call_node(int lineno, std::string *id, cdk::sequence_node *args) :
        cdk::expression_node(lineno), _identifier(id), _arguments(args) {
    }
  public:
    inline std::string *identifier() {
      return _identifier;
    }
    inline cdk::sequence_node *arguments() {
      return _arguments;
    }
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_call_node(this, level);
    }

  };

} // xpl

#endif
