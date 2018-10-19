#include "Light.h"

namespace pompeii
{
  namespace engine
  {
    Light::Light( const glm::vec3& color )
      : _intensity( 1.0f )
      , _enable( true )
      , _attenuation( glm::vec3(
        1.0f,        // Constant
        0.014f,      // Linear
        0.0007f      // Quadratic
      ) )
      , _color( color )
      , _specColor( glm::vec3( 1.0f, 1.0f, 1.0f ) )
      , _shadowType( ShadowType::None )
    {
      std::cout << "Light created" << std::endl;
    }

    Light::~Light( void )
    {
      std::cout << "Light destroyed" << std::endl;
    }

    void Light::start( void )
    {

    }

    void Light::update( void )
    {

    }
    inline float Light::intensity( void ) const
    {
      return this->_intensity;
    }
    inline glm::vec3 Light::attenuation( void ) const
    {
      return this->_attenuation;
    }
    inline bool Light::isEnable( void ) const
    {
      return this->_enable;
    }
    glm::vec3 Light::color( void ) const
    {
      return this->_color;
    }
    glm::vec3 Light::specColor( void ) const
    {
      return this->_specColor;
    }
    void Light::intensity( float i )
    {
      this->_intensity = i;
    }
    void Light::attenuation( const glm::vec3& att )
    {
      this->_attenuation = att;
    }
    void Light::enabled( const bool e )
    {
      this->_enable = e;
    }
    void Light::color( const glm::vec3& c )
    {
      this->_color = c;
    }
    void Light::specColor( const glm::vec3& sc )
    {
      this->_specColor = sc;
    }
    ShadowType Light::getShadowType( ) const
    {
      return _shadowType;
    }
    void Light::setShadowType( ShadowType shadow )
    {
      _shadowType = shadow;
    }
  }
}