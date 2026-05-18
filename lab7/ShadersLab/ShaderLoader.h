#pragma once
#include "pch.h"
#include "ShaderBase.h"

class ShaderLoader
{
public:
	ShaderLoader(void) {}
	~ShaderLoader(void) {}

	// Выполняем загрузку исходного кода шейдера из файла
	GLuint LoadShader(
		GLenum shaderType,
		const wchar_t* fileName,
		GLuint shaderId = 0)
	{
		std::ifstream stream(fileName);
		if (!stream.is_open())
		{
			throw std::runtime_error("Can't open shader file");
		}
		return LoadShader(shaderType, stream, shaderId);
	}

	GLuint LoadShader(
		GLenum shaderType,
		const char* fileName,
		GLuint shaderId = 0)
	{
		std::ifstream stream(fileName);
		if (!stream.is_open())
		{
			throw std::runtime_error("Can't open shader file");
		}

		return LoadShader(shaderType, stream, shaderId);
	}

	// Выполняем загрузку исходного кода шейдера из входного потока
	GLuint LoadShader(
		GLenum shaderType,
		std::istream& stream,
		GLuint shaderId = 0)
	{
		// Копируем данные из файлового потока в строковый поток
		std::stringstream strStream;
		strStream << stream.rdbuf();

		// Получаем строку, содержащую исходный код шейдера
		// и загружаем шейдер из нее
		return LoadShaderFromString(shaderType, strStream.str(), shaderId);
	}

	// Выполняем загрузку исходного кода шейдера из строки
	GLuint LoadShaderFromString(
		GLenum shaderType,
		std::string const& source,
		GLuint shaderId = 0
	)
	{
		Shader shader(shaderId);
		// Создаем новый шейдер
		// (если в нам был передан нулевой идентификатор шейдера)
		if (!shader)
		{
			shader.Create(shaderType);
		}
		shader.SetSource(source.c_str());

		// Возвращаем идентификатор шейдерного объекта
		return shader.Detach();
	}
};