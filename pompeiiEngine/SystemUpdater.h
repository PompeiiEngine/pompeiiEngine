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
        std::cout << "SystemRegister::update::Always" << std::endl;
        sysRegister.runUpdate( ISystem::UpdateType::Always );


        std::cout << "SystemRegister::update::Pre" << std::endl;
        sysRegister.runUpdate( ISystem::UpdateType::Pre );
        std::cout << "SystemRegister::update::Normal" << std::endl;
        sysRegister.runUpdate( ISystem::UpdateType::Normal );
        std::cout << "SystemRegister::update::Post" << std::endl;
        sysRegister.runUpdate( ISystem::UpdateType::Post );


        std::cout << "SystemRegister::update::Render" << std::endl;
        sysRegister.runUpdate( ISystem::UpdateType::Render );
      }

    };
  }
}