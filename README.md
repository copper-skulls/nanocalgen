# NaNoCalGen README / User documentation

For developer documentation, go [here](./README_dev.md).

**NaNo**WriMo **Cal**endar **Gen**erator

An utility for generating an `.SVG` calendar grid with wordcount labels for given parameters. Specifically made as a tool to make creating goal calendars for [NaNoWriMo](https://nanowrimo.org/) easier for whoever wants to do so, as managing such grids by hand is annoying and fidgety at best.

Currently supports (daily and cumulative):

- regular goals (goal wordcount evenly divided between days)
- uphill goals (start at 1 word, ramp up)
- downhill/reverse goals (reversed uphill: start at high wordcount, end at 1)

Default values should be somewhat sensible and work out-of-the-box, otherwise they're explained below.

## Building and running

To build, just run: `clang++ -std=c++20 *.cpp -o ncg`.

Then, to run: `./ncg [options]`.

## Options

Options are loaded in this order, and newer values overload older ones:

1. Defaults
2. Environment (order of loading options that override each other is NOT RIGIDLY DEFINED. be careful and don't count on any particular order if using environment-variable settings)
	- these are loaded from the noted `NCG_[option]` env. variables
3. CLI options (+ and file-options loaded at the moment parser finds them in the CLI options)
  - CLI uses the `--[name of the option] [value]` format
  - file-options are loaded by the `--config [path]` option
    - this option is **only** available from CLI arguments to avoid possibility of circular references and other problems
  - file options use the `[name of the option] [value]` format, one per line [(example here)](./example_config.txt)

Some options allow for different ways/logics how to set the same variables. In that case, the newer-overrides-older logic still applies.

> :warning: Most string-based options allow for **arbitrary string input** and thus are not safe from code injection into the resulting SVG. **Be careful** using configs supplied by other people - proper sanitization is not within the scope of this utility.

### General SVG options

`width [int, default=1400]` - result SVG document width
- env: `NCG_WIDTH`
- overriden by: `cell-width`

`height [int, default=1000]` - result SVG document height
- env: `NCG_HEIGHT`
- overriden by: `cell-height`

### Calendar cell options

**cell-width [int, default=None]** - width of calendar cell
- env: `NCG_CELL_WIDTH`
- sets: `width` with some extra logic steps (will work off currently set `margins`)

**cell-height [int, default=None]** - height of calendar cell
- env: `NCG_CELL_HEIGHT`
- sets: `height` with some extra logic steps (will work off currently set `day-number`, `monday-cell-offset`, `first-month-day` and `margins`)

**cell-primary-color [string, default="white"]** - fill color for cell
- will accept arbitrary string without checking. recommended: color names and hex codes
- env: `NCG_CELL_PRIMARY_COLOR`

**cell-secondary-color [string, default="black"]** - stroke color for cell
- will accept arbitrary string without checking. recommended: color names and hex codes
- env: `NCG_CELL_SECONDARY_COLOR`

**cell-stroke-width [int, default=5]** - stroke width for cell (can be 0)
- env: `NCG_CELL_STROKE_WIDTH`

**cell-roundness [int, default=10]** - roundness for cell
- env: `NCG_CELL_ROUNDNESS`

### Cell layout options

**monday-cell-offset [int [0-6], default=0]** - offset of the monday cell (to the right). ie. if you want your week to start on Sunday, set this to 2
- env: `NCG_MONDAY_CELL_OFFSET`

**day-number [int, default=31]** - number of days in the calendar grid
- accepts an arbitrary int, doesn't check for "month-liness" of them
- env: `NCG_DAY_NUMBER`

**first-month-day [int [1-7], default=2]** - day the month starts. 1 is Monday, 7 is Sunday.
- env: `NCG_FIRST_MONTH_DAY`
- overriden by: `month`

**margins [int, default=10]** - space between border of the document and first cell, last cell and the border of the document, and between cells themselves,
- env: `NCG_MARGINS`

**month [string, default=None]** - pre-calculates grid layout needed for a given month
- accepts a string of the format `YYYY-MM`
- env: `NCG_MONTH`
- sets: `first-month-day`, `day-number`

### Text options

**text-offset-x [int, default=10]** - padding from the border of the cell, in the x direction
- env: `NCG_TEXT_OFFSET_X`

**text-offset-y [int, default=10]** - padding from the border of the cell, in the y direction
- env: `NCG_TEXT_OFFSET_Y`

**text-baseline [string, default="hanging"]** - setting of the dominant baseline for the text
- will accept arbitrary string without checking. see `dominant-baseline` property for svg text to see what will work. recommended: "hanging" or "text-before-anchor"
- env: `NCG_TEXT_BASELINE`

**text-color [string, default="black"]** - fill color for text
- will accept arbitrary string without checking. recommended: color names and hex codes
- env: `NCG_TEXT_COLOR`

**font-family [string, default="FreeMono"]** - font family for all text
- will accept arbitrary string without checking.
- env: `NCG_FONT_FAMILY`

**font-weight [string, default="bold"]** - font weight for all text
- will accept arbitrary string without checking. recommended: "bold", "italic", "regular"
- env: `NCG_FONT_WEIGHT`

**font-size [int, default=30]** - font size for all text
- env: `NCG_FONT_SIZE`

### Goal options

**goal-wordcount [int, default=50000]** - goal wordcount
- note that there might be slight deviances from the set goal caused by rounding
- env: `NCG_GOAL_WORDCOUNT`

**goal-type [string, default="regular-cumulative"]** - goal type
- does NOT accept arbitrary string. options:
	- `regular-cumulative` - evenly divides words of goal per each day. displays cumulative
	- `regular-daily` - evenly divides words of goal per each day. displays daily
	- `uphill-cumulative` - starts small (1), rises. displays cumulative
	- `uphill-daily` - starts small (1), rises. displays daily
	- `reverse-cumulative` (alt: `downhill-cumulative`) - reverse of uphill. displays cumulative
	- `reverse-daily` (alt: `donwnhill-daily`) - reverse of uphill. displays daily
- env: `NCG_GOAL_TYPE`

**goal-rounding [string, default="up"]** - rounding behaviour
- accepts: `up` ie. ceiling operation, and `down` ie. flooring operation
- env: `NCG_GOAL_ROUNDING`
