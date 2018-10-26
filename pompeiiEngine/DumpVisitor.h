#pragma once

#include "Visitor.h"

#include <string>

namespace pompeii
{
  namespace engine
  {
    class GameObject;
    class DumpVisitor
      : public Visitor
    {
    public:
      virtual void traverse( GameObject* go ) override;
      virtual void visit( GameObject* go ) override;
    private:
      void _dumpNode( GameObject* go, const std::string& type );
      unsigned int _auxLevel = 0;
    };
  }
}