#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/scene.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int ID;

	Shader() = default;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	// use/activate the shader
	void use();
	void clear();

	// utility functions for setting uniforms
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void set4Float(const std::string& name, float v1, float v2, float v3, float v4) const;
	void set4Float(const std::string& name, aiColor4D color) const;
	void set4Float(const std::string& name, glm::vec4 v) const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec2(const std::string &name, glm::vec2 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec3(const std::string &name, glm::vec3 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w) const;
	void setVec4(const std::string &name, glm::vec4 &value) const;
	void setMat2(const std::string &name, glm::mat2 &value) const;
	void setMat3(const std::string &name, glm::mat3 &value) const;
	void setMat4(const std::string &name, glm::mat4 &value) const;

	~Shader();
private:
	void checkCompileErrors(unsigned int shader, std::string type);
};