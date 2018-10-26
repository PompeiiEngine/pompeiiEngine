#pragma once

#include "ISystem.h"

#include <pompeii/pompeii.h>
#include <GLFW/glfw3.h>

namespace pompeii
{
  namespace engine
  {
    class DisplaySystem
      : public ISystem
    {
    private:
      GLFWwindow* _window;
      std::shared_ptr< Instance > _instance;
      std::shared_ptr< Surface > _surface;
      std::shared_ptr< Device > _logicalDevice;
      std::shared_ptr< PhysicalDevice > _physicalDevice;

      bool _fullscreen = false;

      vk::PhysicalDeviceProperties _physicalDeviceProperties;
      vk::PhysicalDeviceFeatures _physicalDeviceFeatures;
      vk::PhysicalDeviceMemoryProperties _physicalDeviceMemoryProperties;

      vk::QueueFlags _supportedQueues;
      uint32_t _graphicsFamily;
      uint32_t _presentFamily;
      uint32_t _computeFamily;
      uint32_t _transferFamily;
      std::shared_ptr< Queue > _graphicsQueue;
      std::shared_ptr< Queue > _presentQueue;
      std::shared_ptr< Queue > _computeQueue;
      std::shared_ptr< Queue > _transferQueue;

      uint32_t _windowWidth, _positionX, _fullscreenWidth = 0u;
      uint32_t _windowHeight, _positionY, _fullscreenHeight = 0u;
      std::string _title;
    public:
      DisplaySystem( void );
      ~DisplaySystem( void );

      virtual void update( void ) override;

      void setFullscreen( bool fullscreen );

      void setTitle( const std::string& title );

      std::shared_ptr< Queue > getGraphicsQueue( void ) const
      {
        return _graphicsQueue;
      }

      std::shared_ptr< Queue > getPresentQueue( void ) const
      {
        return _presentQueue;
      }

      std::shared_ptr< Queue > getComputeQueue( void ) const
      {
        return _computeQueue;
      }

      std::shared_ptr< Queue > getTransferQueue( void ) const
      {
        return _transferQueue;
      }

      std::shared_ptr< Device > getDevice( void ) const
      {
        return _logicalDevice;
      }
    };
  }
}