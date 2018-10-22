#pragma once

#include "SystemRegister.h"
#include "SystemUpdater.h"

namespace pompeii
{
  namespace engine
  {
    void SystemRegister::runUpdate(const ISystem::UpdateType& update) const
    {
      for ( auto it = _systems.begin( ); it != _systems.end( ); ++it )
      {
        auto key = it->first;
        auto module = it->second;
        if (static_cast<int32_t>(std::floor(key)) == update)
        {
          module->update( );
        }
      }
    }
  }
}