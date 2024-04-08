# HueMaster
Create a usable colour scheme from an image

## Key Ideas
* Use [K-means clustering](https://en.wikipedia.org/wiki/K-means_clustering) to find the most dominant colours in an image
* Use [contrast](https://webaim.org/resources/contrastchecker/) to find usable combinations of colours
* Text colors should always be close to black or white for legibility
* Background colors should be dark/light for legibility
* Find colors that are not too similar to each other for easy distinction

## Usage
```bash
HueMaster <path_to_image>
```
Generates a `.Xresources` file from the image and outputs to stdout
The color scheme may be quickly tested at: https://terminal.sexy
