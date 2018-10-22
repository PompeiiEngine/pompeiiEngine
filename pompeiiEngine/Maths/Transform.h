#pragma once

#include "../includes.hpp"

namespace pompeii
{
  namespace engine
  {
    class Transform
    {
    public:
    //private:
      glm::vec3 _position;
      glm::vec3 _rotation;
      glm::vec3 _scale;
    public:
      static const Transform ZERO;
      
      Transform( void );
      Transform( const Transform& other );
      explicit Transform( const glm::vec3& position, 
        const glm::vec3& rotation = glm::vec3( 0.0f ),
        const glm::vec3& scaling = glm::vec3( 1.0f ) );
      Transform( const glm::vec3& position,
        const glm::vec3& rotation, float scale );

      glm::mat4 worldMatrix( void ) const;
      glm::mat4 modelMatrix( void ) const;

      bool operator==( const Transform& other ) const;
      bool operator!=( const Transform& other ) const;

      friend std::ostream &operator<<( std::ostream &stream,
        const Transform& transform );
    };
  }
}