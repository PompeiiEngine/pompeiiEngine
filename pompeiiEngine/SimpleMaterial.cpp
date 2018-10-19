#include "SimpleMaterial.h"
#include "GameObject.h"

#include "Mesh.h"

namespace pompeii
{
  namespace engine
  {
    SimpleMaterial::SimpleMaterial( const glm::vec3& diff )
      : _diffuseColor( diff )
    {

    }

    void SimpleMaterial::start( void )
    {
      auto mesh = this->getGameObject( )->getComponent< Mesh >( true );

      if( mesh == nullptr )
      {
        throw;
      }
    }

    void SimpleMaterial::update( void )
    {

    }
  }
}