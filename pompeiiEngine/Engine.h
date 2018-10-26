#pragma once

#include "SystemRegister.h"
#include "SystemUpdater.h"

namespace pompeii
{
  namespace engine
  {
    class Engine
    {
    static Engine* _instance;
    protected:
      bool _running;

      SystemUpdater _systemUpdater;
      SystemRegister _systemRegister;
    public:
      static Engine* instance( void ) { return _instance; }

      explicit Engine( void )
        : _running ( true )
      {
        _instance = this;


      }
  
      ~Engine( void )
      {

      }      

      template< typename T >
      T* getSystem( void ) const
      {
        return _systemRegister.getSystem< T >( );
      }
      template<typename T>
      T* registerSystem( const ISystem::UpdateType& update )
      {
        return _systemRegister.registerSystem< T >( update );
      }
      template<typename T>
      bool deregisterSystem( void )
      {
        return _systemRegister.deregisterSystem< T >( );
      }
      bool isRunning( void ) const
      {
        return _running;
      }

      void run( void )
      {
        while( _running )
        {
          std::cout << "Engine::run" << std::endl;
          _systemUpdater.update( _systemRegister );
          _running = false;
        }
      }
    };
  }
}