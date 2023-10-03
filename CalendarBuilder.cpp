#include "CalendarBuilder.hpp"
#include <algorithm>
#include <stdexcept>

SVGRoot CalendarBuilder::from_config(const Config &config) {
  // Generate an SVGRoot representing a calendar with given `config`.

  int actual_inner_margins = config.margins() + config.cell_stroke_width();
  int actual_border_margins = config.margins() + (config.cell_stroke_width()/2);
  int first_primary_cell_offset = (config.first_month_day() - 1 + config.monday_cell_offset()) % 7;

  int row_count = MathUtils::ceiling_division(first_primary_cell_offset + config.day_number(), 7);

  int rectangle_width = (config.width() - 6*actual_inner_margins - 2*actual_border_margins) / 7;
  int rectangle_height = (config.height() - 6*actual_inner_margins - 2*actual_border_margins) / row_count;

  std::vector<std::unique_ptr<SVGElement>> primary_squares;
  std::vector<std::unique_ptr<SVGElement>> secondary_squares;
  std::vector<std::unique_ptr<SVGElement>> primary_square_dates;
  std::vector<std::unique_ptr<SVGElement>> primary_square_wordcounts;

  std::vector<int> wordcount_labels =
      calculate_wordcount_labels(config.day_number(), config.goal_type(),
      config.goal_rounding(), config.goal_wordcount()
  );

  int y = actual_border_margins;
  int cell_index = 0;
  for (int week = 0; week < row_count; ++week) {
    int x = actual_border_margins;
    for (int day = 0; day < 7; ++day) {
      int date = cell_index - first_primary_cell_offset + 1;

      if (date >= 1 && date <= config.day_number()) {
        // primary square
        SVGRectangle square;
        square.x = x; square.y = y;
        square.width = rectangle_width; square.height = rectangle_height;
        square.roundness_x = config.cell_roundness();
        square.roundness_y = config.cell_roundness();
        primary_squares.push_back(std::make_unique<SVGRectangle>(std::move(square)));

        SVGText date_label;
        date_label.text = std::to_string(date);
        date_label.x = x + config.text_offset_x();
        date_label.y = y + config.text_offset_y();
        primary_square_dates.push_back(std::make_unique<SVGText>(std::move(date_label)));

        SVGText wordcount_label;
        wordcount_label.text = std::to_string(wordcount_labels[date-1]);
        wordcount_label.x = x + rectangle_width - config.text_offset_x();
        wordcount_label.y = y + rectangle_height - config.text_offset_y();
        primary_square_wordcounts.push_back(std::make_unique<SVGText>(std::move(wordcount_label)));
      } else {
        // secondary square
        SVGRectangle square;
        square.x = x; square.y = y;
        square.width = rectangle_width; square.height = rectangle_height;
        square.roundness_x = config.cell_roundness();
        square.roundness_y = config.cell_roundness();
        secondary_squares.push_back(std::make_unique<SVGRectangle>(std::move(square)));
      }
      x += rectangle_width + actual_inner_margins;
      ++cell_index;
    }
    y += rectangle_height + actual_inner_margins;
  }

  SVGGroup secondary_square_group;
  secondary_square_group.properties["style"] = create_style_string(
      config.cell_secondary_color(), config.cell_secondary_color(), config.cell_stroke_width());
  secondary_square_group.children = std::move(secondary_squares);

  SVGGroup primary_square_group;
  primary_square_group.properties["style"] = create_style_string(
      config.cell_primary_color(), config.cell_secondary_color(), config.cell_stroke_width());
  primary_square_group.children = std::move(primary_squares);

  SVGGroup dates_group;
  dates_group.properties["dominant-baseline"] = config.text_baseline();
  dates_group.children = std::move(primary_square_dates);

  SVGGroup wordcount_group;
  wordcount_group.properties["text-anchor"] = "end";
  wordcount_group.children = std::move(primary_square_wordcounts);

  SVGGroup overarching_text_group;
  overarching_text_group.properties["style"] = create_style_string(config.text_color());
  overarching_text_group.properties["font-family"] = config.font_family();
  overarching_text_group.properties["font-size"] = std::to_string(config.font_size());
  overarching_text_group.properties["font-weight"] = config.font_weight();
  overarching_text_group.children.push_back(std::make_unique<SVGGroup>(std::move(dates_group)));
  overarching_text_group.children.push_back(std::make_unique<SVGGroup>(std::move(wordcount_group)));

  SVGRoot root;
  root.width = config.width(); root.height = config.height();
  root.children.push_back(std::make_unique<SVGGroup>(std::move(secondary_square_group)));
  root.children.push_back(std::make_unique<SVGGroup>(std::move(primary_square_group)));
  root.children.push_back(std::make_unique<SVGGroup>(std::move(overarching_text_group)));

  return root;
}

std::string CalendarBuilder::create_style_string
  (std::string main_color, std::string stroke_color, int stroke_width) {
    // Create a string for a `style` xml property with given parameters.
    return "fill:" + main_color + "; stroke-width:" + std::to_string(stroke_width) + "; stroke:" + stroke_color;
}

std::string CalendarBuilder::create_style_string
  (std::string main_color) {
    // Create a string for a `style` xml property with given parameters.
    return "fill:" + main_color;
}

std::vector<int> CalendarBuilder::calculate_date_labels(int day_number) {
  // Return a vector used for date labeling for month of length `day_number`.
  std::vector<int> dates;
  for (int i = 1; i <= day_number; ++i) {
    dates.push_back(i);
  }
  return dates;
}

std::vector<int> CalendarBuilder::calculate_wordcount_labels
  (int day_number, GoalType goal_type, GoalRounding rounding, int goal_wordcount) {
    std::vector<int> labels;
    // Return vector of wordcount labels for each day, using given parameters.
    // Throw: `std::logic_error` if given `goal_type` has not been handled (THIS IS A BUG).
    switch (goal_type) {
      case GoalType::regular_c:
        labels = wc_regular_c(day_number, rounding, goal_wordcount);
        break;
      case GoalType::regular_d:
        labels = wc_regular_d(day_number, rounding, goal_wordcount);
        break;
      case GoalType::uphill_c:
        labels = wc_uphill_c(day_number, rounding, goal_wordcount);
        break;
      case GoalType::uphill_d:
        labels = wc_uphill_d(day_number, rounding, goal_wordcount);
        break;
      case GoalType::reverse_c:
        labels = wc_reverse_c(day_number, rounding, goal_wordcount);
        break;
      case GoalType::reverse_d:
        labels = wc_reverse_d(day_number, rounding, goal_wordcount);
        break;
      default:
        throw std::logic_error("This goal type has not been implemented.");
        break;
    }
    return labels;
}

std::vector<int> CalendarBuilder::wc_regular_c(int day_number, GoalRounding rounding, int goal_wordcount) {
  // Return wordcount labels for regular goal (cumulative).
  return MathUtils::cumulate(wc_regular_d(day_number, rounding, goal_wordcount));
}

std::vector<int> CalendarBuilder::wc_regular_d(int day_number, GoalRounding rounding, int goal_wordcount) {
  // Return wordcount labels for regular goal (daily).
  int daily_wc;
  if (rounding == GoalRounding::up) {
    daily_wc = MathUtils::ceiling_division(goal_wordcount, day_number);
  } else {
    daily_wc = goal_wordcount / day_number;
  }
  return std::vector<int>(day_number, daily_wc);
}

std::vector<int> CalendarBuilder::wc_uphill_c(int day_number, GoalRounding rounding, int goal_wordcount) {
  // Return wordcount labels for uphill goal (cumulative).
  return MathUtils::cumulate(wc_uphill_d(day_number, rounding, goal_wordcount));
}

std::vector<int> CalendarBuilder::wc_uphill_d(int day_number, GoalRounding rounding, int goal_wordcount) {
  // Return wordcount labels for uphill goal (daily).
  std::vector<int> reverse_daily = wc_reverse_d(day_number, rounding, goal_wordcount);
  std::reverse(reverse_daily.begin(), reverse_daily.end());
  return reverse_daily;
}

std::vector<int> CalendarBuilder::wc_reverse_c(int day_number, GoalRounding rounding, int goal_wordcount) {
  // Return wordcount labels for reverse/downhill goal (cumulative).
  return MathUtils::cumulate(wc_reverse_d(day_number, rounding, goal_wordcount));
}

std::vector<int> CalendarBuilder::wc_reverse_d(int day_number, GoalRounding rounding, int goal_wordcount) {
  // Return wordcount labels for reverse/downhill goal (daily).
  // Source for formula used:
  // https://docs.google.com/spreadsheets/d/1ZOl8XTjEWyCttvqn6SBsAvXXH53JrHIXbhQMexGg79s/edit#gid=0`
  // Since GDOCS links are unstable, the link can also be found here:
  // https://www.wikiwrimo.org/wiki/Reverse_NaNo
  std::vector<int> labels(day_number, 1);

  int std_average;
  if (rounding == GoalRounding::up) {
    std_average = MathUtils::ceiling_division(goal_wordcount, day_number);
  } else {
    std_average = goal_wordcount / day_number;
  }

  for (int i=0; i < day_number - 1; ++i) {
    // we leave the 1 at the end!
    labels[i] = (2*std_average);
    if (rounding == GoalRounding::up) {
      labels[i] -= (i * (2*std_average)) / (day_number - 1);
    } else {
      labels[i] -= MathUtils::ceiling_division((i * (2*std_average)), (day_number - 1));
    }
  }

  return labels;
}

int MathUtils::ceiling_division(int dividend, int divisor) {
  // Divide with rounding UP.
  // Due to different ways `%` is implemented for negative numbers,
  // might behave strange for negative numbers (but that is not this use case).
  return (dividend % divisor == 0) ? dividend/divisor : (dividend/divisor) + 1;
}

std::vector<int> MathUtils::cumulate(std::vector<int> original) {
  // Returns a vector in which returned[i] is the sum of original[0-i].
  int accumulator = 0;
  for (int i=0; i < original.size(); ++i) {
    accumulator += original[i];
    original[i] = accumulator;
  }
  return original;
}
