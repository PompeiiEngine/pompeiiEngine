#pragma once

#include <string>

namespace pompeii
{
  namespace engine
  {
    typedef std::string ComponentUID;
    #define IMPLEMENT_COMPONENT(__CLASS__) \
    public: \
      static ComponentUID StaticGetUID( void ) { \
        static std::string sUID = #__CLASS__; \
        return ( ComponentUID ) sUID; /* This will be unique! */ \
      } \
      virtual ComponentUID GetUID( void ) const { return StaticGetUID( ); }
    
    class GameObject;
    class IComponent
    {
    private:
      GameObject* _gameObject;
      bool _started, _enabled, _removed;
    public:
      explicit IComponent( void ) :
        _gameObject(nullptr),
        _started(false),
        _enabled(true),
        _removed(false)
      {
      }

      virtual void start( void ) { }
      virtual void update( void ) { }

      virtual ComponentUID GetUID( void ) const = 0;
      
      GameObject* getGameObject( void ) const { return _gameObject; }
      void setGameObject( GameObject* go ) { _gameObject = go; }

      bool isStarted( void ) const { return _started; }
      bool setStarted( bool started ) /*const*/ { _started = started; }

      bool isEnabled( void ) const { return _enabled; }
      bool setEnabled( bool enabled ) /*const*/ { _enabled = enabled; }

      bool isRemoved( void ) const { return _removed; }
      bool setRemoved( bool removed ) /*const*/ { _removed = removed; }
    };
  }
}