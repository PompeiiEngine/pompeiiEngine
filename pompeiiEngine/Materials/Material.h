#pragma once

#include "../Components/Component.h"
#include "MaterialPipeline.h"
#include <memory>

namespace pompeii
{
  namespace engine
  {
    class Material:
      public Component
    {
    public:
      virtual void start( void ) override = 0;
      virtual void update( void ) override = 0;
      virtual std::shared_ptr<MaterialPipeline> getMaterialPipeline( void ) const = 0;
    };
  }
}