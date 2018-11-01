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
  T getParameter( const std::string& uboName )
  {
    if(uBlock == nullptr) throw;
    auto u = uBlock->getUniform( uboName );
    if(u == nullptr) throw;

    uint32_t offset = u->offset;

    uint32_t size_ = std::min( 
      static_cast< uint32_t > ( sizeof( T ) ),
      static_cast< uint32_t > ( u->size )
    );

    T v;

    //memcpy( &v, data + offset, static_cast< uint32_t > ( u->size ) - static_cast< uint32_t > ( sizeof( T ) ) + offset );

    memcpy( &v, ( char* ) data + offset, u->size );

    return v;
  }

  template< typename T >
  void setParameter( const T& obj, uint32_t offset, uint32_t size )
  {
    //std::copy( ( char* ) data, ( char* ) data + offset, &obj );

    memcpy( ( char* ) data + offset, &obj, size );
    changed = true;
  }
  template< typename T >
  void setParameter( const std::string& uboName, const T& obj, uint32_t size = 0 )
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
    setParameter( obj, static_cast<uint32_t>( u->offset ), size_ );
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
      vec2 atlasOffset;
      vec4 transform;
      vec4 colourOffset;
      float atlasRows;
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
      vec2 atlasOffset;
      vec4 transform;
      vec4 colourOffset;
      float atlasRows;
    } object;

    layout( set = 0, binding = 1 ) uniform UboObject2
    {
      float value2;
      vec3 value;
      float alpha;
    } object2;

    layout(set = 0, binding = 1) uniform sampler2D samplerColour;

    layout(location = 0) in vec2 inUv;

    layout(location = 0) out vec4 outColour;

    void main() 
    {
      outColour = texture(samplerColour, inUv) * vec4(object.colourOffset.rgb, 1.0f);
      outColour.a *= object2.alpha;

      if (outColour.a < 0.05)
      {
        outColour = vec4(object2.value.x, object2.value.y, object2.value.z, object2.value2);
        discard;
      }
    } )", vk::ShaderStageFlagBits::eFragment );

  program.processShader( );

  program.dump( );

  UniformBlock* ub;
  for( const auto& ubb: program.uniformBlocks )
  {
    if(ubb.get( )->name == "UboObject2")
    {
      ub = ubb.get( );
      break;
    }
  }

  std::cout << "SIZE: " << ub->size << std::endl;

  UniformHandler uh(ub);

  uh.setParameter("value", glm::vec3( 1.2f, 2.4f, 3.6f ) );
  uh.setParameter("value2", 4.8f );
  uh.setParameter("alpha", 0.01f );

  std::cout << "value2: " << uh.getParameter<float>("value2") << std::endl;
  std::cout << "value: " << 
    glm::to_string( uh.getParameter<glm::vec3>("value") ) << std::endl;
  std::cout << "alpha: " << uh.getParameter<float>("alpha") << std::endl;

  struct UboObject2
  {
    float value2        ;
    glm::vec3 value     ;
    float alpha         ;
  };

  std::cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl;

  UboObject2 obj = *(static_cast<UboObject2*>(uh.data));

  std::cout << "value2: " << obj.value2 << std::endl;
  std::cout << "value: " << obj.value[0] << std::endl;
  std::cout << "value: " << glm::to_string( obj.value ) << std::endl;
  std::cout << "alpha: " << obj.alpha << std::endl;

  /*uh.setParameter("atlasOffset", glm::value_ptr( glm::vec2( 0.25f, 0.75f ) ) );
  uh.setParameter("transform", glm::value_ptr( glm::vec4( 1.0f, 2.2f, 3.3f, 4.6f ) ) );
  uh.setParameter("alpha", 1.25f);

  std::cout << "SIZEOF: " << sizeof( glm::vec4 ) << std::endl;

  std::cout << "atlasOffset: " << glm::to_string( uh.getParameter<glm::vec2>("atlasOffset") ) << std::endl;
  std::cout << "transform: " << glm::to_string( uh.getParameter<glm::vec4>("transform") ) << std::endl;
  std::cout << "alpha: " << uh.getParameter<float>("alpha") << std::endl;

  struct Object {
    glm::vec2 atlasOffset;
    glm::vec4 transform;
    glm::vec4 colourOffset;
    float alpha;
    float atlasRows;
  };

  Object obj = *(static_cast<Object*>(uh.data));

  std::cout << "atlasOffset: " << glm::to_string( obj.atlasOffset ) << std::endl;
  std::cout << "transform: " << glm::to_string( obj.transform ) << std::endl;
  std::cout << "alpha: " << obj.alpha << std::endl;*/

  
  return EXIT_SUCCESS;
}