#include "DumpVisitor.h"
#include "GameObject.h"

#include <iostream>
#include <sstream>

namespace pompeii
{
  namespace engine
  {
    void DumpVisitor::traverse( GameObject* go )
    {
      std::cout << "TRAVERSING ... " << std::endl;

      Visitor::traverse( go );

      std::cout << std::endl << " ... DONE" << std::endl;
    }
    void DumpVisitor::visit( GameObject* go )
    {
      _dumpNode( go, "GameObject" );

      ++_auxLevel;
      Visitor::visit( go );
      --_auxLevel;
    }
    void DumpVisitor::_dumpNode( GameObject* go, const std::string& type )
    {
      std::cout << "\n";
      std::stringstream tab;
      for ( unsigned int i = 0; i < _auxLevel; ++i )
      {
        tab << "  ";
      }

      std::cout << tab.str() << "[" + type + "] "
        << go->getName() << " (" << go << ")";
    }
  }
}