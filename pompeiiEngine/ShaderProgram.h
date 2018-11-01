#pragma once

#include <pompeii/pompeii.h>

struct TBuiltInResource;

namespace glslang
{
  class TProgram;
}

namespace std
{
  template<typename T, typename... Args>
  std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }
}

namespace pompeii
{
  namespace engine
  {
    class Uniform
    {
    public:
      std::string name;
      int32_t binding;
      int32_t offset;
      int32_t size;
      int32_t type;
      vk::ShaderStageFlags stageFlags;

      Uniform( const std::string& name, const uint32_t& binding, 
        const uint32_t& offset, const uint32_t& size,
        const int32_t& type, const vk::ShaderStageFlagBits& stageFlags );

      bool operator==(const Uniform& other) const
      {
        return name == other.name && binding == other.binding && 
          offset == other.offset && type == other.type;
      }

      bool operator!=(const Uniform& other) const
      {
        return !(*this == other);
      }

      std::string dump( void ) const;
    };

    class VertexAttribute
    {
    public:
      std::string name;
      uint32_t location;
      uint32_t size;
      int32_t type;
      VertexAttribute( const std::string& name, const uint32_t& location,
        const uint32_t& size, const int32_t& type );

      std::string dump( void ) const;
    };

    class UniformBlock
    {
    public:
      enum class BlockType
      {
        Uniform,
        Storage,
        PushConstant
      };
      std::string name;
      int32_t binding;
      uint32_t size;
      vk::ShaderStageFlags stageFlags;
      BlockType type;
      std::vector<std::unique_ptr<Uniform>> uniforms;

      UniformBlock( const std::string& name, const uint32_t& binding, 
        const uint32_t& size, const vk::ShaderStageFlagBits& stageFlags, 
        const BlockType& type );

      UniformBlock(const UniformBlock&) = delete; 
      UniformBlock& operator=(UniformBlock&) = delete;

      void addUniform( Uniform* unif );

      Uniform* getUniform( const std::string& name );

      std::string dump( void ) const;
    };

    class ShaderProgram
    {
    public:
      std::string name;
      std::vector< std::unique_ptr< Uniform > > uniforms;
      std::vector< std::unique_ptr< UniformBlock > > uniformBlocks;
      std::vector< std::unique_ptr< VertexAttribute > > vertexAttribs;

      std::vector< vk::VertexInputAttributeDescription > attributeDescriptions;
      std::vector< DescriptorSetLayoutBinding > descriptors;
      std::vector<vk::PushConstantRange> pushConstantRange;

      explicit ShaderProgram( const std::string& name );
      ~ShaderProgram( void );

      ShaderProgram( const ShaderProgram& ) = delete;
      ShaderProgram& operator=( ShaderProgram& ) = delete;

      /*std::shared_ptr< ShaderModule >*/
      /*const std::vector<uint32_t>&*/
      std::vector<uint32_t> addShader( const std::string& source, 
        const vk::ShaderStageFlagBits& stage );

      void dump( void );

      void processShader( void );

    private:
      void autocatching( const glslang::TProgram& program, 
        const vk::ShaderStageFlagBits& stageFlag );
      void catchUniformBlock(const glslang::TProgram& program, 
        const vk::ShaderStageFlagBits& stageFlag, const uint32_t& i);
      void catchUniform(const glslang::TProgram& program, 
        const vk::ShaderStageFlagBits& stageFlag, const uint32_t& i);
      void catchVertexAttribute(const glslang::TProgram& program, 
        const vk::ShaderStageFlagBits& stageFlag, const uint32_t& i);
      TBuiltInResource getResources( void );
    };
  }
}