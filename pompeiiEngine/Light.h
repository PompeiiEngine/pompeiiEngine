#pragma once

#include "IComponent.h"

#include <glm/glm.hpp>

#include "includes.hpp"

namespace pompeii
{
  namespace engine
  {
    class Light:
      public IComponent
    {
      IMPLEMENT_COMPONENT( Light )
    protected:
      glm::vec3 _color;
    public:
      Light( const glm::vec3& color = glm::vec3( 1.0f ) )
        : _color( color )
      {
        std::cout << "Light created" << std::endl;
      }
      ~Light( void )
      {
        std::cout << "Light destroyed" << std::endl;
      }
      void start( void ) override
      {

      }

      void update( void ) override
      {

      }

      glm::vec3 getColor( void ) const { return _color; }

      void setColor( const glm::vec3& color ) { _color = color; }
    };
  }
}