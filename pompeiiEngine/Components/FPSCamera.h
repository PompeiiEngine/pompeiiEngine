#pragma once

#include "../ICamera.h"

#include <glm/glm.hpp>

namespace pompeii
{
  namespace engine
  {
    class FPSCamera
      : public ICamera
    {
    public:
      void update( void ) override
      {
        // viewMatrix = glm::mat4
      }

    };
  }
}