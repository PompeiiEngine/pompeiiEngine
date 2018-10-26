#include "DisplaySystem.h"

#include <glslang/SPIRV/GlslangToSpv.h>

namespace pompeii
{
  namespace engine
  {
    DisplaySystem::DisplaySystem( void )
      : _window( nullptr )
      , _windowWidth ( 500 )
      , _windowHeight( 500 )
      , _title( "PompeiiEngine" )
      , _supportedQueues( 0 )
    {
      // Creating GLFW window
      
      {
        // glfwSetErrorCallback( CallbackError );

        if( glfwInit( ) == GLFW_FALSE )
        {
          throw "GLFW error: Failed to initialize!";
        }

        if( glfwVulkanSupported( ) == GLFW_FALSE )
        {
          throw "GLFW error: Failed to find Vulkan support";
        }

        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        // Disable context creation
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); 

        // For new GLFW, and macOS.
        glfwWindowHint(GLFW_STENCIL_BITS, 8); // Fixes 16 bit stencil bits in macOS
        glfwWindowHint(GLFW_STEREO, GLFW_FALSE); // Disable stereo view

        // Create a windowed mode window and its context.
        _window = glfwCreateWindow( _windowWidth, _windowHeight, _title.c_str(), nullptr, nullptr);

        if ( _window == nullptr )
        {
          glfwTerminate( );
          throw "GLFW error: Failed to create GLFW window";
        }

        // Get the resolution of the primary monitor.
        GLFWmonitor* monitor = glfwGetPrimaryMonitor( );
        const GLFWvidmode* videoMode = glfwGetVideoMode( monitor );

        // Moving window to center position
        _positionX = (videoMode->width - _windowWidth) * 0.5;
        _positionY = (videoMode->height - _windowHeight) * 0.5;
        glfwSetWindowPos( _window, _positionX, _positionY );

        // Shows GLFW window.
        glfwShowWindow( _window );
    }

    // Setup layers
    {

    }

    std::vector<std::string> requiredInstanceExtensions;
    // Setup extensions
    {
      //std::vector<std::string> requiredDeviceExtensions;

      unsigned int glfw_exts_count;
      const char** glfw_exts = glfwGetRequiredInstanceExtensions( 
        &glfw_exts_count );
      for ( unsigned int i = 0; i < glfw_exts_count; ++i )
      {
        requiredInstanceExtensions.emplace_back( std::string( glfw_exts[ i ] ) );
      
        std::cout << glfw_exts[ i ] << std::endl;
      }
      //requiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };


    }

    // Instance
    {
      vk::ApplicationInfo appInfo(
        "GLFWRender", //info.appInfo.c_str( ),
        VK_MAKE_VERSION( 1, 0, 0 ), // TODO: Use version from core
        "PompeiiEngine",
        VK_MAKE_VERSION( 1, 0, 0 ), // TODO: Use version from engine
        VK_API_VERSION_1_0
      );
      vk::InstanceCreateInfo ici;
      ici.setPApplicationInfo( &appInfo );

      std::vector<const char *> requiredExts;
      {
        for ( const auto &extension : requiredInstanceExtensions )
        {
          requiredExts.push_back( extension.c_str( ) );
        }
      }

      ici.setEnabledExtensionCount( static_cast< uint32_t >( requiredExts.size( ) ) );
      ici.setPpEnabledExtensionNames( requiredExts.data( ) );

      std::vector<const char* > layers =
      {
        "VK_LAYER_LUNARG_standard_validation"
      };
      ici.setEnabledLayerCount( static_cast< uint32_t >( layers.size( ) ) );
      ici.setPpEnabledLayerNames( layers.data( ) );

      vk::Instance instance = vk::createInstance( ici );

      _instance = Instance::createFromVkInstance( instance );

      if ( !_instance )
      {
        throw "Instance don't exist";
      }
    }

    // Physical device
    {
      assert( _instance->getPhysicalDeviceCount( ) != 0 );

      _physicalDevice = _instance->getPhysicalDevice( 0 );

      if ( !_physicalDevice )
      {
        throw "PhysicalDevice don't exist";
      }
      _physicalDeviceProperties = _physicalDevice->getDeviceProperties( );
      _physicalDeviceFeatures = _physicalDevice->getDeviceFeatures( );
      _physicalDeviceMemoryProperties = _physicalDevice->getMemoryProperties( );

      // TODO Get max sample count
      {
        //vk::SampleCountFlags 
      }
    }

    // Surface
    {
      // Creates the WSI Vulkan surface.
      VkSurfaceKHR c_surface;
      VkResult result = glfwCreateWindowSurface(
        VkInstance( static_cast< vk::Instance >( *_instance ) ), 
        _window, nullptr, &c_surface );

      if ( result != VK_SUCCESS )
      {
        throw std::runtime_error( "failed to create window surface!" );
      }

      _surface = std::make_shared< Surface >( _instance,
        vk::SurfaceKHR( c_surface ), true );

      if ( !_surface )
      {
        throw "Surface don't exist";
      }
    }

    // Queues
    {
      int32_t graphicsFamily = -1,
              presentFamily = -1,
              computeFamily = -1,
              transferFamily = -1;
      // Search for a graphics queue and a present queue in the array of 
      //    queue families, try to find one that supports both
      auto queueFamilyIndex = _physicalDevice->getQueueFamilyProperties( );
      for ( uint32_t i = 0, l = queueFamilyIndex.size( ); i < l; ++i )
      {
        auto flags = queueFamilyIndex[ i ].queueFlags;

        // Check for graphics queue supporting
        if( flags & vk::QueueFlagBits::eGraphics )
        {
          std::cout << "Graphics queue founded (" << i << ")!" << std::endl;
          graphicsFamily = i;
          _graphicsFamily = i;
          _supportedQueues |= vk::QueueFlagBits::eGraphics;
        }

        // Check for compute queue supporting
        if( flags & vk::QueueFlagBits::eCompute )
        {
          std::cout << "Compute queue founded (" << i << ")!" << std::endl;
          computeFamily = i;
          _computeFamily = i;
          _supportedQueues |= vk::QueueFlagBits::eCompute;
        }

        // Check for transfer queue supporting
        if( flags & vk::QueueFlagBits::eTransfer )
        {
          std::cout << "Transfer queue founded (" << i << ")!" << std::endl;
          transferFamily = i;
          _transferFamily = i;
          _supportedQueues |= vk::QueueFlagBits::eTransfer;
        }

        // Check for presentation support.
        VkBool32 presentSupport = _physicalDevice->supportSurfaceKHR( i, _surface );

        if ( queueFamilyIndex[i].queueCount > 0 && presentSupport )
        {
          std::cout << "Present queue founded (" << i << ")!" << std::endl;
          presentFamily = i;
          _presentFamily = i;
        }

        if ( graphicsFamily != -1 && presentFamily != -1 &&
          computeFamily != -1 && transferFamily != -1 )
        {
          break;
        }
      }
      if (graphicsFamily == -1)
      {
        throw "Queue with VK_QUEUE_GRAPHICS_BIT undefined";
      }
    }

    // Logical device
    {
      std::vector< vk::DeviceQueueCreateInfo > queueCreateInfos;
      const float queuePriority = 0.0f;


      if( _supportedQueues & vk::QueueFlagBits::eGraphics )
      {
        vk::DeviceQueueCreateInfo qci;
        qci.queueFamilyIndex = _graphicsFamily;
        qci.queueCount = 1;
        qci.pQueuePriorities = &queuePriority;

        queueCreateInfos.emplace_back( qci );
      }
      else
      {
        _graphicsFamily = VK_NULL_HANDLE;
      }

      /*if( _supportedQueues & vk::QueueFlagBits::eCompute )
      {
        vk::DeviceQueueCreateInfo qci;
        qci.queueFamilyIndex = _computeFamily;
        qci.queueCount = 1;
        qci.pQueuePriorities = &queuePriority;

        queueCreateInfos.emplace_back( qci );
      }
      else
      {
        _computeFamily = VK_NULL_HANDLE;
      }

      if( _supportedQueues & vk::QueueFlagBits::eTransfer )
      {
        vk::DeviceQueueCreateInfo qci;
        qci.queueFamilyIndex = _transferFamily;
        qci.queueCount = 1;
        qci.pQueuePriorities = &queuePriority;

        queueCreateInfos.emplace_back( qci );
      }
      else
      {
        _transferFamily = VK_NULL_HANDLE;
      }*/

      // TODO: Move to another zone
      // Create a new device with the VK_KHR_SWAPCHAIN_EXTENSION enabled.
      std::vector<std::string> enabledLayerNames;

      std::vector<std::string> enabledExtensionNames;


      std::vector<std::string > _requestedDeviceExts;

      _requestedDeviceExts.push_back( VK_KHR_SWAPCHAIN_EXTENSION_NAME );

      for ( const auto& ext : _requestedDeviceExts )
      {
        if ( _physicalDevice->extensionSupported( ext ) )
        {
          enabledExtensionNames.push_back( ext );
        }
      }



      _logicalDevice = _physicalDevice->createDevice(
        queueCreateInfos,
        enabledLayerNames,
        enabledExtensionNames,
        _physicalDeviceFeatures
      );

      _graphicsQueue = _logicalDevice->getQueue( _graphicsFamily, 0 );
      _presentQueue = _logicalDevice->getQueue( _presentFamily, 0 );
      _computeQueue = _logicalDevice->getQueue( _computeFamily, 0 );
      _transferQueue = _logicalDevice->getQueue( _transferFamily, 0 );

    }

    #ifndef __ANDROID__
      glslang::InitializeProcess(); // Move to ctor of Engine
    #endif
    }
    DisplaySystem::~DisplaySystem( void )
    {
    #ifndef __ANDROID__
      glslang::FinalizeProcess(); // Move to dtor of Engine
    #endif

      // Wait for device idle to destroy GLFW widow
      _logicalDevice->waitIdle( );

      // Free glfw window's callbacks
      glfwDestroyWindow( _window );

      _logicalDevice.reset( );
      _surface.reset( );
      _physicalDevice.reset( );
      _instance.reset( );

      // Terminate glfw
      glfwTerminate( );
    }

    void DisplaySystem::update( void )
    {
      std::cout << "\tDisplaySystem::update" << std::endl;

      glfwPollEvents( );
    }

    void DisplaySystem::setTitle( const std::string& title )
    {
      _title = title;
      glfwSetWindowTitle( _window, _title.c_str( ) );
    }

    void DisplaySystem::setFullscreen( bool fullscreen )
    {
      if ( _fullscreen == fullscreen )
      {
        return;
      }

      _fullscreen = fullscreen;

      GLFWmonitor *monitor = glfwGetPrimaryMonitor(); // monitors[1];
      const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);

      if (fullscreen)
      {
        std::cout << "FULLSCREEN" << std::endl;
        _fullscreenWidth = videoMode->width;
        _fullscreenHeight = videoMode->height;
        glfwSetWindowMonitor( _window, monitor, 0, 0, _fullscreenWidth, _fullscreenHeight, GLFW_DONT_CARE );
      }
      else
      {
        std::cout << "WINDOWED" << std::endl;
        _positionX = (videoMode->width - _windowWidth) * 0.5;
        _positionY = (videoMode->height - _windowHeight) * 0.5;
        glfwSetWindowMonitor( _window, nullptr, _positionX, _positionY, _windowWidth, _windowHeight, GLFW_DONT_CARE );
      }
    }
  }
}