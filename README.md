# HueMaster
Create a usable colour scheme from an image

## Key Ideas
* Use [K-means clustering](https://en.wikipedia.org/wiki/K-means_clustering) to find the most dominant colours in an image
* Use [contrast](https://webaim.org/resources/contrastchecker/) to find usable combinations of colours
* Text colors should always be close to black or white for legibility
* Background colors should be dark/light for legibility
* Find colors that are not too similar to each other for easy distinction
* Keep the format files human-readable

## Installation
Clone the repository and recursively:
```bash
git clone --recursive git@github.com:MrPersonDev/HueMaster.git
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
huemaster <path_to_image>
```

## Configuration
Create a configuration file in the same directory as the executable (for now) named `config.toml`\
The configuration file should have the following format:
```toml
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
[Xresources]
format_path = "Xresources.format"
real_path = "~/.Xresources"
```
\
The file specified in `format_path` should be a copy of the configuration file with placeholders for the colours.\
The placeholders can be in one of two formats:
* "$$BACKGROUND$$" or "$$FOREGROUND$$" for the background and foreground colours respectively
* "$$COLOR0$$" to "$$COLOR15$$" for the 16 colours in the palette

For example, for `.Xresources` configuration:
```Xresources
! special
*.foreground:   $$FOREGROUND$$
*.background:   $$BACKGROUND$$
*.cursorColor:  $$FOREGROUND$$

! black
*.color0:       $$COLOR0$$
*.color8:       $$COLOR8$$

...
```

The parsed and formatted file will be written to the path specified in `real_path`.\
Make sure to back up the original files before running the program!
