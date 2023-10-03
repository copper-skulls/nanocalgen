#ifndef HPP_CALENDAR_BUILDER__
#define HPP_CALENDAR_BUILDER__

// Building the calendar as an SVG document.

#include "SVGDocument.hpp"
#include "Config.hpp"

class CalendarBuilder {
  // Builder class itself.
  public:
    static SVGRoot from_config(const Config &config);
  private:

    static std::string create_style_string(std::string main_color, std::string stroke_color, int stroke_width);
    static std::string create_style_string(std::string main_color);

    static std::vector<int> calculate_date_labels(int day_number);

    static std::vector<int> calculate_wordcount_labels
      (int day_number, GoalType goal_type, GoalRounding rounding, int goal_wordcount);

    static std::vector<int> wc_regular_c(int day_number, GoalRounding rounding, int goal_wordcount);
    static std::vector<int> wc_regular_d(int day_number, GoalRounding rounding, int goal_wordcount);
    static std::vector<int> wc_uphill_c(int day_number, GoalRounding rounding, int goal_wordcount);
    static std::vector<int> wc_uphill_d(int day_number, GoalRounding rounding, int goal_wordcount);
    static std::vector<int> wc_reverse_c(int day_number, GoalRounding rounding, int goal_wordcount);
    static std::vector<int> wc_reverse_d(int day_number, GoalRounding rounding, int goal_wordcount);
};

class MathUtils {
  // General useful math functions (here used by CalendarBuilder to work).
  public:
    static int ceiling_division(int dividend, int divisor);
    static std::vector<int> cumulate(std::vector<int> original);
};
#endif // !HPP_CALENDAR_BUILDER__
