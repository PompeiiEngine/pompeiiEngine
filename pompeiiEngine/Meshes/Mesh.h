#pragma once

#include "../Components/Component.h"
#include "../includes.hpp"

#include <memory>
#include "Model.h"

namespace pompeii
{
  namespace engine
  {
    class Mesh:
      public Component
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