#ifndef __POMPEII_ENGINE_TRANSFORM__
#define __POMPEII_ENGINE_TRANSFORM__

#include "../includes.hpp"

/*namespace pompeii
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
}*/

namespace pompeii
{
  namespace engine
  {
    enum RotationOrder
    {
      OrderXYZ,
      OrderXZY,
      OrderYZX,
      OrderYXZ,
      OrderZXY,
      OrderZYX
    };
    enum Space
    {
      World,
      Self
    };
    class Transform
    {
    protected:
      RotationOrder rotationOrder = RotationOrder::OrderXYZ;
      glm::vec3 _eulerAngles;
      glm::vec3 _position;
      glm::vec3 _scale;
      glm::quat _rotate;
    public:
      /*Transform& fromMatrix( const glm::mat4& m )
      {
        Matrox4f
      }
      Transform( const glm::mat4& m )
      {
        *this = fromMatrix( m );
      }*/
      Transform( void )
      {
        _position = glm::vec3( 0.0f );
        _scale = glm::vec3( 1.0f );
        _isIdentity = true;
      }
      const glm::quat& getRotation( void ) const
      {
        return _rotate;
      }
      glm::quat& rotate( void )
      {
        _isIdentity = false;
        return _rotate;
      }
      void setRotation( const glm::quat& q )
      {
        _rotate = q;
        _isIdentity = false;
      }
      // TODO void setRotation( const glm::vec3& axis, float angle )
      //{
      //  _rotate.fromAxisAngle( axis, angle );
      //  _isIdentity = false;
      //}

      glm::mat4 computeModel( void ) const
      {
        glm::mat4 result;
        if ( !_isIdentity )
        {
          glm::vec3 position = this->getPosition( );
          glm::quat quaternion = this->getRotation( );
          glm::vec3 scale = this->getScale( );

          /// ROTATION
          float x = quaternion.x,
            y = quaternion.y,
            z = quaternion.z,
            w = quaternion.w;
          float x2 = x + x, y2 = y + y, z2 = z + z;
          float xx = x * x2, xy = x * y2, xz = x * z2;
          float yy = y * y2, yz = y * z2, zz = z * z2;
          float wx = w * x2, wy = w * y2, wz = w * z2;

          result[ 0 ][ 0 ] = 1.0f - ( yy + zz );
          result[ 1 ][ 0 ] = xy - wz;
          result[ 2 ][ 0 ] = xz + wy;

          result[ 0 ][ 1 ] = xy + wz;
          result[ 1 ][ 1 ] = 1.0f - ( xx + zz );
          result[ 2 ][ 1 ] = yz - wx;

          result[ 0 ][ 2 ] = xz - wy;
          result[ 1 ][ 2 ] = yz + wx;
          result[ 2 ][ 2 ] = 1.0 - ( xx + yy );

          // last column
          result[ 0 ][ 3 ] = 0.0f;
          result[ 1 ][ 3 ] = 0.0f;
          result[ 2 ][ 3 ] = 0.0f;

          // bottom row
          result[ 3 ][ 0 ] = 0.0f;
          result[ 3 ][ 1 ] = 0.0f;
          result[ 3 ][ 2 ] = 0.0f;
          result[ 3 ][ 3 ] = 1.0f;

          /// SCALE
          x = scale.x;
          y = scale.y;
          z = scale.z;
          result[ 0 ][ 0 ] *= x; result[ 1 ][ 0 ] *= y; result[ 2 ][ 0 ] *= z;
          result[ 0 ][ 1 ] *= x; result[ 1 ][ 1 ] *= y; result[ 2 ][ 1 ] *= z;
          result[ 0 ][ 2 ] *= x; result[ 1 ][ 2 ] *= y; result[ 2 ][ 2 ] *= z;
          result[ 0 ][ 3 ] *= x; result[ 1 ][ 3 ] *= y; result[ 2 ][ 3 ] *= z;

          /// POSITION
          result[ 3 ][ 0 ] = position.x;
          result[ 3 ][ 1 ] = position.y;
          result[ 3 ][ 2 ] = position.z;
        }
        return result;
      }
      glm::vec3 getRight( void ) const
      {
        //return this->getRotation( ) * glm::vec3::RIGHT;

        glm::vec3 input = glm::vec3( 1.0f, 0.0f, 0.0f );
        glm::vec3 aux = glm::vec3( 0.0f );
        for( unsigned int i = 0; i < 3; ++i )
        {
          aux[ i ] = _scale[ i ] * input[ i ];
        }
        return _rotate * aux;
      }
      /*void setRight( const glm::vec3& v )
      {
        setRotation( glm::quat::fromRotation( glm::vec3::right, v ) );
      }*/
      glm::vec3 getUp( void ) const
      {
        glm::vec3 input = glm::vec3( 0.0f, 1.0f, 0.0f );
        glm::vec3 aux = glm::vec3( 0.0f );
        for( unsigned int i = 0; i < 3; ++i )
        {
          aux[ i ] = _scale[ i ] * input[ i ];
        }
        return _rotate * aux;
        //return this->getRotation( ) * glm::vec3::UP;
      }
      /*void setUp( const glm::vec3& v )
      {
        setRotation( glm::quat::fromRotation( glm::vec3::UP, v ) );
      }*/

      glm::vec3 computeDirection( void ) const
      {
        glm::vec3 input(0.0f, 0.0f, -1.0f);
        glm::vec3 aux = glm::vec3( 0.0f );
        for( unsigned int i = 0; i < 3; ++i )
        {
          aux[ i ] = _scale[ i ] * input[ i ];
        }
        return _rotate * aux;
      }
      glm::vec3 getForward( void ) const
      {
        return this->getRotation( ) * glm::vec3( 0.0f, 0.0f, 1.0f );
      }
      /*void setForward( const glm::vec3& v )
      {
        setRotation( glm::quat::fromRotation( glm::vec3::FORWARD, v ) );
      }

      glm::vec3 transformDirection( const glm::vec3& direction )
      {
        glm::vec3 result;
        return result;
      }
      void translate( const glm::vec3& translation, Space relativeTo = Space::Self )
      {
        if ( relativeTo == Space::World )
        {
          this->_position += translation;
        }
        else
        {
          this->_position += this->transformDirection( translation );
        }
      }
      void translate( float x, float y, float z, Space relativeTo = Space::Self )
      {
        this->translate( x, y, z, relativeTo );
      }
      void translate( const glm::vec3& translation, const Transform* relativeTo )
      {
        if ( relativeTo != nullptr )
        {
          this->_position += relativeTo->transformDirection( translation );
        }
        else
        {
          this->_position += translation;
        }
      }
      void translate( float x, float y, float z, const Transform* relativeTo )
      {
        this->translate( glm::vec3( x, y, z ), relativeTo );
      }
      void rotate( const glm::vec3& eulerAngles, Space relativeTo = Space::Self )
      {
        glm::quat rhs = glm::quat::fromEuler(
          eulerAngles.x( ), eulerAngles.y( ), eulerAngles.z( ) );
        if ( relativeTo == Space::Self )
        {
          this->_localRotation *= rhs;
        }
        else
        {
          this->_rotation *= glm::quat::inverse( this->getRotation( ) ) * rhs * this->getRotation( );
        }
      }
      void rotate( float xAngle, float yAngle, float zAngle )
      {
        this->rotate( glm::vec3( xAngle, yAngle, zAngle ), Space::Self );
      }*/
      bool isIdentity( void ) const {
        return _isIdentity;
      }
      Transform& operator= ( const Transform& t2 )
      {
        _position = t2._position;
        _rotate = t2._rotate;
        _scale = t2._scale;
        _isIdentity = t2._isIdentity;
        return *this;
      }
      void translate( float x, float y, float z )
      {
        translate( glm::vec3( x, y, z ) );
      }
      void translate( const glm::vec3& pos )
      {
        setPosition( getPosition( ) + pos );
      }
      const glm::vec3& getPosition( void ) const
      {
        return _position;
      }
      glm::vec3& position( )
      {
        return _position;
      }
      void setPosition( const glm::vec3& pos )
      {
        _position = pos;
        _isIdentity = false;
      }
      void setPosition( float x, float y, float z )
      {
        _position[ 0 ] = x;
        _position[ 1 ] = y;
        _position[ 2 ] = z;
        _isIdentity = false;
      }
      void scale( float x, float y, float z )
      {
        scale( glm::vec3( x, y, z ) );
      }
      void scale( const glm::vec3& sc )
      {
        setScale( getScale( ) + sc );
      }
      const glm::vec3& getScale( void ) const
      {
        return _scale;
      }
      void setScale( float v )
      {
        setScale( v, v, v );
      }
      void setScale( const glm::vec3& sc )
      {
        _scale = sc;
        _isIdentity = false;
      }
      void setScale( float x, float y, float z )
      {
        _scale[ 0 ] = x;
        _scale[ 1 ] = y;
        _scale[ 2 ] = z;
        _isIdentity = false;
      }
      void makeIdentity( void )
      {
        _position = glm::vec3( 0.0f );
        _rotate = glm::quat( );
        _scale = glm::vec3( 1.0f );
        _isIdentity = true;
      }
      void computeFrom( const Transform& t1, const Transform& t2 )
      {
        if ( t1.isIdentity( ) )
        {
          *this = t2;
        }
        else if ( t2.isIdentity( ) )
        {
          *this = t1;
        }
        else
        {
          glm::vec3 aux = glm::vec3( 0.0f );
          for( unsigned int i = 0; i < 3; ++i )
          {
            aux[ i ] = t1._scale[ i ] * t2._position[ i ];
          }

          _position = t1._position + t1._rotate * aux;
          _rotate = t1._rotate * t2._rotate;
          for( unsigned int i = 0; i < 3; ++i )
          {
            _scale[ i ] = t1._scale[ i ] * t2._scale[ i ];
          }
          _isIdentity = false;
        }
      }
      // TODO: void lookAt( const glm::vec3& target, const glm::vec3& up = glm::vec3( 0.0f, 1.0f, 0.0f ) )
      // TODO: {
      // TODO:   glm::vec3 dir = target - getPosition( );
      // TODO:   dir = glm::normalize( dir );
      // TODO:   _rotate = glm::quatLookAt( dir, up );
      // TODO: }

      // TODO: Transform& fromMatrix( const glm::mat4& m )
      // TODO: {
      // TODO:   glm::mat3 viewRotation( m );
      // TODO: 
      // TODO:   rotate( ).fromRotationMatrix( viewRotation );
      // TODO:   setPosition( m[ 3 ][ 0 ], m[ 3 ][ 1 ], m[ 3 ][ 2 ] );
      // TODO:   setScale( glm::vec3( 1.0f ) );
      // TODO: 
      // TODO:   return *this;
      // TODO: }

    protected:
      // OPTIMIZATION: If identity, discard multiplication on global matrices generation!!
      bool _isIdentity;
    };
  }
}

#endif /* __POMPEII_ENGINE_TRANSFORM__ */
