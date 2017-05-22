#ifndef __CDK_IF_ELSIF_ELSE_NODE_H__
#define __CDK_IF_ELSIF_ELSE_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>

namespace xpl {

  /**
   * Class for describing if-elsif-else nodes.
   */
  class if_elsif_else_node: public cdk::basic_node {
    cdk::expression_node *_condition;
    cdk::basic_node *_thenblock;
    cdk::sequence_node *_elsifs;
    cdk::basic_node *_elseblock;

  public:
    inline if_elsif_else_node(int lineno, cdk::expression_node *condition, cdk::basic_node *thenblock, cdk::sequence_node *elsifs, cdk::basic_node *elseblock) :
      cdk::basic_node(lineno), _condition(condition), _thenblock(thenblock), _elsifs(elsifs), _elseblock(elseblock) {
    }

  public:
    inline cdk::expression_node *condition() { return _condition; }
    inline cdk::basic_node *thenblock() { return _thenblock; }
    inline cdk::sequence_node *elsifs() { return _elsifs; }
    inline cdk::basic_node *elseblock() { return _elseblock; }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_if_elsif_else_node(this, level);
    }
  };

} // xpl

#endif
