#pragma once
#include <memory>
#include "Core/Log.h"

namespace Greenbox {

    // Shader
    enum class ShaderDataType
    {
        None = 0, 
        Float, 
        Float2, 
        Float3, 
        Float4, 
        Mat3, 
        Mat4, 
        Int, 
        Int2, 
        Int3, 
        Int4, 
        Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:     return 4;
        case ShaderDataType::Float2:    return 4 * 2;
        case ShaderDataType::Float3:    return 4 * 3;
        case ShaderDataType::Float4:    return 4 * 4;
        case ShaderDataType::Mat3:      return 4 * 3 * 3;
        case ShaderDataType::Mat4:      return 4 * 4 * 4;
        case ShaderDataType::Int:       return 4;
        case ShaderDataType::Int2:      return 4 * 2;
        case ShaderDataType::Int3:      return 4 * 3;
        case ShaderDataType::Int4:      return 4 * 4;
        case ShaderDataType::Bool:      return 1;
        }

        GB_ASSERT(false, "Unknown ShaderDataType");
        return 0;
    }

    static uint32_t GetComponentCount(ShaderDataType Type)
    {
        switch (Type)
        {
        case ShaderDataType::Float:     return 1;
        case ShaderDataType::Float2:    return 2;
        case ShaderDataType::Float3:    return 3;
        case ShaderDataType::Float4:    return 4;
        case ShaderDataType::Mat3:      return 3 * 3;
        case ShaderDataType::Mat4:      return 4 * 4;
        case ShaderDataType::Int:       return 1;
        case ShaderDataType::Int2:      return 2;
        case ShaderDataType::Int3:      return 3;
        case ShaderDataType::Int4:      return 4;
        case ShaderDataType::Bool:      return 1;
        }

        GB_ASSERT(false, "Unknown ShaderDataType");
        return 0;
    }


    // Texture
    enum class TextureDataType
    {
        None,

        // Color
        RGB,
        RGB8,
        RGBA,
        RGBA8,
        R32I,
        RED_INTEGER,

        // Depth
        DEPTH24_STENCIL8,
    };

    static GLenum OpenGLDataType(TextureDataType dataType)
    {
        switch (dataType)
        {
        case TextureDataType::RGB: return GL_RGB;
        case TextureDataType::RGB8: return GL_RGB8;
        case TextureDataType::RGBA: return GL_RGBA;
        case TextureDataType::RGBA8: return GL_RGBA8;
        case TextureDataType::R32I: return GL_R32I;
        case TextureDataType::RED_INTEGER: return GL_RED_INTEGER;
        case TextureDataType::DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
        }

        GB_ASSERT(false, "Greenbox doesn't support this dataType");
        return 0;
    }
}