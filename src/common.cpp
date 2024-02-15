#include "common.h"
#include <fstream>
#include <sstream>

std::optional<std::string> LoadTextFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        SPDLOG_ERROR("failed to open file: {}", filename);
        return {};
    }
    std::stringstream text; 
    text << fin.rdbuf(); // stringstream에 file을 때려 넣은 다음에
    return text.str(); // string으로 리턴 -> 그러면 optional type으로 감싸져서 리턴되는 것
} 