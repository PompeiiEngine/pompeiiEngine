#pragma once

#include "../Engine.h"
#include "../DisplaySystem.h"

namespace pompeii
{
  namespace engine
  {
    class Model
    {
    private:
      std::shared_ptr< VertexBuffer > _vertexBuffer;
      std::shared_ptr< IndexBuffer > _indexBuffer;

      uint32_t _vertexCount;
      uint32_t _indexCount;

      glm::vec3 _minExtents;
      glm::vec3 _maxExtents;
      float _radius;
    public:
      Model( void ) 
      {
        
      }
      template<typename T>
      explicit Model( const std::vector< T>& verts,
        const std::vector< uint32_t>& indices = { } )
      {
        auto display = Engine::instance( )->getSystem< DisplaySystem >( );
        auto device = display->getDevice( );

        if( !verts.empty( ) )
        {
          _vertexCount = verts.size( );

          _vertexBuffer = device->createVertexBuffer( _vertexCount * sizeof( T ) );

          // Copies the vertex data to the buffer.
          _vertexBuffer->writeData( 0, sizeof( T ) * _vertexCount,
            verts.data( ) );
          std::cout << "VBO OK" << std::endl;
        }
        if( !indices.empty( ) )
        {
          _indexCount = indices.size( );

          _indexBuffer = device->createIndexBuffer( vk::IndexType::eUint32, 
            _indexCount * sizeof( uint32_t ) );

          // Copies the indices data to the buffer.
          _indexBuffer->writeData( 0, sizeof( uint32_t ) * _indexCount,
            indices.data( ) );
          std::cout << "IBO OK" << std::endl;
        }


        _minExtents = glm::vec3( ( +std::numeric_limits<float>::infinity( ) ) );
        _maxExtents = glm::vec3( ( -std::numeric_limits<float>::infinity( ) ) );

        // TODO
      }

      bool cmdRender( const std::shared_ptr< CommandBuffer >& cmd )
      {
        std::cout << "Bind VBO and EBO" << std::endl;

        if( _vertexBuffer != nullptr && _indexBuffer != nullptr )
        {
          cmd->bindVertexBuffer( 0, _vertexBuffer );
          cmd->bindIndexBuffer( _indexBuffer, 0, vk::IndexType::eUint32 );
        
          cmd->drawIndexed( _indexCount, 1, 0, 0, 1 );
        }
        else if ( _vertexBuffer != nullptr && _indexBuffer == nullptr )
        {
          cmd->bindVertexBuffer( 0, _vertexBuffer );
          cmd->draw( _vertexCount, 1, 0, 0 );
        }
        else
        {
          throw;
        }

        return true;
      }
    };
  }
}