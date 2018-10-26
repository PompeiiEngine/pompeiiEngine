#pragma once

#include "Visitor.h"

#include "includes.hpp"

#include <map>
#include <vector>

#include "ICamera.h"

namespace pompeii
{
  namespace engine
  {
    struct Renderable
    {
      //MaterialPtr material;
      //Geometry* geometry;
      glm::mat4 modelTransform;
      float zDistance;
      Renderable( //MaterialPtr m, Geometry* g,
        const glm::mat4& mt, float zDist )
      {
        //this->material = m;
        //this->geometry = g;
        this->modelTransform = mt;
        this->zDistance = zDist;
      }
    };
    class BatchQueue
    {
    public:
      enum class RenderableType
      {
        OPAQUE,
        TRANSPARENT,
        SHADOW
      };
      BatchQueue( void )
      {
        _renderables[ RenderableType::OPAQUE ] = std::vector<Renderable>( );
        _renderables[ RenderableType::TRANSPARENT ] = std::vector<Renderable>( );
      }
      virtual ~BatchQueue( void )
      {
        reset( );
      }
      void reset( void )
      {
        setCamera( nullptr );
        //_lights.clear( );
        for ( auto &it : _renderables )
        {
          it.second.clear( );
        }
        _renderables.clear( );
      }
      void setCamera( ICamera* c )
      {
        if ( c!= nullptr )
        {
          _camera = c;
          _projectionMatrix = _camera->getProjectionMatrix( );
          _viewMatrix = _camera->getViewMatrix( );
        }
        else
        {
          _camera = nullptr;
          _projectionMatrix = glm::mat4( );
          _viewMatrix = glm::mat4( );
        }
      }
      ICamera* getCamera( void )
      {
        return _camera;
      }
      const glm::mat4& getProjectionMatrix( void ) const { return _projectionMatrix; }
      const glm::mat4& getViewMatrix( void ) const { return _viewMatrix; }
    protected:
      ICamera* _camera;
    protected:
      glm::mat4 _projectionMatrix;
      glm::mat4 _viewMatrix;
      std::map< RenderableType, std::vector< Renderable >> _renderables;
    };

    class GameObject;
    class ComputeBatchQueue
      : public Visitor
    {
    public:
      ComputeBatchQueue( ICamera* camera, 
        const std::shared_ptr< BatchQueue >& bq )
        : Visitor( )
        , _camera( camera )
        , _batch( bq )
      {
      }
      virtual void visit( GameObject* go ) override;
      virtual void traverse( GameObject* go ) override;
    protected:
      ICamera* _camera;
      std::shared_ptr< BatchQueue > _batch;
    };
  }
}