#pragma once

#include <stdexcept>
#include <string>

namespace pompeii
{
  namespace engine
  {
  	class Exception: public std::runtime_error
  	{
  	public:
  		Exception( std::string message )
  		: std::runtime_error( message.c_str( ) )
  		{
  		}
  	};
  }
}