#pragma once

#include "Exception.h"

namespace pompeii
{
  namespace engine
  {
  	class RuntimeException: public Exception
  	{
  	public:
  		RuntimeException( std::string message )
  		: Exception( "PROGRAM TERMINATED BECAUSE OF AN ERROR: " + message )
  		{
  		}
  	};
  }
}
