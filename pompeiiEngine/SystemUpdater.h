#pragma once

#include "SystemRegister.h"

namespace pompeii
{
  namespace engine
  {
    class SystemUpdater
    {
    public:
      SystemUpdater( void )
      {

      }

      void update( const SystemRegister& sysRegister )
      {
        sysRegister.runUpdate( ISystem::UpdateType::Always );


        sysRegister.runUpdate( ISystem::UpdateType::Pre );
        sysRegister.runUpdate( ISystem::UpdateType::Normal );
        sysRegister.runUpdate( ISystem::UpdateType::Post );


        sysRegister.runUpdate( ISystem::UpdateType::Render );
      }

    };
  }
}