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
        Always,
        Pre,
        Normal,
        Post,
        Render
      };

      virtual void update( void ) = 0;
    };
  }
}