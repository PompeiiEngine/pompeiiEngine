#include "Mesh.h"

namespace pompeii
{
  namespace engine
  {
    Model::Model( void )
    {

    }

    bool Model::cmdRender( /* commandBuffer */ )
    {
      std::cout << "Bind VBO and EBO" << std::endl;

      return true;
    }

    Mesh::Mesh( const std::shared_ptr< Model >& model )
      : _model( model )
    {

    }
    Mesh::Mesh( const std::string& filename )
    {
      if( filename.empty( ) )
      {
        throw;
      }
      _model = std::make_shared< Model >( );
      std::cout << "Mesh loaded" << std::endl;
    }
    void Mesh::start( void )
    {

    }
    void Mesh::update( void )
    {

    }
  }
}