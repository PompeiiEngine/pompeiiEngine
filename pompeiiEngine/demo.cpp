/**
 * Copyright (c) 2017 - 2018, Pompeii
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#include <iostream>

#include <pompeiiEngine/pompeiiEngine.h>
using namespace pompeii::engine;

int main( int, char** )
{
  GameObject* world = new GameObject( );
  std::cout << "Num. components: " << world->getComponentCount( ) << std::endl;
  Light* l = new Light( );
  world->addComponent( l );
  std::cout << "Num. components: " << world->getComponentCount( ) << std::endl;
  world->addComponent( new Mesh( ) );
  std::cout << "Num. components: " << world->getComponentCount( ) << std::endl;
  world->removeComponent< Light >( );
  std::cout << "Num. components: " << world->getComponentCount( ) << std::endl;
  world->removeComponent( Mesh::StaticGetUID( ) );
  std::cout << "Num. components: " << world->getComponentCount( ) << std::endl;
  delete world;

  auto sun = new GameObject( );
  sun->addComponent< Mesh >( std::make_shared< Model > ( ) );
  sun->addComponent< SimpleMaterial > ( glm::vec3( 1.0f, 0.0f, 0.0f ) );
  return EXIT_SUCCESS;
}