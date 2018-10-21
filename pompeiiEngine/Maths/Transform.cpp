#include "Transform.h"

namespace pompeii
{
  namespace engine
  {
    const Transform Transform::ZERO = Transform( );

    Transform::Transform( void )
      : _position( glm::vec3( 0.0f ) )
      , _rotation( glm::vec3( 0.0f ) )
      , _scale( glm::vec3(1.0f, 1.0f, 1.0f) )
    {
    }

    Transform::Transform( const Transform& other )
      :_position( other._position )
      , _rotation( other._rotation )
      , _scale( other._scale )
    {
    }

    Transform::Transform( const glm::vec3& position, 
      const glm::vec3& rotation, const glm::vec3& scaling )
      : _position( position )
      , _rotation( rotation )
      , _scale( scaling )
    {
    }

    Transform::Transform(const glm::vec3& position, 
      const glm::vec3& rotation, float scale )
      : _position( position )
      , _rotation( rotation ),
      , _scale( glm::vec3( scale ) )
    {
    }

    Matrix4 Transform::getWorldMatrix( void ) const
    {
      return Matrix4::TransformationMatrix(_position, _rotation, _scale);
    }

    Matrix4 Transform::getModelMatrix( void ) const
    {
      return Matrix4::TransformationMatrix(glm::vec3( 0.0f ), _rotation, glm::vec3( 0.0f ));
    }
    bool Transform::operator==( const Transform& other ) const
    {
      return _position == other._position && _rotation == other._rotation && _scale == other._scale;
    }

    bool Transform::operator!=( const Transform& other ) const
    {
      return !(*this == other);
    }

    std::ostream &operator<<(std::ostream &stream, 
      const Transform& transform)
    {
      std::stringstream res;
      res << "Transform(" << _position << ", "
        << _rotation << ", " << _scale << ")";
      stream << res.str( );
      return stream;
    }
  }
}