#include "shader.h"

ShaderUPtr Shader::CreateFromFile(const std::string& filename, GLenum shaderType) {
    auto shader = ShaderUPtr(new Shader());
    if (!shader->LoadFile(filename, shaderType))
        return nullptr; // 실패? 메모리 해제
    return std::move(shader); // 성공할 경우 바깥으로 내보낼 방법 필요 -> move(소유권 이전)
}

Shader::~Shader(){
    if(m_shader){
        glDeleteShader(m_shader);
    }
}

bool Shader::LoadFile(const std::string& filename, GLenum shaderType) {
    auto result = LoadTextFile(filename);
    if (!result.has_value())
        return false;

    auto& code = result.value();
    const char* codePtr = code.c_str();
    int32_t codeLength = (int32_t)code.length();


    // create and compile shader
    m_shader = glCreateShader(shaderType);
    glShaderSource(m_shader, 1, (const GLchar* const*)&codePtr, &codeLength);
    glCompileShader(m_shader);

      // check compile error
    int success = 0;
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success); //iv ->int vector(pointer?)
    // shader에 있는 정보를 얻어오는 함수
    // Compile상태가 성공했는지 여부를? -> failure? success = 0; 

    if (!success) {
        char infoLog[1024]; // 얻어올 버퍼의 크기
        glGetShaderInfoLog(m_shader, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to compile shader: \"{}\"", filename);
        SPDLOG_ERROR("reason: {}", infoLog); // 실패한 이유 출력
        return false;
    }
    return true;
}