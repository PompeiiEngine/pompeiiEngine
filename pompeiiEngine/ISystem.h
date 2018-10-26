#pragma once

namespace pompeii
{
  namespace engine
  {
    class ISystem
    {
    public:
      enum class UpdateType
      {
        Always = 0,
        Pre = 1,
        Normal = 2,
        Post = 3,
        Render = 4
      };

      virtual void update( void ) = 0;
    };
  }
}