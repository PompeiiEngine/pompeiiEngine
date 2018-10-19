#pragma once

#include <memory>

#include "../includes.hpp"

namespace pompeii
{
  namespace engine
  {
    class MaterialPipeline
    //  : public VulkanResource
    {
    private:
      std::unique_ptr< Pipeline > _pipeline;
      // pipeline, ....
    public:
      bool bindPipeline( const std::shared_ptr< CommandBuffer >& )
      {
      }
      Pipeline* pipeline( void ) { return _pipeline.get( ); }
    };
  }
}