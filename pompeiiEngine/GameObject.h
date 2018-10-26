#pragma once

#include <string>

#include "Components/Component.h"
#include <vector>
#include <memory>

#include "Maths/Transform.h"

#include "Visitor.h"

namespace pompeii
{
  namespace engine
  {
    class GameObject
    {
    private:
      Transform _transform;
      std::string _name;
      GameObject* _parent;
      bool _removed;
      std::vector< std::unique_ptr< Component > > _components;
    public:
      std::vector< GameObject* > _children;

      explicit GameObject( const std::string& name = "default", 
        const Transform& transform = Transform( ) );
      GameObject(const GameObject&) = delete; 
      GameObject& operator=(const GameObject&) = delete;

      ~GameObject( void );

      void update( void );

      std::string getName( void ) const { return _name; }
      void setName( const std::string& name ) { _name = name; }

      bool isRemoved( void ) const { return _removed; }
      void setRemoved( bool removed ) { _removed = removed; }

      bool hasParent( void ) const { return _parent != nullptr; }
      GameObject* getParent( void ) const { return _parent; }
      void setParent( GameObject* parent ) { _parent = parent; }

      const std::vector< std::unique_ptr< Component > >& 
        getComponents( void ) const { return _components; }

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
      Component* getComponent( const std::string& name );

      template<typename T, typename... Args>
      bool addComponent(Args &&... args)
      {
        auto component = new T( std::forward< Args >( args ) ... );
        return addComponent(component);
      }
      bool addComponent( Component *component );

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


      template< class T >
      bool hasComponent( void )
      {
        for ( auto it = _components.begin( );
          it != _components.end( ); ++it )
        {
          auto comp = dynamic_cast< T* >( ( *it ).get( ) );

          if ( comp != nullptr )
          {
            return true;
          }
        }

        return false;
      }
      bool hasComponent( const std::string& name );

      
      void perform( Visitor &visitor )
      {
        visitor.traverse( this );
      }
      void perform( const Visitor &visitor )
      {
        const_cast< Visitor& >( visitor ).traverse( this );
      }
      virtual void accept( Visitor &visitor )
      {
        visitor.visit( this );
      }

      virtual void addChild( GameObject* go )
      {
        GameObject* parent = go->getParent( );
        if ( parent == this )
        {
          throw; // RuntimeException( "You trying to add this node repeated" );
        }
        if ( parent != nullptr )
        {
          throw; //HasParentException( go->getName( ), getName( ), parent->getName( ) );
        }
        go->setParent( this );
        _children.push_back( go );
      }

      #include "GameObject.inl"
    };
  }
}