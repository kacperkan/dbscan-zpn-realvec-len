## Requirements
- docker

## Run all experiments

1. docker build -t dami .
1. docker run -it --rm -v $(pwd)/experiments:/usr/src/dami/experiments dami  