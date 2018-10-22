#pragma once

#include "ISystem.h"
#include <map>

#include <pompeii/pompeii.h>

namespace pompeii
{
  namespace engine
  {
    class SystemUpdater;
    class SystemRegister
      : public NonCopyable< SystemRegister >
    {
    private:
      std::map< float, std::unique_ptr< ISystem > > _systems;
    public:
      SystemRegister( void )
      {
        //registerSystem< Renderer >( ISystem::UpdateType::Render );
      }
      bool has( ISystem* system ) const
      {
        for ( auto it = _systems.begin( ); it != _systems.end( ); ++it )
        {
          if( it->second.get( ) == system )
          {
            return true;
          }
        }
        return false;
      }

      template< typename T >
      T* getSystem( void ) const
      {
        for ( auto it = _systems.begin( ); it != _systems.end( ); ++it )
        {
          auto s = dynamic_cast<T*>( it->second.get( ) );
          if( s )
          {
            return s;
          }
        }
        return nullptr;
      }

      ISystem* registerSystem( ISystem* system, 
        const ISystem::UpdateType& update )
      {
        if( has( system ) )
        {
          return nullptr;
        }
        float key = static_cast<float>(update) + 
          (0.01f * static_cast<float>(_systems.size()));
        //_systems.emplace(key, system);
        return system;
      }

      template< typename T >
      ISystem* registerSystem( const ISystem::UpdateType& update )
      {
        auto system = static_cast< T* >( malloc( sizeof( T ) ) );
        registerSystem( system, update );
        new(system) T( );
        return system;
      }

      template<typename T>
      bool deregisterSystem( void )
      {
        for (auto it = --_systems.end( ); it != _systems.begin( ); --it)
        {
          auto system = dynamic_cast<T *>((*it).second.get());

          if (system != nullptr)
          {
            _systems.erase( it );
            return true;
          }
        }

        return false;
      }

      void runUpdate(const ISystem::UpdateType& update) const;

    };
  }
}