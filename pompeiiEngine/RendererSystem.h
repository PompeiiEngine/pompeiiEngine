#pragma once

#include "ISystem.h"

#include <pompeii/pompeii.h>

namespace pompeii
{
  namespace engine
  {
    class RendererSystem  
      : public ISystem
    {
    private:
      std::unique_ptr< PipelineCache > _pipelineCache;

      std::shared_ptr< CommandBuffer > _cmdBuffer;

    public:
      RendererSystem( void )
      {
        _pipelineCache = nullptr; // TODO
      }
      ~RendererSystem( void )
      {
        // Wait for device idle

        // Destroying PipelineCache
        _pipelineCache.reset( );
      }

      void update( void ) override
      {
        std::cout << "\tRendererSystem::update" << std::endl;




        // endRenderPass( );
      }

    protected:
      bool startRenderPass( const uint32_t& i )
      {



        return true;
      }

      void endRenderPass( const uint32_t& i )
      {

        _cmdBuffer->endRenderPass( );

        _cmdBuffer->end( );


      }

      void recreatePass( const uint32_t& i )
      {
        
      }

      void nextSubpass( void )
      {
        _cmdBuffer->nextSubpass( vk::SubpassContents::eInline );
      }
    };
  }
}