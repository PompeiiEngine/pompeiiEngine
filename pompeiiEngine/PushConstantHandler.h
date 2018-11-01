#pragma once

#include <pompeii/pompeii.h>
#include "ShaderProgram.h"

namespace pompeii
{
  namespace engine
  {
    class PushConstantHandler
    {
    public:
      UniformBlock* uBlock;
      void* data;

      explicit PushConstantHandler( void )
        : uBlock( nullptr )
        , data( nullptr )
      {
      }

      explicit PushConstantHandler(UniformBlock* uniformBlock)
        : uBlock( uniformBlock )
        , data(malloc(static_cast< uint32_t >(uniformBlock->size)))
      {
      }

      ~PushConstantHandler( void )
      {
        free( data );
      }

      template< typename T >
      T getParameter( const std::string& uboName )
      {
        if(uBlock == nullptr) throw;
        auto u = uBlock->getUniform( uboName );
        if(u == nullptr) throw;

        uint32_t size_ = std::min( 
          static_cast< uint32_t > ( sizeof( T ) ),
          static_cast< uint32_t > ( u->size )
        );
        uint32_t offset = u->offset;

        T v;

        memcpy( &v, data + offset, size_ );

        return v;
      }

      template< typename T >
      void setParameter( const T& obj, uint32_t offset, uint32_t size )
      {
        memcpy( ( char* ) data + offset, &obj, size );
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
          uBlock = ub;
          data = malloc( static_cast< uint32_t > ( ub->size ) );
          return false;
        }
        return true;
      }

      bool bind( const std::shared_ptr< CommandBuffer >& /*cmd, 
        const std::shared_ptr< Pipeline >& pipeline*/ )
      {
        //cmd->pushConstant( pipeline->layout, uBlock->stageFlags, data );
      }
    };
  }
}