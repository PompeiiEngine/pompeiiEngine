#include "GameObject.h"

namespace pompeii
{
  namespace engine
  {
    GameObject::GameObject( const std::string& name, const Transform& t )
      : _name( name )
      , _transform( t )
      , _parent( nullptr )
    {

    }
    GameObject::~GameObject( void )
    {
      for ( auto it = _components.begin( );
        it != _components.end( ); ++it )
      {
        ( *it )->setGameObject( nullptr );
      }
      _components.clear( );
    }

    void GameObject::update( void )
    {
      for ( auto it = _components.begin( ); it != _components.end( ); )
      {
        if ( ( *it )->isRemoved( ) )
        {
          it = _components.erase( it );
          continue;
        }

        if ( ( *it )->getGameObject( ) != this )
        {
          ( *it )->setGameObject( this );
        }

        if (!(*it)->isStarted())
        {
          ( *it )->start( );
          ( *it )->setStarted( true );
        }

        if ( ( *it )->isEnabled( ) )
        {
          ( *it )->update( );
        }

        ++it;
      }
    }

    Component* GameObject::getComponent( const std::string& name )
    {
      for ( auto it = _components.begin( );
        it != _components.end( ); ++it )
      {
        auto componentName = (*it)->GetUID( );

        if ( name == componentName )
        {
          return ( *it ).get( );
        }
      }

      return nullptr;
    }

    bool GameObject::addComponent( Component* comp )
    {
      if ( comp == nullptr )
      {
        return false;
      }

      comp->setGameObject( this );
      _components.emplace_back( comp );
      return true;
    }

    bool GameObject::removeComponent( const std::string& name )
    {
      for ( auto it = _components.begin( );
        it != _components.end( ); ++it )
      {
        auto componentName = (*it)->GetUID( );

        if ( name != componentName )
        {
          continue;
        }

        ( *it )->setGameObject( nullptr );

        _components.erase( it );
        return true;
      }

      return false;
    }

    bool GameObject::hasComponent( const std::string& name )
    {
      for ( auto it = _components.begin( );
        it != _components.end( ); ++it )
      {
        auto componentName = (*it)->GetUID( );

        if ( name == componentName )
        {
          return true;
        }
      }

      return false;
    }
  }
}