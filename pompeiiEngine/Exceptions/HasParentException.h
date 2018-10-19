#pragma once

#include "Exception.h"

namespace pompeii
{
  namespace engine
  {
  	class HasParentException: public Exception
  	{
  	public:
  		HasParentException( std::string childName, 
  			std::string parentName, std::string targetName )
  			: Exception( "Cannot attach node (\"" + childName + 
  				"\") to (\"" + targetName + 
  				"\") because it already has a parent (\"" + 
  				parentName + "\")" )
  		{
  		}
  	};
  }
}