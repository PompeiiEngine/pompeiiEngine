#include "ShaderProgram.h"

#include <glslang/SPIRV/GlslangToSpv.h>

#include <sstream>

namespace pompeii
{
  namespace engine
  {
    Uniform::Uniform(const std::string& name_, const uint32_t& binding_, 
      const uint32_t& offset_, const uint32_t& size_,
      const int32_t& type_, const vk::ShaderStageFlagBits& stageFlags_)
      : name( name_ )
      , binding( binding_ )
      , offset( offset_ )
      , size( size_ )
      , type( type_ )
      , stageFlags( stageFlags_ )
    {
    }
    std::string Uniform::dump( void ) const
    {
      std::stringstream result;
      result << "Uniform(name: '" << name << "', binding: " 
        << binding << ", offset: " << offset << ", size: "
        << size << ", type: ";

      // https://github.com/KhronosGroup/glslang/blob/master/glslang/MachineIndependent/gl_types.h
      switch ( type )
      {
        case 0x8B5E: // GL_SAMPLER_2D
        case 0x904D: // GL_IMAGE_2D
        case 0x9108: // GL_SAMPLER_2D_MULTISAMPLE
        case 0x9055: // GL_IMAGE_2D_MULTISAMPLE
          result << "IMAGE 2D";
          break;
        case 0x8B60: // GL_SAMPLER_CUBE
        case 0x9050: // GL_IMAGE_CUBE
          break;
          result << "IMAGE CUBE";
        case 0x8B5F: // GL_SAMPLER_3D
        case 0x904E: // GL_IMAGE_3D
          result << "IMAGE 3D";
          break;

        case 0x8B5B: // GL_FLOAT_MAT3
          result << "MAT3";
          break;
        case 0x8B5C: // GL_FLOAT_MAT4
          result << "MAT4";
          break;
        default:
          result << "none";
          break;
      }

      result << ", stage: " << vk::to_string( stageFlags );

      result << ")";
      return result.str(  );
    }

    VertexAttribute::VertexAttribute( const std::string& name_, const uint32_t& location_,
      const uint32_t& size_, const int32_t& type_ )
      : name( name_ )
      , location( location_ )
      , size( size_ )
      , type( type_ )
    {
    }
    std::string VertexAttribute::dump( void ) const
    {
      std::stringstream result;
      result << "VertexAttribute(name: '" << name <<
        "', location: " << location <<
        ", size: " << size << ", type: ";

      switch( type )
      {
        case 0x1406: // GL_FLOAT
          result << "VK_FORMAT_R32_SFLOAT";
          break;
        case 0x8B50: // GL_FLOAT_VEC2
          result << "VK_FORMAT_R32G32_SFLOAT";
          break;
        case 0x8B51: // GL_FLOAT_VEC3
          result << "VK_FORMAT_R32G32B32_SFLOAT";
          break;
        case 0x8B52: // GL_FLOAT_VEC4
          result << "VK_FORMAT_R32G32B32A32_SFLOAT";
          break;
        case 0x1404: // GL_INT
          result << "VK_FORMAT_R32_SINT";
          break;
        case 0x8B53: // GL_INT_VEC2
          result << "VK_FORMAT_R32G32_SINT";
          break;
        case 0x8B54: // GL_INT_VEC3
          result << "VK_FORMAT_R32G32B32_SINT";
          break;
        case 0x8B55: // GL_INT_VEC4
          result << "VK_FORMAT_R32G32B32A32_SINT";
          break;
        case 0x1405: // GL_UNSIGNED_INT
          result << "VK_FORMAT_R32_SINT";
          break;
        case 0x8DC6: // GL_UNSIGNED_INT_VEC2
          result << "VK_FORMAT_R32G32_SINT";
          break;
        case 0x8DC7: // GL_UNSIGNED_INT_VEC3
          result << "VK_FORMAT_R32G32B32_SINT";
          break;
        case 0x8DC8: // GL_UNSIGNED_INT_VEC4
          result << "VK_FORMAT_R32G32B32A32_SINT";
          break;
        default:
          result << "VK_FORMAT_UNDEFINED";
          break;
      }

      result << ")";
      return result.str( );
    }

    UniformBlock::UniformBlock( const std::string& name_, const uint32_t& binding_ , 
      const uint32_t& size_, const vk::ShaderStageFlagBits& stageFlags_ , 
      const BlockType& type_ )
      : name( name_ )
      , binding( binding_ )
      , size( size_ )
      , stageFlags( stageFlags_ )
      , type( type_ )
    {
    }
    void UniformBlock::addUniform( Uniform* unif )
    {
      for ( const auto &u : uniforms )
      {
        if ( *u == *unif )
        {
          return;
        }
      }
      uniforms.emplace_back( unif );
    }
    Uniform* UniformBlock::getUniform( const std::string& name )
    {
      for ( const auto &uniform : uniforms )
      {
        if ( uniform->name == name )
        {
          return uniform.get( );
        }
      }

      return nullptr;
    }
    std::string UniformBlock::dump( void ) const
    {
      std::stringstream result;
      result << "UniformBlock(name: '" << name <<
        "', binding: " << binding << ", size: " <<
        size << ", type: ";

      switch (type)
      {
        case BlockType::Uniform:
          result << "BLOCK_UNIFORM";
          break;
        case BlockType::Storage:
          result << "BLOCK_STORAGE";
          break;
        case BlockType::PushConstant:
          result << "BLOCK_PUSH";
          break;
        default:
          break;
      }
      result << ", stage: " << vk::to_string( stageFlags );
      result << ")";

      return result.str( );
    }

    ShaderProgram::ShaderProgram( const std::string& name_ )
      : name( name_ )
    {
      #ifndef __ANDROID__
        glslang::InitializeProcess(); // TODO: Move to ctor of Engine
      #endif
    }
    ShaderProgram::~ShaderProgram( void )
    {
      #ifndef __ANDROID__
        glslang::FinalizeProcess(); // TODO: Move to dtor of Engine
      #endif
    }

    /*std::shared_ptr< ShaderModule >*/
    /*const std::vector<uint32_t>&*/
    std::vector<uint32_t> ShaderProgram::addShader( const std::string& source, 
      const vk::ShaderStageFlagBits& stage )
    {
      static const std::map<vk::ShaderStageFlagBits, EShLanguage> stageToLanguageMap
      {
        { vk::ShaderStageFlagBits::eVertex, EShLangVertex },
        { vk::ShaderStageFlagBits::eTessellationControl, EShLangTessControl },
        { vk::ShaderStageFlagBits::eTessellationEvaluation, EShLangTessEvaluation },
        { vk::ShaderStageFlagBits::eGeometry, EShLangGeometry },
        { vk::ShaderStageFlagBits::eFragment, EShLangFragment },
        { vk::ShaderStageFlagBits::eCompute, EShLangCompute }
      };

      std::map<vk::ShaderStageFlagBits, EShLanguage>::const_iterator stageIt = stageToLanguageMap.find(stage);
      assert( stageIt != stageToLanguageMap.end());

      // Enable SPIR-V and Vulkan rules when parsing GLSL
      EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);


      TBuiltInResource resource = getResources( );

      //shader.setEnvInput(glslang::EShSourceGlsl, stageIt->second, glslang::EShClientVulkan, 100);
      //shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
      //shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);

      glslang::TProgram program;

      glslang::TShader shader(stageIt->second);

      const char *shaderStrings[1];
      shaderStrings[0] = source.c_str();
      shader.setStrings(shaderStrings, 1);

      std::cout << "Parsing ... " << std::endl;
      if (!shader.parse(&resource, 100, false, messages))
      {
        std::string infoLog = shader.getInfoLog();
        std::string infoDebugLog = shader.getInfoDebugLog();
        std::cout << "____________ PARSE ____________" << std::endl;
        std::cout << "infoLog: " << infoLog << std::endl;
        std::cout << "infoDebugLog: " << infoDebugLog << std::endl;
        throw; //return false;
      }
      std::cout << " ... OK " << std::endl;

      program.addShader(&shader);

      std::cout << "Linking ... " << std::endl;
      if (!program.link(messages))
      {
        std::string infoLog = program.getInfoLog();
        std::string infoDebugLog = program.getInfoDebugLog();
        std::cout << "____________ LINK ____________" << std::endl;
        std::cout << "infoLog: " << infoLog << std::endl;
        std::cout << "infoDebugLog: " << infoDebugLog << std::endl;
        throw; //return false;
      }
      std::cout << " ... OK " << std::endl;

      program.buildReflection( );

      autocatching( program, stage );

      std::vector<uint32_t> spirv;
      glslang::GlslangToSpv(
        *program.getIntermediate(stageIt->second), spirv);

      return spirv; //return true;
    }

    void ShaderProgram::dump( void ) 
    {
      std::stringstream result;

      if (!vertexAttribs.empty())
      {
        result << "Vertex Attributes: \n";

        for (auto &vAttrib : vertexAttribs)
        {
          result << "  - " << vAttrib->dump() << "\n";
        }
      }

      if (!uniforms.empty())
      {
        result << "Uniforms: \n";

        for (auto &uniform : uniforms)
        {
          result << "  - " << uniform->dump() << "\n";
        }
      }

      if (!uniformBlocks.empty())
      {
        result << "Uniform Blocks: \n";

        for (auto &uniformBlock : uniformBlocks)
        {
          result << "  - " << uniformBlock->dump() << " \n";

          for (auto &uniform : uniformBlock->uniforms)
          {
            result << "\t  - " << uniform->dump() << " \n";
          }
        }
      }

      if ( !descriptors.empty( ) )
      {
        result << "Descriptors: \n";

        for ( const auto& d : descriptors )
        {
          result << "  - ";

          result << "binding: " << d.binding << ", type: " << vk::to_string( d.descriptorType )
            << ", stage: " << vk::to_string( d.stageFlags );

          result << " \n";
        }
      }

      if ( !attributeDescriptions.empty( ) )
      {
        result << "Vertex Input: \n";

        for ( const auto& att : attributeDescriptions )
        {
          result << "  - ";

          result << "location: " << att.location << ", binding: " << att.binding <<
          ", format: " << vk::to_string( att.format )
            << ", offset: " << att.offset;

          result << " \n";
        }
      }

      if ( !pushConstantRange.empty( ) )
      {
        result << "PushConstant: \n";

        for ( const auto& pcr : pushConstantRange )
        {
          result << "  - ";

          result << "stageFlags: " << vk::to_string( pcr.stageFlags ) << 
          ", offset: " << pcr.offset
            << ", size: " << pcr.size;

          result << " \n";
        }
      }

      std::cout << result.str( ) << std::endl;
    }

    void ShaderProgram::autocatching( const glslang::TProgram& program, 
      const vk::ShaderStageFlagBits& stageFlag )
    {
      for ( int32_t i = program.getNumLiveUniformBlocks( ) - 1; i >= 0; i-- )
      {
        catchUniformBlock(program, stageFlag, i);
      }

      for ( int32_t i = 0, l = program.getNumLiveUniformVariables( ); i < l; ++i )
      {
        catchUniform(program, stageFlag, i);
      }

      for ( int32_t i = 0, l = program.getNumLiveAttributes( ); i < l; ++i )
      {
        catchVertexAttribute(program, stageFlag, i);
      }
    }
    void ShaderProgram::catchUniformBlock(const glslang::TProgram& program, 
      const vk::ShaderStageFlagBits& stageFlag, const uint32_t& i)
    {
      for( auto& uniBlock: uniformBlocks )
      {
        if( uniBlock->name == program.getUniformBlockName( i ) )
        {
          uniBlock->stageFlags = uniBlock->stageFlags | stageFlag;
          return;
        }
      }

      UniformBlock::BlockType type = UniformBlock::BlockType::Uniform;

      if (strcmp(program.getUniformBlockTType(i)->getStorageQualifierString(), "buffer") == 0)
      {
        type = UniformBlock::BlockType::Storage;
      }
      else if (program.getUniformBlockTType(i)->getQualifier().layoutPushConstant)
      {
        type = UniformBlock::BlockType::PushConstant;
      }

      uniformBlocks.emplace_back(
        std::make_unique<UniformBlock>(program.getUniformBlockName(i), 
          program.getUniformBlockBinding(i), program.getUniformBlockSize(i), 
          stageFlag, type
        )
      );
    }
    void ShaderProgram::catchUniform(const glslang::TProgram& program, 
      const vk::ShaderStageFlagBits& stageFlag, const uint32_t& i)
    {
      if (program.getUniformBinding(i) == -1)
      {
        auto splitName = StringUtils::split_str(
          program.getUniformName(i), "."
        );

        if (splitName.size() == 2)
        {
          for (auto &uniformBlock : uniformBlocks)
          {
            if (uniformBlock->name == splitName.at(0))
            {
              uniformBlock->addUniform(new Uniform(
                splitName.at(1), program.getUniformBinding(i), 
                program.getUniformBufferOffset(i),
                1, // TODO HARDCODED (recreated later) sizeof(float) * program.getUniformTType(i)->computeNumComponents(),
                program.getUniformType(i), stageFlag)
              );
              return;
            }
          }
        }
      }

      for (auto &uniform : uniforms)
      {
        if (uniform->name == program.getUniformName(i))
        {
          uniform->stageFlags = (uniform->stageFlags | stageFlag);
          return;
        }
      }

      uniforms.emplace_back(std::make_unique<Uniform>(
        program.getUniformName(i), program.getUniformBinding(i), 
        program.getUniformBufferOffset(i), -1, program.getUniformType(i), stageFlag)
      );
    }
    void ShaderProgram::catchVertexAttribute(const glslang::TProgram& program, 
      const vk::ShaderStageFlagBits& stageFlag, const uint32_t& i)
    {
      for ( auto &vertexAttribute : vertexAttribs )
      {
        if (vertexAttribute->name == program.getAttributeName(i))
        {
          return;
        }
      }

      vertexAttribs.emplace_back(
        std::make_unique<VertexAttribute>(
          program.getAttributeName(i), 
          program.getAttributeTType(i)->getQualifier().layoutLocation, 
          sizeof(float) * program.getAttributeTType(i)->getVectorSize(), // TODO (CRISTIAN) MAAAL
          program.getAttributeType(i)
        )
      );
    }

    void ShaderProgram::processShader( void )
    {
      // Sort uniforms by binding
      std::sort(uniforms.begin(), uniforms.end(),
        [](const std::unique_ptr<Uniform> &l, const std::unique_ptr<Uniform> &r)
      {
        return l->binding < r->binding;
      });

      // Sort uniform blocks by binding.
      std::sort(uniformBlocks.begin(), uniformBlocks.end(),
        [](const std::unique_ptr<UniformBlock> &l, const std::unique_ptr<UniformBlock> &r)
      {
        return l->binding < r->binding;
      });

      // Sort uniform block uniforms by offsets.
      for (auto &uniformBlock : uniformBlocks)
      {
        std::sort(uniformBlock->uniforms.begin(), uniformBlock->uniforms.end(),
          [](const std::unique_ptr<Uniform> &l, const std::unique_ptr<Uniform> &r)
        {
          return l->offset < r->offset;
        });
      }




      // Fixing uniform size in uniform block
      for( auto& ub: uniformBlocks )
      {
        for( int i = 0; i < ub->uniforms.size( ); ++i )
        {
          if( i + 1 < ub->uniforms.size( ) )
          {
            ub->uniforms[ i ]->size = ub->uniforms[ i + 1 ]->offset - ub->uniforms[ i ]->offset;
          }
          if( i + 1 == ub->uniforms.size( ) )
          {
            ub->uniforms[ i ]->size = ub->size - ub->uniforms[ i ]->offset;
          }
        }
      }

      // Sort attributes by location.
      std::sort(vertexAttribs.begin(), vertexAttribs.end(),
        [](const std::unique_ptr<VertexAttribute> &l, const std::unique_ptr<VertexAttribute> &r)
      {
        return l->location < r->location;
      });


      for( const auto& ublock: uniformBlocks )
      {
        switch ( ublock->type )
        {
          case UniformBlock::BlockType::Uniform:
            descriptors.emplace_back( 
              DescriptorSetLayoutBinding( 
                ublock->binding, 
                vk::DescriptorType::eUniformBuffer, 
                ublock->stageFlags
              ) );
            break;
          case UniformBlock::BlockType::Storage:
            descriptors.emplace_back( 
              DescriptorSetLayoutBinding( 
                ublock->binding, 
                vk::DescriptorType::eStorageBuffer, 
                ublock->stageFlags
              ) );
            break;
          case UniformBlock::BlockType::PushConstant:
            uint32_t currentOffset = 0;
            for ( const auto &ub : uniformBlocks )
            {
              if ( ub->type != UniformBlock::BlockType::PushConstant)
              {
                continue;
              }

              vk::PushConstantRange pcr;
              pcr.stageFlags = ub->stageFlags;
              pcr.offset = currentOffset;
              pcr.size = static_cast<uint32_t>(ub->size);

              pushConstantRange.emplace_back(pcr);

              currentOffset += pcr.size;
            }
            break;
        }
      }

      for( const auto& ubo: uniforms )
      {
        switch( ubo->type )
        {
          case 0x8B5E: // GL_SAMPLER_2D
          case 0x904D: // GL_IMAGE_2D
          case 0x9108: // GL_SAMPLER_2D_MULTISAMPLE
          case 0x9055: // GL_IMAGE_2D_MULTISAMPLE
            //result << "IMAGE 2D";
            descriptors.emplace_back( 
              DescriptorSetLayoutBinding( 
                ubo->binding, 
                vk::DescriptorType::eCombinedImageSampler, 
                ubo->stageFlags
              ) );
            break;
          case 0x8B60: // GL_SAMPLER_CUBE
          case 0x9050: // GL_IMAGE_CUBE
            //result << "IMAGE CUBE";
            descriptors.emplace_back( 
              DescriptorSetLayoutBinding( 
                ubo->binding, 
                vk::DescriptorType::eCombinedImageSampler, 
                ubo->stageFlags
              ) );
            break;
          case 0x8B5F: // GL_SAMPLER_3D
          case 0x904E: // GL_IMAGE_3D
            //result << "IMAGE 3D";
            descriptors.emplace_back( 
              DescriptorSetLayoutBinding( 
                ubo->binding, 
                vk::DescriptorType::eCombinedImageSampler, 
                ubo->stageFlags
              ) );
            break;
          default:
            break;
        }
      }
      
      attributeDescriptions.reserve( vertexAttribs.size( ) );
      uint32_t currentOffset = 0;
      for( auto &va: vertexAttribs )
      {
        vk::Format valFormat = vk::Format::eUndefined;
        switch( va->type )
          {
            case 0x1406: // GL_FLOAT
              valFormat = vk::Format::eR32Sfloat;
              break;
            case 0x8B50: // GL_FLOAT_VEC2
              valFormat = vk::Format::eR32G32Sfloat;
              break;
            case 0x8B51: // GL_FLOAT_VEC3
              valFormat = vk::Format::eR32G32B32Sfloat;
              break;
            case 0x8B52: // GL_FLOAT_VEC4
              valFormat = vk::Format::eR32G32B32A32Sfloat;
              break;
            case 0x1404: // GL_INT
              valFormat = vk::Format::eR32Sint;
              break;
            case 0x8B53: // GL_INT_VEC2
              valFormat = vk::Format::eR32G32Sint;
              break;
            case 0x8B54: // GL_INT_VEC3
              valFormat = vk::Format::eR32G32B32Sint;
              break;
            case 0x8B55: // GL_INT_VEC4
              valFormat = vk::Format::eR32G32B32A32Sint;
              break;
            case 0x1405: // GL_UNSIGNED_INT
              valFormat = vk::Format::eR32Uint;
              break;
            case 0x8DC6: // GL_UNSIGNED_INT_VEC2
              valFormat = vk::Format::eR32G32Uint;
              break;
            case 0x8DC7: // GL_UNSIGNED_INT_VEC3
              valFormat = vk::Format::eR32G32B32Uint;
              break;
            case 0x8DC8: // GL_UNSIGNED_INT_VEC4
              valFormat = vk::Format::eR32G32B32A32Uint;
              break;
            default:
              break;
          }
        ;
        attributeDescriptions.emplace_back( 
          vk::VertexInputAttributeDescription( 
            va->location, 0 /* TODO: Not accepted instancing */,
            valFormat, currentOffset
          )
        );

        currentOffset += va->size;
      }
    }

    TBuiltInResource ShaderProgram::getResources( void )
    {
      TBuiltInResource resources = {};
      resources.maxLights = 32;
      resources.maxClipPlanes = 6;
      resources.maxTextureUnits = 32;
      resources.maxTextureCoords = 32;
      resources.maxVertexAttribs = 64;
      resources.maxVertexUniformComponents = 4096;
      resources.maxVaryingFloats = 64;
      resources.maxVertexTextureImageUnits = 32;
      resources.maxCombinedTextureImageUnits = 80;
      resources.maxTextureImageUnits = 32;
      resources.maxFragmentUniformComponents = 4096;
      resources.maxDrawBuffers = 32;
      resources.maxVertexUniformVectors = 128;
      resources.maxVaryingVectors = 8;
      resources.maxFragmentUniformVectors = 16;
      resources.maxVertexOutputVectors = 16;
      resources.maxFragmentInputVectors = 15;
      resources.minProgramTexelOffset = -8;
      resources.maxProgramTexelOffset = 7;
      resources.maxClipDistances = 8;
      resources.maxComputeWorkGroupCountX = 65535;
      resources.maxComputeWorkGroupCountY = 65535;
      resources.maxComputeWorkGroupCountZ = 65535;
      resources.maxComputeWorkGroupSizeX = 1024;
      resources.maxComputeWorkGroupSizeY = 1024;
      resources.maxComputeWorkGroupSizeZ = 64;
      resources.maxComputeUniformComponents = 1024;
      resources.maxComputeTextureImageUnits = 16;
      resources.maxComputeImageUniforms = 8;
      resources.maxComputeAtomicCounters = 8;
      resources.maxComputeAtomicCounterBuffers = 1;
      resources.maxVaryingComponents = 60;
      resources.maxVertexOutputComponents = 64;
      resources.maxGeometryInputComponents = 64;
      resources.maxGeometryOutputComponents = 128;
      resources.maxFragmentInputComponents = 128;
      resources.maxImageUnits = 8;
      resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
      resources.maxCombinedShaderOutputResources = 8;
      resources.maxImageSamples = 0;
      resources.maxVertexImageUniforms = 0;
      resources.maxTessControlImageUniforms = 0;
      resources.maxTessEvaluationImageUniforms = 0;
      resources.maxGeometryImageUniforms = 0;
      resources.maxFragmentImageUniforms = 8;
      resources.maxCombinedImageUniforms = 8;
      resources.maxGeometryTextureImageUnits = 16;
      resources.maxGeometryOutputVertices = 256;
      resources.maxGeometryTotalOutputComponents = 1024;
      resources.maxGeometryUniformComponents = 1024;
      resources.maxGeometryVaryingComponents = 64;
      resources.maxTessControlInputComponents = 128;
      resources.maxTessControlOutputComponents = 128;
      resources.maxTessControlTextureImageUnits = 16;
      resources.maxTessControlUniformComponents = 1024;
      resources.maxTessControlTotalOutputComponents = 4096;
      resources.maxTessEvaluationInputComponents = 128;
      resources.maxTessEvaluationOutputComponents = 128;
      resources.maxTessEvaluationTextureImageUnits = 16;
      resources.maxTessEvaluationUniformComponents = 1024;
      resources.maxTessPatchComponents = 120;
      resources.maxPatchVertices = 32;
      resources.maxTessGenLevel = 64;
      resources.maxViewports = 16;
      resources.maxVertexAtomicCounters = 0;
      resources.maxTessControlAtomicCounters = 0;
      resources.maxTessEvaluationAtomicCounters = 0;
      resources.maxGeometryAtomicCounters = 0;
      resources.maxFragmentAtomicCounters = 8;
      resources.maxCombinedAtomicCounters = 8;
      resources.maxAtomicCounterBindings = 1;
      resources.maxVertexAtomicCounterBuffers = 0;
      resources.maxTessControlAtomicCounterBuffers = 0;
      resources.maxTessEvaluationAtomicCounterBuffers = 0;
      resources.maxGeometryAtomicCounterBuffers = 0;
      resources.maxFragmentAtomicCounterBuffers = 1;
      resources.maxCombinedAtomicCounterBuffers = 1;
      resources.maxAtomicCounterBufferSize = 16384;
      resources.maxTransformFeedbackBuffers = 4;
      resources.maxTransformFeedbackInterleavedComponents = 64;
      resources.maxCullDistances = 8;
      resources.maxCombinedClipAndCullDistances = 8;
      resources.maxSamples = 4;
      resources.limits.nonInductiveForLoops = true;
      resources.limits.whileLoops = true;
      resources.limits.doWhileLoops = true;
      resources.limits.generalUniformIndexing = true;
      resources.limits.generalAttributeMatrixVectorIndexing = true;
      resources.limits.generalVaryingIndexing = true;
      resources.limits.generalSamplerIndexing = true;
      resources.limits.generalVariableIndexing = true;
      resources.limits.generalConstantMatrixVectorIndexing = true;
      return resources;
    }
  }
}