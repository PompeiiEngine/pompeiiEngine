#pragma once

#include "ICamera.h"

namespace pompeii
{
  namespace engine
  {
    class IScene
    {
    private:
      std::unique_ptr< ICamera > _camera;
    public:
      IScene( ICamera* camera )
        : _camera ( camera )
      {
      }
      virtual void start( void ) = 0;

      virtual void update( void ) = 0;
    
      ICamera* getCamera( void ) const { return _camera.get( ); }
      void setCamera( ICamera* camera ) { _camera.reset( camera ); }
    };
  }
}