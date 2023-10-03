#include "Config.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>

enum class Config::IntOptions {
  width, height,
  cell_width, cell_height, cell_stroke_width, cell_roundness,
  monday_cell_offset, day_number, first_month_day, margins,
  text_offset_x, text_offset_y, font_size, goal_wordcount
}; // options that take int arguments

enum class Config::StringOptions {
  cell_primary_color, cell_secondary_color, month,
  text_baseline, text_color, font_family, font_weight,
  goal_type, goal_rounding, output_filename
}; // options that tage string arguments

const std::map<std::string, Config::IntOptions> Config::ENV_TO_INT_OPTION = {
  {"NCG_WIDTH", Config::IntOptions::width}, {"NCG_HEIGHT", Config::IntOptions::height},
  {"NCG_CELL_WIDTH", Config::IntOptions::cell_width}, {"NCG_CELL_HEIGHT", Config::IntOptions::cell_height},
  {"NCG_CELL_STROKE_WIDTH", Config::IntOptions::cell_stroke_width}, {"NCG_CELL_ROUNDNESS", Config::IntOptions::cell_roundness},
  {"NCG_MONDAY_CELL_OFFSET", Config::IntOptions::monday_cell_offset}, {"NCG_DAY_NUMBER", Config::IntOptions::day_number},
  {"NCG_FIRST_MONTH_DAY", Config::IntOptions::first_month_day}, {"NCG_MARGINS", Config::IntOptions::margins},
  {"NCG_TEXT_OFFSET_X", Config::IntOptions::text_offset_x}, {"NCG_TEXT_OFFSET_Y", Config::IntOptions::text_offset_y},
  {"NCG_FONT_SIZE", Config::IntOptions::font_size}, {"NCG_GOAL_WORDCOUNT", Config::IntOptions::goal_wordcount}
}; // mapping from environment variable names to corresponding int-taking options

const std::map<std::string, Config::StringOptions> Config::ENV_TO_STRING_OPTION = {
  {"NCG_CELL_PRIMARY_COLOR", Config::StringOptions::cell_primary_color}, {"NCG_CELL_SECONDARY_COLOR", Config::StringOptions::cell_secondary_color},
  {"NCG_MONTH", Config::StringOptions::month}, {"NCG_TEXT_BASELINE", Config::StringOptions::text_baseline},
  {"NCG_TEXT_COLOR", Config::StringOptions::text_color}, {"NCG_FONT_FAMILY", Config::StringOptions::font_family},
  {"NCG_FONT_WEIGHT", Config::StringOptions::font_weight}, {"NCG_GOAL_TYPE", Config::StringOptions::goal_type},
  {"NCG_GOAL_ROUNDING", Config::StringOptions::goal_rounding}, {"NCG_OUTPUT_FILENAME", Config::StringOptions::output_filename}
}; // mapping from environment variable names to corresponding string-taking options

const std::map<std::string, Config::IntOptions> Config::FILE_TO_INT_OPTION = {
  {"width", Config::IntOptions::width}, {"height", Config::IntOptions::height},
  {"cell-width", Config::IntOptions::cell_width}, {"cell-height", Config::IntOptions::cell_height},
  {"cell-stroke-width", Config::IntOptions::cell_stroke_width}, {"cell-roundness", Config::IntOptions::cell_roundness},
  {"monday-cell-offset", Config::IntOptions::monday_cell_offset}, {"day-number", Config::IntOptions::day_number},
  {"first-month-day", Config::IntOptions::first_month_day}, {"margins", Config::IntOptions::margins},
  {"text-offset-x", Config::IntOptions::text_offset_x}, {"text-offset-y", Config::IntOptions::text_offset_y},
  {"font-size", Config::IntOptions::font_size}, {"goal-wordcount", Config::IntOptions::goal_wordcount}
}; // mapping from file/CLI options to correcponding int-taking options

const std::map<std::string, Config::StringOptions> Config::FILE_TO_STRING_OPTION = {
  {"cell-primary-color", Config::StringOptions::cell_primary_color}, {"cell-secondary-color", Config::StringOptions::cell_secondary_color},
  {"month", Config::StringOptions::month}, {"text-baseline", Config::StringOptions::text_baseline},
  {"text-color", Config::StringOptions::text_color}, {"font-family", Config::StringOptions::font_family},
  {"font-weight", Config::StringOptions::font_weight}, {"goal-type", Config::StringOptions::goal_type},
  {"goal-rounding", Config::StringOptions::goal_rounding}, {"output-filename", Config::StringOptions::output_filename}
}; // mapping from file/CLI options to corresponding string-taking options


inline int Config::width() const { return width_; } // document width
inline int Config::height() const { return height_; }; // document height

inline const std::string &Config::cell_primary_color() const { return cell_primary_color_; };
// color (arbitrary string) used to fill in a primary calendar cell
inline const std::string &Config::cell_secondary_color() const { return cell_secondary_color_; };
// color (arbitrary string) used to fill in a secondary calendar cell and stroke primary calendar cell
inline int Config::cell_stroke_width() const { return cell_stroke_width_; };
// stroke width for primary cell
inline int Config::cell_roundness() const { return cell_roundness_; };
// `ry` and `rx` value for calendar cells

inline int Config::day_number() const { return day_number_; }; // number of days in calendar
inline int Config::monday_cell_offset() const { return monday_cell_offset_; }; // monday cell shift to the right (0-6)
inline int Config::first_month_day() const { return first_month_day_; }; // first day of month (1-MON, 7-SUN)
inline int Config::margins() const { return margins_; }; // space between calendar cells (inbetweeen AND document border)

inline int Config::text_offset_x() const { return text_offset_x_; };
// space between calendar cell border and its labels (X)
inline int Config::text_offset_y() const { return text_offset_y_; };
// space between calendar cell border and its label (Y)
inline const std::string &Config::text_baseline() const { return text_baseline_; };
// dominant-baseline (arbitrary string) used for date labels of cells
inline const std::string &Config::text_color() const { return text_color_; };
// color (arbitrary string) used for all labels
inline const std::string &Config::font_family() const { return font_family_; };
// font family (arbitrary string) used by all labels
inline const std::string &Config::font_weight() const { return font_weight_; };
// font weight (arbitrary string) used by all labels
inline int Config::font_size() const { return font_size_; };
// font size used by all labels

inline int Config::goal_wordcount() const { return goal_wordcount_; };
// wordcount aimed at to be reached on the last day of the calendar
inline GoalType Config::goal_type() const { return goal_type_; };
// goal type deciding the words-per-day distribution
inline GoalRounding Config::goal_rounding() const { return goal_rounding_; };
// rounding behaviour when calculating words-per-day distribution 

inline const std::string &Config::output_filename() const { return output_filename_; };
// filename where to write the document

Config::Config()
// default constructor with reasonable defaults
// (defaults should match those documented in `README.md`)
  : width_(1400), height_(1000), cell_primary_color_("white"), cell_secondary_color_("black"),
    cell_stroke_width_(5), cell_roundness_(10), day_number_(31), monday_cell_offset_(0),
    first_month_day_(2), margins_(10), text_offset_x_(10), text_offset_y_(10),
    text_baseline_("hanging"), text_color_("black"), font_family_("FreeMono"), font_weight_("bold"),
    font_size_(30), goal_wordcount_(50000), goal_type_(GoalType::regular_c),
    goal_rounding_(GoalRounding::up), output_filename_("out.svg") {}

void Config::print(std::ostream &os) {
  // Prints config to `os`
  // Meant to be human-readable and for debugging.
  // Shouldn't be used for machine parsing - `goal_type` and `goal_rounding`
  // are handled by casting to int, and may change with changes in
  // implementation easily.
  os << "CONFIG:" << std::endl;
  os << "=======" << std::endl;
  os << "width: " << width() << std::endl;
  os << "height: " << height() << std::endl;
  os << "cell_primary_color: " << cell_primary_color() << std::endl;
  os << "cell_secondary_color: " << cell_secondary_color() << std::endl;
  os << "cell_stroke_width: " << cell_stroke_width() << std::endl;
  os << "cell_roundness: " << cell_roundness() << std::endl;
  os << "day_number: " << day_number() << std::endl;
  os << "monday_cell_offset: " << monday_cell_offset() << std::endl;
  os << "first_month_day: " << first_month_day() << std::endl;
  os << "margins: " << margins() << std::endl;
  os << "text_offset_x: " << text_offset_x() << std::endl;
  os << "text_offset_y: " << text_offset_y() << std::endl;
  os << "text_baseline: " << text_baseline() << std::endl;
  os << "text_color: " << text_color() << std::endl;
  os << "font_family: " << font_family() << std::endl;
  os << "font_weight: " << font_weight() << std::endl;
  os << "font_size: " << font_size() << std::endl;
  os << "goal_wordcount: " << goal_wordcount() << std::endl;
  os << "goal_type: " << (int)goal_type() << std::endl;
  os << "goal_rounding: " << (int)goal_rounding() << std::endl;
  os << "output_filename: " << output_filename() << std::endl;
}

void Config::load_from_environment() {
  // Load configuration values from environment variables,
  // overwrite what is already present.
  // Throw (pass along): `std::invalid_argument` if any option was not in correct format (not int, out-of-bounds...)
  char *loaded_value;

  for (auto const &keyval : ENV_TO_INT_OPTION) {
    loaded_value = std::getenv(keyval.first.c_str());
    if (loaded_value != nullptr) {
      set_int_option_master(keyval.second, loaded_value);
    }
  }

  for (auto const &keyval : ENV_TO_STRING_OPTION) {
    loaded_value = std::getenv(keyval.first.c_str());
    if (loaded_value != nullptr) {
      set_string_option_master(keyval.second, loaded_value);
    }
  }
}

void Config::load_from_cli_args(const std::vector<std::string> &args) {
  // Load configuration values from CLI-format arguments (`--[option] [value]`).
  // overwrite what is already present.
  // Throw: `std::invalid_argument` if any option not recognized.
  // Throw: `std::invalid_argument` if value is missing for an option.
  // Throw (pass along): `std::invalid_argument` if any option was not in correct format (not int, out-of-bounds...)
  std::string current_arg = "";
  bool expecting_value = false;

  for (int i=0; i < args.size(); ++i) {

    if (expecting_value) {
      expecting_value = false;

      if (FILE_TO_INT_OPTION.contains(current_arg)) {
        set_int_option_master(FILE_TO_INT_OPTION.at(current_arg), args[i]);
      } else if (FILE_TO_STRING_OPTION.contains(current_arg)) {
        set_string_option_master(FILE_TO_STRING_OPTION.at(current_arg), args[i]);
      } else if (current_arg == "config") {
        load_from_file(args[i]);
      } else {
        throw std::invalid_argument("Unrecognized CLI option: " + current_arg + ".");
      }

    } else {
      expecting_value = true;

      if (args[i].size() <= 2) {
        throw std::invalid_argument("CLI option too short.");
      } else {
        current_arg = args[i].substr(2);
      }
    }
  }

  if (expecting_value) {
    throw std::invalid_argument("Did not provide expected value for the option " + current_arg + ".");
  }
}

void Config::load_from_file(const std::string &filename) {
  // Load configuration values from file. (format: one per line, `[option] [value]`)
  // overwrite what is already present.
  // Throw: `std::invalid_argument` if `filename` failed to open.
  // Throw (pass along): `std::invalid_argument` if any option not recognized.
  // Throw (pass along): `std::invalid_argument` if any option was not in correct format (not int, out-of-bounds...)

  std::ifstream input_file;
  input_file.open(filename);

  if (!input_file.good()) {
    input_file.close();
    throw std::invalid_argument("Failed to open file to read options from: " + filename + ".");
  }

  load_from_file(input_file);
  input_file.close();
}

void Config::load_from_file(std::istream &is) {
  // Load configuration values from file. (format: one per line, `[option] [value]`)
  // overwrite what is already present.
  // Throw: `std::invalid_argument` if `[option]` not recognized.
  // Throw (pass along): `std::invalid_argument` if any option was not in correct format (not int, out-of-bounds...)
  std::string current_line;
  while (std::getline(is, current_line)) {
    std::istringstream current_line_stream(current_line);
    std::string arg, val;
    std::getline(current_line_stream, arg, ' ');
    std::getline(current_line_stream, val);

    if (FILE_TO_INT_OPTION.contains(arg)) {
      set_int_option_master(FILE_TO_INT_OPTION.at(arg), val);
    } else if (FILE_TO_STRING_OPTION.contains(arg)) {
      set_string_option_master(FILE_TO_STRING_OPTION.at(arg), val);
    } else {
      throw std::invalid_argument("Unrecognized file option: " + arg + ".");
    }
  }
}

void Config::set_int_option_master(Config::IntOptions option, const std::string &val) {
  // Set `option` to int value of `string`, if in bounds of given option.
  // Throw (pass along): `std::invalid_argument` if `val` couldn't be properly parsed into an int.
  // Throw (pass along): `std::invalid_argument` if out of bonds for given option.
  // Throw: `std::logic_error` if given enum case hasn't been implemented (THIS WOULD BE A BUG.)
  int converted_val = ConfigParsingUtils::get_int(val);
  switch (option) {
    case (Config::IntOptions::width): 
      width_ = converted_val;
      break;
    case (Config::IntOptions::height):
      height_ = converted_val;
      break;
    case (Config::IntOptions::cell_width):
      set_cell_width(converted_val);
      break;
    case (Config::IntOptions::cell_height):
      set_cell_height(converted_val);
      break;
    case (Config::IntOptions::cell_stroke_width):
      cell_stroke_width_ = converted_val;
      break;
    case (Config::IntOptions::cell_roundness):
      cell_roundness_ = converted_val;
      break;
    case (Config::IntOptions::monday_cell_offset):
      set_monday_cell_offset(converted_val);
      break;
    case (Config::IntOptions::day_number):
      day_number_ = converted_val;
      break;
    case (Config::IntOptions::first_month_day):
      set_first_month_day(converted_val);
      break;
    case (Config::IntOptions::margins):
      margins_ = converted_val;
      break;
    case (Config::IntOptions::text_offset_x):
      text_offset_x_ = converted_val;
      break;
    case (Config::IntOptions::text_offset_y):
      text_offset_y_ = converted_val;
      break;
    case (Config::IntOptions::font_size):
      font_size_ = converted_val;
      break;
    case (Config::IntOptions::goal_wordcount):
      goal_wordcount_ = converted_val;
      break;
    default:
      throw std::logic_error("Int option not implemented.");
      break;
  }
}

void Config::set_string_option_master(Config::StringOptions option, const std::string &val) {
  // Set `option` to int value of `string`, if in correct format.
  // Note: most options accept arbitrary strings without checking.
  // Throw (pass along): `std::invalid_argument` if `val` is not in correct format.
  // Throw: `std::logic_error` if given enum case hasn't been implemented (THIS WOULD BE A BUG.)
  switch(option) {
    case (Config::StringOptions::cell_primary_color):
      cell_primary_color_ = val;
      break;
    case (Config::StringOptions::cell_secondary_color):
      cell_secondary_color_ = val;
      break;
    case (Config::StringOptions::month):
      set_month(val);
      break;
    case (Config::StringOptions::text_baseline):
      text_baseline_ = val;
      break;
    case (Config::StringOptions::text_color):
      text_color_ = val;
      break;
    case (Config::StringOptions::font_family):
      font_family_ = val;
      break;
    case (Config::StringOptions::font_weight):
      font_weight_ = val;
      break;
    case (Config::StringOptions::goal_type):
      goal_type_ = ConfigParsingUtils::get_goal_type(val);
      break;
    case (Config::StringOptions::goal_rounding):
      goal_rounding_ =  ConfigParsingUtils::get_goal_rounding(val);
      break;
    case (Config::StringOptions::output_filename):
      output_filename_ = val;
      break;
    default:
      throw std::logic_error("String option not implemented.");
      break;
  }
}

void Config::set_cell_width(int val) {
  // Set document width given cell width `val` and current config state.
  // 7 cells, 6 gaps between them, and 2 gaps at the end and beginning of the document
  width_ = (7 * val) + (6 * margins_) + (2 * margins_);
};

void Config::set_cell_height(int val) {
  // Set document height given cell height `val` and current config state.
  int row_count = ((monday_cell_offset_ + first_month_day_ - 1) % 7) + day_number_;
  row_count = (row_count % 7 == 0) ? row_count / 7 : (row_count / 7) + 1;
  height_ = (row_count * val) + ((row_count - 1) * margins_) + (2* margins_);
}

void Config::set_monday_cell_offset(int val) {
  // Set monday cell offset.
  // Throw: `std::invalid_argument` if `val` not in [0-6].
  if (val < 0 || val > 6) {
    throw std::invalid_argument("Value given for monday-cell-offset out of range ([0-6]).");
  } else {
    monday_cell_offset_ = val;
  }
}

void Config::set_first_month_day(int val) {
  // Set first month day (1-MON, 7-SUN)
  // Throw: `std::invalid_argument` if `val` not in [1-7].
  if (val < 1 || val > 7) {
    throw std::invalid_argument("Value given for first-month-day out of range ([1-7]).");
  } else {
    first_month_day_ = val;
  }
}

void Config::set_month(const std::string &val) {
  // Set first month day, day_number given `val` in format `YYYY-MM`
  // Throw (pass along): `std::invalid_argument` if `val` in invalid
  // format or month is out of range.
  // Throw: `std::logic_error` if this function tries to get number
  // of days for month that is not [1-12]. THIS WOULD BE A BUG,
  // as `get_year_month` is already making a bounds check.
  int year, month;
  ConfigParsingUtils::get_year_month(val, year, month);
  switch(month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      day_number_ = 31;
      break;
    case 4: case 6: case 9: case 11:
      day_number_ = 30;
      break;
    case 2:
      day_number_ = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0) ? 29 : 28;
      break;
    default:
      throw std::logic_error("Attempted to pick a day_number for an invalid month.");
      break;
  }
  first_month_day_ = ConfigParsingUtils::get_first_month_day(year, month);
}

int ConfigParsingUtils::get_int(const std::string &source) {
  // Get an int. Passes along exceptions from `stoi`.
  // Throw: `std:.invalid_argument` if entire string isn't consumed.

  int parsed_value;
  size_t value_end;
  parsed_value = stoi(source, &value_end);

  if (value_end < source.size()) {
    throw std::invalid_argument("Didn't consume the entire string creating an int.");
  }

  return parsed_value;
}

GoalType ConfigParsingUtils::get_goal_type(const std::string &source) {
  // Get a GoalType.
  // Throw: `std::invalid_argument` if source is not known as an identifier of the enum.

  if (source == "regular-cumulative") {
    return GoalType::regular_c;
  } else if (source == "regular-daily") {
    return GoalType::regular_d;
  } else if (source == "uphill-cumulative") {
    return GoalType::uphill_c;
  } else if (source == "uphill-daily") {
    return GoalType::uphill_d;
  } else if (source == "reverse-cumulative" || source == "downhill-cumulative") {
    return GoalType::reverse_c;
  } else if (source == "reverse-daily" || source == "downhill-daily") {
    return GoalType::reverse_d;
  } else {
    throw std::invalid_argument("Unrecognized goal type of " + source + ".");
  }
}

GoalRounding ConfigParsingUtils::get_goal_rounding(const std::string &source) {
  // Get a GoalRounding.
  // Throw: `std::invalid_argument` if source is not known as an identifier of the enum.

  if (source == "up") {
    return GoalRounding::up;
  } else if (source == "down") {
    return GoalRounding::down;
  } else {
    throw std::invalid_argument("Unrecognized goal rounding of " + source + ".");
  }
}

void ConfigParsingUtils::get_year_month(const std::string &source, int &year, int &month) {
  // Get year and month from string in format `YYYY-MM`.
  // Throw: `std::invalid argument` if invalid format or month is out of range.
  if (source.length() != 7 || source[4] != '-') {
    throw std::invalid_argument("Month string in invalid format. (expected 'YYYY-MM').");
  }
  std::string yearstring = source.substr(0, 4);
  std::string monthstring = source.substr(5);
  year = get_int(yearstring);
  month = get_int(monthstring);
  if (month < 1 || month > 12) {
    throw std::invalid_argument("Month in month string out of range [1, 12].");
  }
}

int ConfigParsingUtils::get_first_month_day(int year, int month) {
  // Return first day of the month (1-MON, 7-SUN)
  // Determining the first day of the month is done using the formula from here:
  // https://cs.uwaterloo.ca/~alopez-o/math-faq/node73.html
  // (adjusted for weeks beginning on MONDAY and not SUNDAY)
  int century = year / 100;
  year -= 100 * century;
  month -= 2;
  if (month <= 0) {
    --year;
    month += 12;
  }

  int weekday = 1 + (int)(2.6f * (float)month - 0.2f)
      - 2*century + year + (year/4) + (century/4);
  // ...because this needs a mod that is DEFINITELY a periodic positive function,
  // we have to write it here, as otherwise mod of neg. number is implementation-defined
  weekday -= (weekday/7) * 7;
  weekday = (weekday <= 0) ? 7+weekday : weekday; // this also shifts sunday (0) to 7
  return weekday;
};
