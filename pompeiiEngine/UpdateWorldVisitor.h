#pragma once

#include "Visitor.h"

namespace pompeii
{
  namespace engine
  {
    class GameObject;
    class UpdateWorldVisitor
      : public Visitor
    {
    public:
      virtual void visit( GameObject* go ) override;
    };
  }
}