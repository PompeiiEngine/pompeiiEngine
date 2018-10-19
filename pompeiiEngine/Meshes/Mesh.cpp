#include "Mesh.h"

namespace pompeii
{
  namespace engine
  {
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