# HueMaster
Create a usable color scheme from an image

## Key Ideas
* Use [K-means clustering](https://en.wikipedia.org/wiki/K-means_clustering) to find the most dominant colors in an image
* Use [contrast](https://webaim.org/resources/contrastchecker/) to find usable combinations of colors
* Text colors should always be close to black or white for legibility
* Background colors should be dark/light for legibility
* Find colors that are not too similar to each other for easy distinction
* Keep the format files human-readable

## Installation
Clone the repository and recursively:
```bash
git clone --recurse-submodules git@github.com:smntic/HueMaster.git
```

Build the project:
```bash
cd HueMaster
cmake .
make
sudo make install
```

This installs the `huemaster` executable to the bin directory.

## Usage
```bash
huemaster
```

## Configuration
Create configuration file with path `~/.config/huemaster/config.toml`.\
The configuration file should have the following format:
```toml
[Wallpaper]
path = "path/to/wallpaper"

[section_name]
format_path = "path/to/format"
real_path = "path/to/real_file"

[another_section_name]
format_path = "path/to/another_format"
real_path = "path/to/another_real_file"

# ...
```
The `section_name` can be any distinct name.\
\
For example, for `.Xresources` configuration:
```toml
[Wallpaper]
path = "wallpaper"

[Xresources]
format_path = "Xresources.format"
real_path = "~/.Xresources"
```
\
The file specified in `format_path` should be a copy of the configuration file with placeholders for the colors.\
The placeholders can be in the color format: \
`$$COLOR.command1(arg1).command2(arg2).format$$` \
or the special format: \
`$$SPECIAL$$`

### Color format

The `COLOR` argument can be any of the following:
* "BACKGROUND" or "FOREGROUND" for the background and foreground colors respectively
* "COLOR0" to "COLOR15" for the 16 colors in the palette
* "ACCENT", "GOOD", "WARNING", "ERROR" and "INFO" are macros for colors that could be used for those purposes.
These colors are modified to be easily distinguishable from the other colors.

The commands are chained together by `.`'s to modify the color.\
The available commands are:
* `lighten(n)` to lighten the color by `n` percent
* `darken(n)` to darken the color by `n` percent
* `alpha(n)` to change the alpha value of the color by `n` percent

For example, for `.Xresources` configuration:
```Xresources
! special
*.foreground:   $$FOREGROUND$$
*.background:   $$BACKGROUND$$
*.cursorColor:  $$FOREGROUND$$

! black
*.color0:       $$COLOR0.lighten(10).darken(20)$$
*.color8:       $$COLOR0.darken(10)$$

...
```

### Special format

The `SPECIAL` argument can be any of the following:
* "LIGHT?STRING_1:STRING_2" to choose between `STRING_1` and `STRING_2` based on the lightness of the background color

\
The `format` argument can be any of the following:
* `HEXRGB` -> #RRGGBB
* `HEXRGBA` -> #RRGGBBAA
* `HEXARGB` -> #AARRGGBB
* `RGB` -> R G B (integers 0-255)
* `RGBA` -> R G B A 
* `ARGB` -> A R G B
* `CRGB` -> R,G,B
* `CRGBA` -> R,G,B,A 
* `CARGB` -> A,R,G,B

\
The parsed and formatted file will be written to the path specified in `real_path`.\
Make sure to back up the original files before running the program!

