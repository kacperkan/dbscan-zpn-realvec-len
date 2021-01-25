#!/bin/bash

mkdir -p data/example

python python/produce_example_data.py data/example
python python/make_dataset_zpn.py \
    data/example/data.arff \
    data/example/data_zpn.arff \
    --has_labels


echo "Downloading complex9 dataset ..."
mkdir data/complex9
wget https://raw.githubusercontent.com/deric/clustering-benchmark/master/src/main/resources/datasets/artificial/complex9.arff \
    -O data/complex9/data.arff

echo "Downloading cluto-t7-10k dataset ..."
mkdir data/cluto-t7-10k
wget https://raw.githubusercontent.com/deric/clustering-benchmark/master/src/main/resources/datasets/artificial/cluto-t7-10k.arff \
    -O data/cluto-t7-10k/data.arff

echo "Downloading letter dataset ..."
mkdir data/letter
wget https://raw.githubusercontent.com/deric/clustering-benchmark/master/src/main/resources/datasets/real-world/letter.arff \
    -O data/letter/data.arff

echo "Making ZPN datasets ..."
python python/make_dataset_zpn.py \
    data/complex9/data.arff \
    data/complex9/data_zpn.arff \
    --has_labels

python python/make_dataset_zpn.py \
    data/cluto-t7-10k/data.arff \
    data/cluto-t7-10k/data_zpn.arff \
    --has_labels

python python/make_dataset_zpn.py \
    data/letter/data.arff \
    data/letter/data_zpn.arff \
    --has_labels
