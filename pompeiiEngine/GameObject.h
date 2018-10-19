#pragma once

#include <string>

#include "Components/Component.h"
#include <vector>
#include <memory>

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
      std::vector< std::unique_ptr< Component > > _components;
    public:
      explicit GameObject( /* Transform TODO */ );
      GameObject(const GameObject&) = delete; 
      GameObject& operator=(const GameObject&) = delete;

      ~GameObject( void );

      void update( void );

      std::string getName( void ) const { return _name; }
      void setName( const std::string& name ) { _name = name; }

      bool isRemoved( void ) const { return _removed; }
      void setRemoved( bool removed ) { _removed = removed; }

      GameObject* getParent( void ) const { return _parent; }
      void setParent( GameObject* parent ) { _parent = parent; }

      const std::vector< std::unique_ptr< Component > >& 
        getComponents( void ) const { return _components; }

      uint32_t getComponentCount( void ) const
      {
        return _components.size( );
      }

      template< class T >
      T* getComponent( bool checkDisabled = false )
      {
        T* component = nullptr;

        for ( const auto& c : _components)
        {
          auto comp = dynamic_cast< T* >( c.get( ) );

          if ( comp != nullptr )
          {
            if ( !checkDisabled && !comp->isEnabled( ) )
            {
              component = comp;
              continue;
            }

            return comp;
          }
        }

        return component;
      }

      bool addComponent( Component *component );

      template<typename T, typename... Args>
      bool addComponent(Args &&... args)
      {
        auto comp = new T( std::forward< Args >( args )... );
        return addComponent(comp);
      }

      template< class T >
      bool removeComponent( void )
      {
        for ( auto it = _components.begin( );
          it != _components.end( ); ++it )
        {
          auto comp = dynamic_cast< T* >( ( *it ).get( ) );

          if ( comp != nullptr )
          {
            ( *it )->setGameObject( nullptr );

            _components.erase( it );
            return true;
          }
        }

        return false;
      }
      bool removeComponent( const std::string& name );

      #include "GameObject.inl"
    };
  }
}