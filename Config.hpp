#ifndef CONFIG_HPP__
#define CONFIG_HPP__

// Loading and parsing configuraton.

#include <string>
#include <iostream>
#include <vector>
#include <map>

enum class GoalType { regular_c, regular_d, uphill_c, uphill_d, reverse_c, reverse_d };
enum class GoalRounding { up, down };

class Config {
  // Configuration class.

  public: 
    int width() const;
    int height() const;

    const std::string &cell_primary_color() const;
    const std::string &cell_secondary_color() const;
    int cell_stroke_width() const;
    int cell_roundness() const;

    int day_number() const;
    int monday_cell_offset() const;
    int first_month_day() const;
    int margins() const;

    int text_offset_x() const;
    int text_offset_y() const;
    const std::string &text_baseline() const;
    const std::string &text_color() const;
    const std::string &font_family() const;
    const std::string &font_weight() const;
    int font_size() const;

    int goal_wordcount() const;
    GoalType goal_type() const;
    GoalRounding goal_rounding() const;

    const std::string &output_filename() const;

    Config();

    void load_from_environment();
    void load_from_cli_args(const std::vector<std::string> &args);
    void load_from_file(const std::string &filename);
    void load_from_file(std::istream &is);
    
    void print(std::ostream &os = std::cout);

  private:

    int width_, height_;

    std::string cell_primary_color_, cell_secondary_color_;
    int cell_stroke_width_, cell_roundness_;

    int day_number_;
    int monday_cell_offset_;
    int first_month_day_;
    int margins_;

    int text_offset_x_, text_offset_y_;
    std::string text_baseline_;
    std::string text_color_;
    std::string font_family_, font_weight_;
    int font_size_;

    int goal_wordcount_;
    GoalType goal_type_;
    GoalRounding goal_rounding_;

    std::string output_filename_;

    enum class IntOptions;
    enum class StringOptions;

    const static std::map<std::string, Config::IntOptions> ENV_TO_INT_OPTION;
    const static std::map<std::string, Config::StringOptions> ENV_TO_STRING_OPTION;
    const static std::map<std::string, Config::IntOptions> FILE_TO_INT_OPTION;
    const static std::map<std::string, Config::StringOptions> FILE_TO_STRING_OPTION;

    void set_int_option_master(Config::IntOptions option, const std::string &val);
    void set_string_option_master(Config::StringOptions option, const std::string &val);

    void set_cell_width(int val);
    void set_cell_height(int val);
    void set_monday_cell_offset(int val);
    void set_first_month_day(int val);
    void set_month(const std::string &val);
};

class ConfigParsingUtils {
  // General wrapper functions to make parsing some config values easier
  // (and more predictable with exceptions).
  public:
    static int get_int(const std::string &source);
    static GoalType get_goal_type(const std::string &source);
    static GoalRounding get_goal_rounding(const std::string &source);
    static void get_year_month(const std::string &source, int &year, int &month);
    static int get_first_month_day(int year, int month);
};
#endif
