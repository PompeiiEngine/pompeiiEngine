#include "ComputeBatchQueue.h"

#include "GameObject.h"

namespace pompeii
{
  namespace engine
  {
    void ComputeBatchQueue::visit( GameObject* go )
    {
      /*TODOif ( _camera != nullptr &&
        _camera->layer( ).check( geometry->layer( ) ) )
      {
        _batch->pushGeometry( geometry );
      }*/
      Visitor::visit( go );
    }
    void ComputeBatchQueue::traverse( GameObject* go )
    {
      _batch->reset( );
      _batch->setCamera( _camera );

      Visitor::traverse( go );
    }
  }
}