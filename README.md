# Rumor Spreading in Randomized Algorithms

This repository contains implementations, experiments, and analysis for studying rumor spreading algorithms under various models such as PUSH, PULL, hybrid approaches, and extensions including failures, multiple calls, and influencers.

## Repository Structure

```
.
├── Hybrid Push-Pull/
├── Influencers/
├── Karp Push-Pull/
├── MultipleInfluencers/
├── k-calls/
├── pull_with_failure_probability/
├── pushVSpull/
├── PPT.pdf
├── Report.pdf
├── README.md

```

### Experiments

Each folder (except PPT and Report) corresponds to an independent experiment. The structure inside each experiment folder is consistent:

- `simulate.cpp`  
  Contains the implementation of the simulation for that specific model or variant.

- `output.csv`  
  Stores the raw data generated from running the simulation.

- `plot.py`  
  Python script used to generate graphs and visualizations from the CSV data.

Each experiment studies a different aspect of rumor spreading:

- `pushVSpull/`  
  Comparison of PUSH and PULL models in terms of rounds and communication cost.

- `pull_with_failure_probability/`  
  Analysis of how failure probability affects dissemination time.

- `k-calls/`  
  Study of multiple calls per node and its impact on spreading speed.

- `Influencers/` and `MultipleInfluencers/`  
  Experiments with multiple initially informed nodes and their effect on convergence time.

- `Hybrid Push-Pull/`  
  Threshold-based hybrid strategy combining PUSH and PULL.

- `Karp Push-Pull/`  
  Implementation and evaluation of the Karp et al. adaptive algorithm.

## How to Run an Experiment

### 1. Compile the simulation

```bash
g++  simulate.cpp -static-libstdc++ -static-libgcc -o simulate
````

### 2. Run the simulation

```bash
./simulate
```

This generates an `output.csv` file containing experimental data.

### 3. Generate plots

```bash
python plot.py
```

This produces graphs based on the simulation results.

## Main Files

* `PPT.pdf`
  Presentation summarizing the problem, intuition, experiments, and key results.

* `Report.pdf`
  Detailed report containing theoretical analysis, experimental setup, results, and discussion.

## Overview

The project explores:

* Time complexity of rumor spreading (number of rounds)
* Communication cost (total number of calls)
* Trade-offs between PUSH and PULL models
* Impact of failures, parallelism, and multiple sources
* Adaptive strategies (Karp et al.) for reducing redundant communication

All experiments are designed to validate theoretical predictions and study constant-factor behavior through large-scale simulations.

## Notes

* All simulations use high-performance random number generation for accuracy and speed.
* Experiments are scaled across different values of ( n ) to observe asymptotic behavior.
* Data analysis and visualization are handled through Python scripts for reproducibility.

