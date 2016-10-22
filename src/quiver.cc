#include <fstream>

#include "argv.h"
#include "program.h"

int main(int argc, char** argv) {
  Argv args;
  args.Add(' ', "debug", "Show extensive debug information.");
  args.Add(' ', "dot", "Generate dot graph file.");
  args.Add(' ', "no-exec", "Don't execute the program.");
  args.Add(' ', "parsed", "Show the parsing debug information.");
  args.Add(' ', "stdin", "Read input from stdin.");

  args.Parse(argc, argv);
  if (args.Check("help")) {
    args.Help();
    return 0;
  }
  if (args.Rest().size() > 0 || args.Check("stdin")) {
    std::ifstream f;
    if (args.Rest().size() > 0) {
      f.open(args.Rest()[0]);
    }
    std::istream& input = args.Check("stdin") ? std::cin : f;
    if (!input) {
      std::cerr << "Failed to open input stream." << std::endl;
      return 1;
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input, line)) {
      lines.push_back(line);
    }
    Program p(&args, lines);
    if (args.Check("parsed")) {
      p.ShowParsed();
    }
    if (args.Check("dot")) {
      p.Dot();
      return 0;
    }
    if (args.Check("no-exec")) {
      return 0;
    }
    p.Execute();
    if (args.Rest().size() > 0) {
      f.close();
    }
  } else {
    args.Help();
  }
  return 0;
}
