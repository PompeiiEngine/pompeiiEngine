#pragma once

#include "IMaterial.h"
#include <glm/glm.hpp>

namespace pompeii
{
  namespace engine
  {
    class SimpleMaterial
      : public IMaterial
    {
      IMPLEMENT_COMPONENT( SimpleMaterial )
    private:
      std::shared_ptr<MaterialPipeline> _material;
      glm::vec3 _diffuseColor;
    public:
      explicit SimpleMaterial( const glm::vec3& diff = glm::vec3( 1.0f ) );

      void start( void ) override;
      void update( void ) override;
      std::shared_ptr<MaterialPipeline> getMaterialPipeline( void ) const override
      {
        return _material;
      }
    };
  }
}