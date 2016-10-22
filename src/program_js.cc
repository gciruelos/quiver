#include <cstdint>
#include <sstream>
#include <vector>
#include <string>
#include <emscripten.h>

#include "program.h"
#include "argv.h"

extern "C" {

EMSCRIPTEN_KEEPALIVE
void runProgramFromString(char* input, char* output, size_t buffer_size) {
  Argv argv;
  std::stringstream ss;
  ss << input;
  std::string line;
  std::vector<std::string> lines;
  while (std::getline(ss, line)) {
    lines.push_back(line);
  }
  std::stringstream buffer;

  Program p(&argv, lines);
  p.Execute(buffer);

  std::string text = buffer.str();
  for (size_t i = 0; i < text.length() && i < buffer_size; i++) {
    output[i] = text[i];
  }
}

}
