#pragma once

#include "Components/Component.h"

#include "includes.hpp"

namespace pompeii
{
  namespace engine
  {
    enum class ShadowType
    {
      None,
      Hard,
      Soft
    };
    class Light:
      public Component
    {
      IMPLEMENT_COMPONENT( Light )
    protected:
      float _intensity;
      bool _enable;
      glm::vec3 _attenuation;
      glm::vec3 _color;
      glm::vec3 _specColor;
      ShadowType _shadowType;
    public:
      Light( const glm::vec3& color = glm::vec3( 1.0f ) );
      ~Light( void );
      void start( void ) override;

      void update( void ) override;

      float intensity( void ) const;
      glm::vec3 attenuation( void ) const;
      bool isEnable( void ) const;
      glm::vec3 color( void ) const;
      glm::vec3 specColor( void ) const;
      void intensity( float i );
      void attenuation( const glm::vec3& att );
      void enabled( const bool e );
      void color( const glm::vec3& c );
      void specColor( const glm::vec3& sc );
      ShadowType getShadowType( ) const;
      void setShadowType( ShadowType shadow );
    };
  }
}