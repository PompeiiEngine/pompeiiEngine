#include "Visitor.h"
#include "GameObject.h"

namespace pompeii
{
  namespace engine
  {
    /*Visitor::Visitor &operator= ( const Visitor& v )
    {
      return *this;
    }*/
    Visitor::~Visitor( void )
    {

    }
    void Visitor::reset( void )
    {
    }
    void Visitor::traverse( GameObject* go )
    {
      reset( );
      go->accept( *this );
    }
    void Visitor::visit( GameObject* go )
    {
      for( auto& child: go->_children )
      {
        child->accept( *this );
      }
    }
  }
}