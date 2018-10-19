#pragma once

#include "IComponent.h"
#include "includes.hpp"

#include <memory>

namespace pompeii
{
  namespace engine
  {
    class Model
    //  : public VulkanResource
    {
    public:
      Model( void );

      bool cmdRender( /* commandBuffer */ );

    };
    class Mesh:
      public IComponent
    {
      IMPLEMENT_COMPONENT( Mesh )
    private:
      std::shared_ptr< Model > _model;
    public:
      explicit Mesh( const std::shared_ptr< Model >& model = nullptr );
      Mesh( const std::string& filename );
      void start( void ) override;
      void update( void ) override;

      virtual std::shared_ptr< Model > getModel( void ) const { return _model; }
    };
  }
}