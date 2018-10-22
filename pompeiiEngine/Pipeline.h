#pragma once

#include <pompeii/pompeii.h>

namespace pompeii
{
  namespace engine
  {
    class IPipeline
    {
    public:
      /*void bind( const std::shared_ptr< CommandBuffer> & cmd ) const
      {
        cmd->bindPipeline( getBindPoint( ), getPipeline( ) );
      }*/

      std::shared_ptr< Pipeline > pipeline( void ) const
      {
        return _pipeline;
      }
      std::shared_ptr< PipelineLayout > pipelineLayout( void ) const
      {
        return _pipelineLayout;
      }
      virtual vk::PipelineBindPoint pipelineBindPoint( void ) const = 0;

    private:
      std::shared_ptr< Pipeline > _pipeline;
      std::shared_ptr< PipelineLayout > _pipelineLayout;
    };
    class GraphicPipeline
    {
    public:
      virtual vk::PipelineBindPoint pipelineBindPoint( void ) const
      {
        return vk::PipelineBindPoint::eGraphics;
      }

    private:
      void createShaderProgram( void );
    };

    class ComputePipeline
      : public IPipeline
    {
    public:
      explicit ComputePipeline( void )
        : IPipeline( )
      {

      }
      virtual vk::PipelineBindPoint pipelineBindPoint( void ) const
      {
        return vk::PipelineBindPoint::eCompute;
      }
    private:
      void createPipelineCompute( void )
      {
        
      }
    };
  }
}