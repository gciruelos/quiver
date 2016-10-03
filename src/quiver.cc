#include "argv.h"
#include "program.h"

int main(int argc, char** argv) {
  Argv args;
  args.Add(' ', "debug", "Show extensive debug information.");
  args.Add(' ', "dot", "Generate dot graph file.");
  args.Add(' ', "parsed", "Show the parsing debug information.");

  args.Parse(argc, argv);
  if (args.Check("help")) {
    args.Help();
    return 0;
  }
  if (args.Rest().size() > 0) {
    Program p(&args);
    if (args.Check("parsed")) {
      p.ShowParsed();
    }
    if (args.Check("dot")) {
      p.Dot();
      return 0;
    }
    p.Execute();
  } else {
    args.Help();
  }
  return 0;
}
