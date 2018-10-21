#pragma once

#include <glm/glm.hpp>

namespace pompeii
{
  namespace engine
  {
    class ICamera
    {
    public:
      virtual void update( void ) = 0;
      virtual glm::mat4 viewMatrix( void ) const = 0;
      virtual glm::mat4 projMatrix( void ) const = 0;
      virtual glm::vec3 position( void ) const = 0;
    };
  }
}