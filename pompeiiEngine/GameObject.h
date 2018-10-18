#pragma once

#include <string>

namespace pompeii
{
  namespace engine
  {
    class GameObject
    {
    private:
      std::string _name;
      GameObject* _parent;
      bool _removed;
    public:
      GameObject( /* Transform TODO */ );
    };
  }
}