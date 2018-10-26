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
      bool _started;
    public:
      IScene( ICamera* camera )
        : _camera ( camera )
        , _started ( false )
      {
      }
      virtual void start( void ) = 0;

      virtual void update( void ) = 0;

      virtual bool isPaused( void ) const = 0;

      bool isStarted( void ) const { return _started; }
      void setStarted(const bool& started ) { _started = started; }
    
      ICamera* getCamera( void ) const { return _camera.get( ); }
      void setCamera( ICamera* camera ) { _camera.reset( camera ); }
    };
  }
}