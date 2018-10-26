#pragma once

#include "SystemRegister.h"
#include "SystemUpdater.h"

#include "RendererSystem.h"
#include "SceneSystem.h"
#include "DisplaySystem.h"

namespace pompeii
{
  namespace engine
  {
    SystemRegister::SystemRegister( void )
    {
      registerSystem< RendererSystem >( ISystem::UpdateType::Render );
      registerSystem< SceneSystem >( ISystem::UpdateType::Normal );
      registerSystem< DisplaySystem >( ISystem::UpdateType::Post );
    }
    void SystemRegister::runUpdate(const ISystem::UpdateType& update) const
    {
      for ( auto it = _systems.begin( ); it != _systems.end( ); ++it )
      {
        auto key = it->first;
        if (static_cast<int32_t>(std::floor(key)) == 
          static_cast<int32_t>( update ) )
        {
          it->second->update( );
        }
      }
    }
  }
}