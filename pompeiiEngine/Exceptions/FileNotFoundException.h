#pragma once

#include "Exception.h"

namespace pompeii
{
  namespace engine
  {
  	class FileNotFoundException: public Exception
  	{
  	public:
  		FileNotFoundException( std::string filePath )
  		: Exception( "Cannot find file " + filePath )
  		{
  		}
  	};
  }
}