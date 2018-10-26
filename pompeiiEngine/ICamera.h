#pragma once

#include "includes.hpp"

namespace pompeii
{
  namespace engine
  {
    class ICamera
    {
    public:
      virtual void update( void ) = 0;
      virtual glm::mat4 getViewMatrix( void ) const = 0;
      virtual glm::mat4 getProjectionMatrix( void ) const = 0;
      virtual glm::vec3 position( void ) const = 0;
    };

    class Camera
      : public ICamera
    {
    protected:
      glm::vec3 _position = glm::vec3( 2.0f, 2.0f, 2.0f );
    public:
      virtual glm::mat4 getViewMatrix( void ) const override
      {
        return glm::lookAt(
          _position,
          glm::vec3(0.0f, 0.0f, 0.0f),
          glm::vec3(0.0f, 0.0f, 1.0f)
        );
      }

      virtual glm::mat4 getProjectionMatrix( void ) const override
      {
        static uint32_t width = 500,
                        height = 500;

        glm::mat4 proj = glm::perspective(
          glm::radians(45.0f), width / (float)height, 0.1f, 10.0f);
        proj[1][1] *= -1;
        return proj;
      }

      virtual glm::vec3 position( void ) const override
      {
        return _position;
      }
    };
  }
}