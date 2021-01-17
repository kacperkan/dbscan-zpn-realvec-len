#!/bin/bash


basic_command_tanimoto="./dbscan_zpn_realvec_len \
    data/example/data.arff \
    sanity_experiment_tanimoto \
    --eps 0.9 \
    --min_pts 10 \
    --has_labels \
    --algorithm_name tanimoto \
    --prefiltering_name none
"

basic_command_base="./dbscan_zpn_realvec_len \
    data/example/data.arff \
    sanity_experiment_base \
    --eps 0.3 \
    --min_pts 10 \
    --has_labels \
    --algorithm_name base \
    --prefiltering_name none
"

basic_command_tanimoto_zpn="./dbscan_zpn_realvec_len \
    data/example/data_zpn.arff \
    sanity_experiment_tanimoto_zpn \
    --eps 0.9 \
    --min_pts 10 \
    --has_labels \
    --algorithm_name tanimoto \
    --prefiltering_name none
"

eval ${basic_command_tanimoto}
eval ${basic_command_base}
eval ${basic_command_tanimoto_zpn}

simpler_basic_command_tanimoto="./dbscan_zpn_realvec_len \
    data/example_simpler/data.arff \
    simpler_sanity_experiment_tanimoto \
    --eps 0.9 \
    --min_pts 1 \
    --has_labels \
    --algorithm_name tanimoto \
    --prefiltering_name none
"

simpler_basic_command_base="./dbscan_zpn_realvec_len \
    data/example_simpler/data.arff \
    simpler_sanity_experiment_base \
    --eps 0.3 \
    --min_pts 1 \
    --has_labels \
    --algorithm_name base \
    --prefiltering_name none
"

eval ${simpler_basic_command_tanimoto}
eval ${simpler_basic_command_base}

epsilons=(
    0.0001 0.001 0.01 0.05 0.1 0.15 0.20 0.25 0.30 0.35 0.40 0.45 0.50 0.55
    0.65 0.70 0.75 0.80 0.85 0.90 0.95 0.96 0.97 0.98 0.99 0.999 0.9999 1.0
)

min_points=(
    0 1 5 10 25 50 100
)

datasets=(
    "complex9" "letter" "cluto-t7-10k" 
)

algorithm_names=(
    "base" "tanimoto"
)

prefilters=(
    "none" "realveclen" "zpnveclen"
)

repeats=1

for eps in ${epsilons[@]}; do
    for min_pts in ${min_points[@]}; do
        for dataset in ${datasets[@]}; do
            for algorithm_name in ${algorithm_names[@]}; do
                for (( repeat=0; repeat < ${repeats}; repeat++ )); do
                    base="${dataset}_${eps}_${min_pts}_${algorithm_name}_${repeat}"
                    if [ ! -d "experiments/${base}_none" ]; then
                        echo "Running: ${base}"
                        echo "No prefiltering ..."

                        command_none="./dbscan_zpn_realvec_len data/${dataset}/data.arff \
                            ${base}_none \
                            --has_labels \
                            --eps ${eps} \
                            --min_pts ${min_pts} \
                            --algorithm_name ${algorithm_name} \
                            --prefiltering_name none"
                        eval ${command_none}
                    else
                        echo  "Omitting: ${base}"
                    fi

                    if [[ ${algorithm_name} != "base" ]]; then
                        if [ ! -d "experiments/${base}_real" ]; then
                            command_real="./dbscan_zpn_realvec_len data/${dataset}/data.arff \
                                ${base}_real \
                                --has_labels \
                                --eps ${eps} \
                                --min_pts ${min_pts} \
                                --algorithm_name ${algorithm_name} \
                                --prefiltering_name realveclen"
                            echo "Real ..."
                            eval ${command_real}
                        else
                            echo  "Omitting: ${base}_real"
                        fi

                        if [ ! -d "experiments/${base}_zpn" ]; then
                            command_zpn="./dbscan_zpn_realvec_len data/${dataset}/data_zpn.arff \
                                ${base}_zpn \
                                --has_labels \
                                --eps ${eps} \
                                --min_pts ${min_pts} \
                                --algorithm_name ${algorithm_name} \
                                --prefiltering_name zpnveclen"

                            echo "ZPN ..."
                            eval ${command_zpn}
                        else
                            echo  "Omitting: ${base}_zpn"
                        fi 
                    fi
                done
            done
        done
    done
done

# Cluto: eps = 0.99; min_pts = 100
# Complex: eps= 0.99; min_pts = 100
# Letter: eps = 0.99; min_pts = 25
final_cluto="./dbscan_zpn_realvec_len \
    data/cluto-t7-10k/data.arff \
    final_cluto \
    --eps 0.99 \
    --min_pts 100 \
    --has_labels \
    --algorithm_name base \
    --prefiltering_name none
"
final_complex="./dbscan_zpn_realvec_len \
    data/complex9/data.arff \
    final_complex9 \
    --eps 0.99 \
    --min_pts 100 \
    --has_labels \
    --algorithm_name base \
    --prefiltering_name none
"
final_letter="./dbscan_zpn_realvec_len \
    data/letter/data.arff \
    final_letter \
    --eps 0.99 \
    --min_pts 25 \
    --has_labels \
    --algorithm_name base \
    --prefiltering_name none
"

eval ${final_cluto}
eval ${final_complex}
eval ${final_letter}

jupyter nbconvert python/analysis.ipynb --to html --no-input --no-prompt --output-dir experiments/ --execute

echo "Report can be found in experiments/analysis.html"
