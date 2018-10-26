#pragma once

#include "ISystem.h"
#include "IScene.h"

namespace pompeii
{
  namespace engine
  {
    class SceneSystem
      : public ISystem
    {
    private:
      std::unique_ptr< IScene > _scene;
    public:
      IScene* getScene( void ) const { return _scene.get( ); }
      void setScene( IScene* scene ) { _scene.reset( scene ); }
      void update( void ) override
      {
        std::cout << "\tSceneSystem::update" << std::endl;
        if( _scene == nullptr )
        {
          return;
        }
        if( _scene->isStarted( ) )
        {
          _scene->start( );
        }

        _scene->update( );
      }
    };
  }
}