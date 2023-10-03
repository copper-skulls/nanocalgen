#include "CalendarBuilder.hpp"
#include <fstream>

int main(int argc, char **argv) {
  std::vector<std::string> args(argv + 1, argv + argc);

  Config config;
  config.load_from_environment();
  config.load_from_cli_args(args);
  config.print();

  SVGRoot calendar = CalendarBuilder::from_config(config);
  std::ofstream output_file;
  output_file.open(config.output_filename());

  if (!output_file.good()) {
    output_file.close();
    throw std::invalid_argument("Failed to open output file for SVG " + config.output_filename() + ".");
  }

  calendar.print(output_file);
  output_file.close();
}
