# Genetic Algorithm for Sequential Ordering Problem (SOP)
## Overview
This C++ implementation solves the NP-hard Sequential Ordering Problem (SOP) using a genetic algorithm approach. The SOP extends the Traveling Salesman Problem (TSP) by adding precedence constraints between nodes.

## Features
### Genetic operators:

Tournament selection (size=3)

Order Crossover (OX)

Swap mutation (5% probability)

### Constraints handling:

Topological sorting for initial valid solutions

Chromosome repair mechanism

### Fitness function:

Path cost summation

Heavy penalty (λ=1000) for constraint violations

## Input Format
The program reads input from "File.txt" with format:

n

cost_0_0 cost_0_1 ... cost_0_n-1

...

cost_n-1_0 ... cost_n-1_n-1

m

i1 j1

...

im jm


## Parameters
Population size: 50

Generations: 100

Mutation rate: 5%

Tournament size: 3

## Implementation Details
### Key Components:

Solution Representation:

Chromosomes as permutations of nodes

Valid initial population via topological sort

### Genetic Operations:

TournamentSelection(): Selects parents using tournament selection

OrderCrossover(): Preserves subsequences from parents

SwapMutation(): Randomly swaps two nodes

### Constraint Management:

RepairChromosome(): Fixes precedence violations

Penalty term in fitness evaluation

### Fitness Evaluation:

Computes total path cost

Applies penalties for constraint violations

## Example

![image](https://github.com/user-attachments/assets/9c08200d-b6f6-4c55-90f9-f10a6e5427f1)
