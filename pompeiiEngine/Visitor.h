#pragma once

namespace pompeii
{
  namespace engine
  {
    class GameObject;
    class Visitor
    {
    protected:
      Visitor( void ) { }
    public:
      virtual ~Visitor( void );
      virtual void reset( void );
      virtual void traverse( GameObject* go );
      virtual void visit( GameObject* go );
    private:
      //Visitor( const Visitor& );
      //Visitor &operator= ( const Visitor& );
    };
  }
}