#pragma once

#include <pompeii/pompeii.h>
#include "ShaderProgram.h"

namespace pompeii
{
  namespace engine
  {
    class StorageBufferHandler
    {
    public:
      UniformBlock* uBlock;
      std::shared_ptr< StorageBuffer > storage;
      void* data;
      bool changed = false;

      explicit StorageBufferHandler( void )
        : uBlock( nullptr )
        , data( nullptr )
        , changed( false )
      {
      }

      explicit StorageBufferHandler(UniformBlock* uniformBlock)
        : uBlock( uniformBlock )
        , data(malloc(static_cast< uint32_t >(uniformBlock->size)))
        , changed( true )
      {

      }

      ~StorageBufferHandler( void )
      {
        free( data );
      }

      template< typename T >
      void setParameter( const T& obj, uint32_t offset, uint32_t size )
      {
        //std::copy( ( char* ) data, ( char* ) data + offset, &obj );

        memcpy( ( char* ) data + offset, &obj, size );
        changed = true;
      }
      template< typename T >
      void setParameter( const std::string& uboName, const T& obj )
      {
        if(uBlock == nullptr) throw;
        auto u = uBlock->getUniform( uboName );
        if(u == nullptr) throw;

        uint32_t size_ = std::min( 
          static_cast< uint32_t > ( sizeof( obj ) ),
          static_cast< uint32_t > ( u->size )
        );
        setParameter( obj, static_cast<uint32_t>( u->offset ), size_ );
      }

      bool update( UniformBlock* ub = nullptr )
      {
        if( ub != uBlock )
        {
          // reset
          uBlock = ub;
          // storage = new ...
          data = malloc( static_cast< uint32_t > ( ub->size ) );
          changed = false;
          return false;
        }
        if( changed )
        {
          // ubo->update( data );
          changed = false;
        }
        return true;
      }
    };
  }
}