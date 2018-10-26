#include "UpdateWorldVisitor.h"
#include "GameObject.h"

namespace pompeii
{
  namespace engine
  {
    void UpdateWorldVisitor::visit( GameObject* go )
    {
      if( go->hasParent( ) )
      {
        std::cout << "Compute world model of " << "'" << go->getName( ) << "'"
          << /*current GameObject*/" from parent"
          << " " << go->getParent( )->getName( )
          << std::endl;
      }
      else
      {
        std::cout << "World model of " << "'" << go->getName( ) << "'"
          << /*current GameObject*/" as local matrix" << std::endl;
      }

      Visitor::visit( go );
    }
  }
}