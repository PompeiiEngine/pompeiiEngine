#include <pompeii/pompeii.h>
#include <pompeiiEngine/pompeiiEngine.h>
using namespace pompeii;
using namespace pompeii::engine;

#include <routes.h>

std::shared_ptr< Device > device;



class ComputePipeline2
{
public:
  ComputePipeline2( const std::string& shader )
    : _shaderProgram( std::make_unique< ShaderProgram >( "program" ) )
  {
    //auto display = Engine::instance( )->getSystem< DisplaySystem >( );
    //_device = display->getDevice( );
    _device = device;

    createShaderProgram( shader );
    createDescriptorLayout( );
    

    // TODO _descriptorPool = ;
    std::vector<vk::DescriptorPoolSize> poolSizes;
    for( const auto& desc: _shaderProgram->descriptors )
    {
      poolSizes.emplace_back( vk::DescriptorPoolSize( desc.descriptorType, 3 ) ); // TODO: HARCODED!!
    }

    _descriptorPool = device->createDescriptorPool( 1, poolSizes );

    _pipelineLayout = _device->createPipelineLayout( 
      _descriptorSetLayout, _shaderProgram->pushConstantRange
    );
    
    auto computeStage = _device->createShaderPipelineShaderStage(
      _shaderModule,
      vk::ShaderStageFlagBits::eCompute
    );
    _pipeline = _device->createComputePipeline( nullptr, {}, computeStage, _pipelineLayout ); 
  }
  ~ComputePipeline2( void )
  {
    _device->waitIdle( );

    _shaderModule.reset( );
    _descriptorSetLayout.reset( );
    // TODO: NECESARY THIS?? _descriptorPool.reset( );
    _pipeline.reset( );
    _pipelineLayout.reset( );
  }

  void dispatch( const std::shared_ptr< CommandBuffer >& cmd, uint32_t groupX, uint32_t groupY, uint32_t groupZ )
  {
    cmd->dispatch( groupX, groupY, groupZ );
  }

  bool cmdRender( const std::shared_ptr< CommandBuffer >& cmd )
  {
    cmd->dispatch( 1, 1, 1 );// TODO: groupX, groupY, groupZ );
  }
  std::shared_ptr< DescriptorSetLayout > getDescriptorSetLayout( void ) const
  {
    return _descriptorSetLayout;
  }
  std::shared_ptr< DescriptorPool > getDescriptorPool( void ) const
  {
    return _descriptorPool;
  }
  std::shared_ptr< Pipeline > getPipeline( void ) const
  {
    return _pipeline;
  }
  std::shared_ptr< PipelineLayout > getPipelineLayout( void ) const
  {
    return _pipelineLayout;
  }
private:
  void createShaderProgram( const std::string& shader )
  {
    _shaderModule = _device->createShaderModule( _shaderProgram->addShader( shader, vk::ShaderStageFlagBits::eCompute ) );

    _shaderProgram->processShader( );

    _shaderProgram->dump( );
  }

  void createDescriptorLayout( void )
  {
    _descriptorSetLayout = _device->createDescriptorSetLayout( 
      _shaderProgram->descriptors
    );
  }
private:
  std::unique_ptr< ShaderProgram > _shaderProgram;
  std::shared_ptr< ShaderModule > _shaderModule;
  std::shared_ptr< Device > _device;
  std::shared_ptr< DescriptorPool > _descriptorPool;
  std::shared_ptr< DescriptorSetLayout > _descriptorSetLayout;
  std::shared_ptr< PipelineLayout > _pipelineLayout;
  std::shared_ptr< Pipeline > _pipeline;
};


class StorageHandler
{
public:
  UniformBlock* uBlock;
  std::shared_ptr< StorageBuffer > storage;
  void* data;
  bool changed = false;

  explicit StorageHandler( void )
    : uBlock( nullptr )
    , storage( nullptr )
    , data( nullptr )
    , changed( false )
  {
  }

  explicit StorageHandler(UniformBlock* uniformBlock)
    : uBlock( uniformBlock )
    , storage( device->createStorageBuffer( uBlock->size ) )
    , data(malloc(static_cast< uint32_t >(uniformBlock->size)))
    , changed( true )
  {

  }

  ~StorageHandler()
  {
    free( data );
  }

  template< typename T >
  void setUniform( const T& obj, uint32_t offset, uint32_t size )
  {
    //std::copy( ( char* ) data, ( char* ) data + offset, &obj );

    memcpy( ( char* ) data + offset, &obj, size );
    changed = true;
  }
  template< typename T >
  void setUniform( const std::string& uboName, const T& obj, uint32_t size = 0 )
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
    setUniform( obj, static_cast<uint32_t>( u->offset ), size_ );
  }

  bool update( UniformBlock* ub = nullptr )
  {
    if( ub != uBlock )
    {
      // reset
      uBlock = ub;
      storage = device->createStorageBuffer( uBlock->size );
      data = malloc( static_cast< uint32_t > ( ub->size ) );
      changed = false;
      return false;
    }
    if( changed )
    {
      storage->set( data );
      changed = false;
    }
    return true;
  }
};

class DescriptorSet2;

class IDescriptor
{
public:
  virtual WriteDescriptorSet getWriteDescriptor( const uint32_t& binding, const ::DescriptorSet2& descriptorSet ) const = 0;
};

class DescriptorSet2
{
protected:
  std::shared_ptr< PipelineLayout > _pipelineLayout;
  std::shared_ptr< DescriptorPool > _descriptorPool;
public:
  std::shared_ptr< DescriptorSet > _descriptorSet;
public:
  explicit DescriptorSet2( const ComputePipeline2& pipeline )
    : _pipelineLayout( pipeline.getPipelineLayout( ) )
    , _descriptorPool( pipeline.getDescriptorPool( ) )
    , _descriptorSet( nullptr )
  {
    auto _device = device;

    _descriptorSet = _device->allocateDescriptorSet( _descriptorPool, pipeline.getDescriptorSetLayout( ) );
  }
  ~DescriptorSet2( )
  {
    _descriptorSet.reset( );
  }
  void update( std::vector< IDescriptor*>& descriptors )
  {
    auto _device = device;
    std::vector<WriteDescriptorSet> wdss;
    for( uint32_t i = 0; i < descriptors.size( ); ++i )
    {
      if( descriptors.at( i ) != nullptr )
      {
        wdss.emplace_back( descriptors.at( i )->getWriteDescriptor( i, *this ) );
      }
    }
    _device->updateDescriptorSets( wdss, { } );
    /* =
    {
      WriteDescriptorSet( descriptorSet, 0, 0,
        vk::DescriptorType::eStorageBuffer, 1, nullptr,
        DescriptorBufferInfo( inABuffer, 0, bufferSize )
      ),
      WriteDescriptorSet( descriptorSet, 1, 0,
        vk::DescriptorType::eStorageBuffer, 1, nullptr,
        DescriptorBufferInfo( inBBuffer, 0, bufferSize )
      ),
      WriteDescriptorSet( descriptorSet, 2, 0,
        vk::DescriptorType::eStorageBuffer, 1, nullptr,
        DescriptorBufferInfo( outBuffer, 0, bufferSize )
      )
    };
    device->updateDescriptorSets( wdss, { } );*/
  }

  void bindDescriptor( const std::shared_ptr< CommandBuffer >& cmd )
  {
    cmd->bindDescriptorSets( vk::PipelineBindPoint::eCompute, _pipelineLayout, 0, { _descriptorSet }, { } );
  }
};

/*class StorageBuffer2
  : public pompeii::StorageBuffer
  , public IDescriptor
{
public:
  virtual WriteDescriptorSet getWriteDescriptor( 
    const uint32_t& binding, const ::DescriptorSet2& descriptorSet ) const override
  {
    return WriteDescriptorSet( descriptorSet._descriptorSet, binding, 0,
      vk::DescriptorType::eStorageBuffer, 1, nullptr,
      DescriptorBufferInfo( shared_from_this( ), 0, _size )
    );
  }
};*/

int main( void )
{
  std::cout << "Create Vulkan Instance...";
    auto instance = Instance::createDefault( "Compute Sum" );
  std::cout << "OK" << std::endl;

  std::cout << "Find Vulkan physical device...";
    // Find a physical device with presentation support
    assert( instance->getPhysicalDeviceCount( ) != 0 );
    auto physicalDevice = instance->getPhysicalDevice( 0 );
    if ( !physicalDevice )
    {
      POMPEII_RUNTIME_ERROR( "Failed to find a device with presentation support" );
    }
  std::cout << "OK" << std::endl;

  std::cout << "Create logical device...";
    // Search for a compute queue in the array of 
    //    queue families, try to find one that support
    auto queueFamilyIndices = physicalDevice->getComputeQueueFamilyIndices( );
    assert( !queueFamilyIndices.empty( ) );
    const uint32_t queueFamilyIndex = queueFamilyIndices[ 0 ];
    std::vector<float> queuePriorities = { 1.0f };
    vk::DeviceQueueCreateInfo queueCreateInfo;
    queueCreateInfo.queueFamilyIndex = static_cast<uint32_t>( queueFamilyIndex );
    queueCreateInfo.queueCount = static_cast<uint32_t>( queuePriorities.size( ) );
    queueCreateInfo.pQueuePriorities = &queuePriorities[ 0 ];
    vk::DeviceCreateInfo deviceCreateInfo;
    deviceCreateInfo.enabledExtensionCount = 0;
    deviceCreateInfo.ppEnabledExtensionNames = nullptr;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;

    device = physicalDevice->createDevice(
      { queueCreateInfo }, { }, { },  physicalDevice->getDeviceFeatures( )
    );
  std::cout << "OK" << std::endl;

  std::cout << "Allocate buffers...";
    const uint32_t bufferElements = 10;
    const vk::DeviceSize bufferSize = bufferElements * sizeof( int32_t );
    std::shared_ptr< StorageBuffer > inABuffer, inBBuffer, outBuffer;
    inABuffer = device->createStorageBuffer( bufferSize );
    inBBuffer = device->createStorageBuffer( bufferSize );
    outBuffer = device->createStorageBuffer( bufferSize );
  std::cout << "OK" << std::endl;


  ::ComputePipeline2 cp( R"(#version 450
    layout( local_size_x = 1, local_size_y = 1 ) in;
    
    layout(std430, binding = 0) buffer InputBufferA
    {
      float inA[ ];
    };
    layout(std430, binding = 1) buffer InputBufferB
    {
      float inB[ ];
    };
    layout(std430, binding = 2) buffer OutputBuffer
    {
      float outBuffer[ ];
    };

    #define ID gl_GlobalInvocationID.x

    void main( )
    {
      outBuffer[ ID ] = inA[ ID ] + inB[ ID ];
    } )" );

    auto descriptorSet = device->allocateDescriptorSet( cp.getDescriptorPool( ), cp.getDescriptorSetLayout( ) );
  std::cout << "OK" << std::endl;

  std::cout << "Prepare commands buffers...";
    auto commandPool = device->createCommandPool( { }, queueFamilyIndex );
  std::cout << "OK" << std::endl;

  std::cout << "Prepare descriptors set...";
    std::vector< WriteDescriptorSet > wdss =
    {
      WriteDescriptorSet( descriptorSet, 0, 0,
        vk::DescriptorType::eStorageBuffer, 1, nullptr,
        DescriptorBufferInfo( inABuffer, 0, bufferSize )
      ),
      WriteDescriptorSet( descriptorSet, 1, 0,
        vk::DescriptorType::eStorageBuffer, 1, nullptr,
        DescriptorBufferInfo( inBBuffer, 0, bufferSize )
      ),
      WriteDescriptorSet( descriptorSet, 2, 0,
        vk::DescriptorType::eStorageBuffer, 1, nullptr,
        DescriptorBufferInfo( outBuffer, 0, bufferSize )
      )
    };
    device->updateDescriptorSets( wdss, { } );
  std::cout << "OK" << std::endl;

  std::cout << "Upload input data...";
    std::vector<float> hostDataA( bufferElements );
    const float min = 1.0f, max = 99.0f;
    for ( float& n : hostDataA )
    {
      n = ( max - min ) * ( ( ( ( float ) rand( ) ) / ( float ) RAND_MAX ) ) + min;
    }
    inABuffer->writeData( 0, bufferSize, hostDataA.data( ) );

    std::vector<float> hostDataB( bufferElements );

    std::vector<float> cpuResult( bufferElements );
    size_t i = 0;
    for ( float& n : hostDataB )
    {
      n = min + ( max - min ) * ( ( ( ( float ) rand( ) ) / ( float ) RAND_MAX ) ) + min;

      cpuResult[ i ] = hostDataA[ i ] + n;
      ++i;
    }
    inBBuffer->writeData( 0, bufferSize, hostDataB.data( ) );
  std::cout << "OK" << std::endl;

  std::cout << "Input data:" << std::endl;

  for ( i = 0; i < bufferElements; ++i )
  {
    std::cout << (hostDataA[ i ] + hostDataB[ i ]) << ", ";
  }
  std::cout << "..., " << ( hostDataA[ bufferElements - 1 ] + 
    hostDataB[ bufferElements - 1 ]) << std::endl;

  std::cout << "Run computations...";

  // Now create an event and wait for it on the GPU
  auto ev = device->createEvent( );

  auto commandBuffer = commandPool->allocateCommandBuffer( );
  commandBuffer->begin( vk::CommandBufferUsageFlagBits::eOneTimeSubmit );

  commandBuffer->setEvent( ev, vk::PipelineStageFlagBits::eBottomOfPipe );

  commandBuffer->bindComputePipeline( cp.getPipeline( ) );
  commandBuffer->bindDescriptorSets( vk::PipelineBindPoint::eCompute, 
    cp.getPipelineLayout( ), 0, { descriptorSet }, { } );
  commandBuffer->dispatch( bufferElements, 1, 1 );
  commandBuffer->end( );

  auto queue = device->getQueue( queueFamilyIndex, 0 );

  // Look for the event on the CPU. It should be RESET since we haven't sent
  // the command buffer yet.
  auto res = ev->getStatus( );
  assert( res == vk::Result::eEventReset );

  // Send the command buffer and loop waiting for the event
  queue->submit( commandBuffer );

  int polls = 0;
  do
  {
    res = ev->getStatus( );
    ++polls;
  } while ( res != vk::Result::eEventSet );
  printf( "%d polls to find the event set\n", polls );

  queue->waitIdle( );

  std::cout << "OK" << std::endl;

  std::cout << "Read results...";
  std::vector<float> result( bufferElements );
  outBuffer->readData( 0, bufferSize, result.data( ) );

  if ( std::equal( result.begin( ), result.end( ), 
    cpuResult.begin( ), []( float r, float h ) { return r == h; } ) )
  {
    std::cout << "Ok. Same arays as equals." << std::endl;
  }
  else
  {
    std::cout << "Fail. Invalid result." << std::endl;
  }

  std::cout << "Output data:" << std::endl;
  for ( i = 0; i < bufferElements; ++i )
  {
    std::cout << result[ i ] << ", ";
  }
  std::cout << "..., " << result[ bufferElements - 1 ] << std::endl;

  return 0;
}