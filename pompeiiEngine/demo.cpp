#include <iostream>

#include <pompeii/pompeii.h>
#include <pompeiiEngine/pompeiiEngine.h>
using namespace pompeii;
using namespace pompeii::engine;

#include <glm/gtc/type_ptr.hpp>

class UniformHandler
{
public:
  UniformBlock* uBlock;
  //std::shared_ptr< UniformBuffer > ubo;
  void* data;
  bool changed = false;


  explicit UniformHandler( void )
    : uBlock( nullptr )
    , data( nullptr )
    , changed( false )
  {
  }

  explicit UniformHandler(UniformBlock* uniformBlock)
    : uBlock( uniformBlock )
    , data(malloc(static_cast< uint32_t >(uniformBlock->size)))
    , changed( true )
  {

  }

  ~UniformHandler()
  {
    free( data );
  }

  template< typename T >
  void push( const T& obj, uint32_t offset, uint32_t size )
  {
    //std::copy( ( char* ) data, ( char* ) data + offset, &obj );

    memcpy( ( char* ) data + offset, &obj, size );
    changed = true;
  }
  template< typename T >
  void push( const std::string& uboName, const T& obj, uint32_t size = 0 )
  {
    if(uBlock == nullptr) throw;
    auto u = uBlock->getUniform( uboName );
    if(u == nullptr) throw;

    uint32_t size_ = size;
    if( size_ == 0 )
    {
      size_ = std::min( static_cast< uint32_t > ( sizeof( obj ) ),
        static_cast< uint32_t > ( u->size ) );
    }
    push( obj, static_cast<uint32_t>( u->offset ), size_ );
  }

  bool update( UniformBlock* ub = nullptr )
  {
    if( ub != uBlock )
    {
      // reset
      uBlock = ub;
      // ubo = new ...
      data = malloc( static_cast< uint32_t > ( ub->size ) );
      changed = false;
      return false;
    }
    if( changed )
    {
      // ubo->update( data );
      changed = false;
    }
    return true;
  }
};

int main( int, char** )
{
  ShaderProgram program ("fooProgram");

  program.addShader( R"(#version 450
    #extension GL_ARB_separate_shader_objects : enable
    #extension GL_ARB_shading_language_420pack : enable

    layout(set = 0, binding = 0) uniform UboObject
    {
      vec4 transform;
      vec4 colourOffset;
      vec2 atlasOffset;
      float atlasRows;
      float alpha;
    } object;

    layout(set = 0, location = 0) in vec3 inPosition;
    layout(set = 0, location = 1) in vec2 inUv;

    layout(location = 0) out vec2 outUv;

    out gl_PerVertex 
    {
      vec4 gl_Position;
    };

    void main()
    {
      gl_Position = vec4((inPosition.xy * object.transform.xy) + object.transform.zw, 0.0f, 1.0f);

      outUv = (inUv.xy / object.atlasRows) + object.atlasOffset;
    } )", vk::ShaderStageFlagBits::eVertex );
  program.addShader( R"(#version 450
    #extension GL_ARB_separate_shader_objects : enable
    #extension GL_ARB_shading_language_420pack : enable

    layout(set = 0, binding = 0) uniform UboObject
    {
      vec4 transform;
      vec4 colourOffset;
      vec2 atlasOffset;
      float atlasRows;
      float alpha;
    } object;

    layout(set = 0, binding = 1) uniform sampler2D samplerColour;

    layout(location = 0) in vec2 inUv;

    layout(location = 0) out vec4 outColour;

    void main() 
    {
      outColour = texture(samplerColour, inUv) * vec4(object.colourOffset.rgb, 1.0f);
      outColour.a *= object.alpha;

      if (outColour.a < 0.05f)
      {
        outColour = vec4(0.0f);
        discard;
      }
    } )", vk::ShaderStageFlagBits::eFragment );

  program.ProcessShader( );

  program.dump( );

  UniformBlock* ub;
  for( const auto& ubb: program.uniformBlocks )
  {
    if(ubb.get( )->name == "UboObject")
    {
      ub = ubb.get( );
      break;
    }
  }

  std::cout << "SIZE: " << ub->size << std::endl;

  UniformHandler uh(ub);

  uh.push("colourOffset", glm::value_ptr( glm::vec4( 1.0f ) ) );
  uh.push("atlasOffset", glm::value_ptr( glm::vec2( ) ) );
  uh.push("alpha", 1.25f);


  
  return EXIT_SUCCESS;
}