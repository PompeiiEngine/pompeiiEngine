#pragma once

#include "IComponent.h"
#include <memory>

namespace pompeii
{
  namespace engine
  {
    class MaterialPipeline
    //  : public VulkanResource
    {
    private:
      // pipeline, ....
    };
    class IMaterial:
      public IComponent
    {
    public:
      virtual void start( void ) override = 0;
      virtual void update( void ) override = 0;
      virtual std::shared_ptr<MaterialPipeline> getMaterialPipeline( void ) const = 0;
    };
  }
}