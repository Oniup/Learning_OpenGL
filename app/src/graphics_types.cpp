#include <app/graphics_types.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
  const char* shader_paths[] = { fragment_path, vertex_path };
  uint32_t shaders[] = { 0, 0 };
  for (int i = 0; i < 2; i++)
  {
    FILE* file = fopen(shader_paths[i], "rb");
    if (file == nullptr)
    {
      printf("failed to create shader [%s]\n", shader_paths[i]);
      exit(-1);
    }

    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* source = (char*)malloc(sizeof(char*) * length);
    fread(source, length, sizeof(char), file);
    source[length] = '\0';
    fclose(file);

#ifdef PRINT_SOURCE_DEBUG
    printf("\n[%s]:\n\n%s\n", shader_paths[i], source);
#endif

    uint32_t shader = glCreateShader(GL_FRAGMENT_SHADER + i);
    glShaderSource(shader, 1, (const char* const*)&source, nullptr);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
      char* message = (char*)malloc(sizeof(char*) * result);
      glGetShaderInfoLog(shader, result, &result, message);
      printf("failed to compile shader [%s]\n%s\n", shader_paths[i], message);
      exit(-1);
    }

    shaders[i] = shader;
  }

  uint32_t program = glCreateProgram();
  glAttachShader(program, shaders[0]);
  glAttachShader(program, shaders[1]);
  glLinkProgram(program);

  glDeleteShader(shaders[0]);
  glDeleteShader(shaders[1]);

  int result;
  glGetProgramiv(program, GL_LINK_STATUS, &result);
  if (!result)
  {
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &result);
    char* message = (char*)malloc(sizeof(char*) * result);
    glGetProgramInfoLog(program, result, &result, message);
    printf("failed to link shaders to shader program [%s, %s]\n%s\n", shader_paths[0], shader_paths[1], message);
    exit(-1);
  }

  _id = program;
}

Shader::~Shader()
{
  glDeleteProgram(_id);
  _id = 0;
}

void Shader::bind()
{
  glUseProgram(_id);
}

void Shader::unbind()
{
  glUseProgram(0);
}

Texture::Texture(const char* png_path, bool flip)
{
  FILE* file = fopen(png_path, "rb");
  if (file == nullptr)
  {
    printf("failed to create texture instance because image doesn't exist [%s]\n", png_path);
    exit(-1);
  }
  fclose(file);

  stbi_set_flip_vertically_on_load(flip);
  int channel;
  unsigned char* buffer = stbi_load(png_path, &_width, &_height, &channel, 4);

  if (buffer == nullptr)
  {
    printf("failed to create image buffer data [%s]\n", png_path);
    exit(-1);
  }

  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D, _id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGBA, _width, _height,
    0, GL_RGBA, GL_UNSIGNED_BYTE, buffer
  );
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(buffer);
}

Texture::~Texture()
{
  glDeleteTextures(1, &_id);

}

void Texture::bind(const int offset)
{
  glActiveTexture(GL_TEXTURE0 + offset);
  glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::unbind()
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

