#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <cstdlib>

int Fitness(const std::vector<int>& chromosome, const std::vector<std::vector<int>>& costs, const std::vector<std::pair<int, int>>& constraints) {
	int fitness = 0;
	for (int i = 0; i < chromosome.size() - 1; ++i) {
		fitness += costs[chromosome[i]][chromosome[i + 1]];
	}

	int penalizare = 1000;
	for (const auto& constraint : constraints) {
		auto position1 = std::find(chromosome.begin(), chromosome.end(), constraint.first);
		auto position2 = std::find(chromosome.begin(), chromosome.end(), constraint.second);
		if(position1 > position2)
			fitness += penalizare;
	}

	return fitness;
}

std::vector<int> GenerateValidChromosome(const int& n, const std::vector<std::pair<int, int>>& constraint) {
	std::vector<std::set<int>> graph(n);
	std::vector<int> internDegree(n, 0);
	std::queue<int> queue;
	std::vector<int> result;

	for (const auto& constraint : constraint) {
		graph[constraint.first].insert(constraint.second);
		++internDegree[constraint.second];
	}

	for (int i = 0; i < n; i++) {
		if (internDegree[i] == 0) {
			queue.push(i);
		}
	}

	while (!queue.empty()) {
		int current = queue.front();
		queue.pop();
		result.push_back(current);
		for (const auto& neighbour : graph[current]) {
			graph[neighbour].erase(current);
			--internDegree[neighbour];
			if (internDegree[neighbour] == 0) {
				queue.push(neighbour);
			}
		}
	}

	return result;
}

std::vector<int> TournamentSelection(const std::vector<std::vector<int>>& population, const std::vector<int>& fitnesses, const int& k = 3) {
	int best = rand() % population.size();

	for (int i = 1; i < k; i++) {
		int index = rand() % population.size();
		if (fitnesses[index] < fitnesses[best]) {
			best = index;
		}
	}

	return population[best];
}

std::vector<int> OrderCrossover(const std::vector<int>& p1, const std::vector<int>& p2) {
	int n = p1.size();
	std::vector<int> child(n, -1);
	int start = rand() % n;
	int end = rand() % n;

	if (start > end) {
		std::swap(start, end);
	}

	for (int i = start; i <= end; ++i) {
		child[i] = p1[i];
	}

	int idx = (end + 1) % n;
	for (int i = 0; i < n; i++) {
		int val = p2[(end + 1 + i) % n];
		if (std::find(child.begin(), child.end(), val) == child.end()) {
			child[idx] = val;
			idx = (idx + 1) % n;
		}
	}

	return child;
}

void SwapMutation(std::vector<int>& chromosome) {
	int i = rand() % chromosome.size();
	int j = rand() % chromosome.size();
	std::swap(chromosome[i], chromosome[j]);
}

void RepairChromosome(std::vector<int>& chromosome, const std::vector<std::pair<int, int>>& constraints) {
	for (const auto& constraint : constraints) {
		auto position1 = std::find(chromosome.begin(), chromosome.end(), constraint.first);
		auto position2 = std::find(chromosome.begin(), chromosome.end(), constraint.second);
		if (position1 > position2) {
			int val = *position2;
			chromosome.erase(position2);
			position1 = std::find(chromosome.begin(), chromosome.end(), constraint.first);
			chromosome.insert(position1 + 1, val);
		}
	}
}

int main() {
	srand(time(0));

	std::vector<int> chromosome;
	int n;
	FILE* f;
	errno_t err = fopen_s(&f, "File.txt", "r");
	if (f == NULL) {
		std::cout << "Could not open the file." << std::endl;
		return 1;
	}

	fscanf_s(f, "%d", &n);
	std::cout << "Number of nodes: " << n << std::endl;
	std::vector<std::vector<int>> costs(n, std::vector<int>(n));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			fscanf_s(f, "%d", &costs[i][j]);
		}
	}
	std::cout << "Matrix of costs:" << std::endl;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cout << costs[i][j] << " ";
		}
		std::cout << std::endl;
	}

	std::vector<std::pair<int, int>> constraints;
	int m;
	fscanf_s(f, "%d", &m);
	std::cout << "\nNumber of constraints: " << m << std::endl;
	for (int i = 0; i < m; ++i) {
		int x, y;
		fscanf_s(f, "%d %d", &x, &y);
		constraints.push_back({ x, y });
	}
	fclose(f);

	const int popSize = 50;
	const int generations = 100;

	std::vector<std::vector<int>> population;
	for (int i = 0; i < popSize; ++i) {
		chromosome = GenerateValidChromosome(n, constraints);
		population.push_back(chromosome);
	}

	std::vector<int> fitnesses(population.size());
	for (int i = 0; i < population.size(); ++i) {
		fitnesses[i] = Fitness(population[i], costs, constraints);
	}
	std::vector<int> bestChromosome = population[0];
	int bestFitness = fitnesses[0];
	for (int i = 1; i < fitnesses.size(); ++i) {
		if (fitnesses[i] < bestFitness) {
			bestFitness = fitnesses[i];
			bestChromosome = population[i];
		}
	}
	std::cout << "\nThe best chromosome: ";
	for (const auto& val : bestChromosome) {
		std::cout << val << " ";
	}
	std::cout << "\nFitness: " << bestFitness << std::endl;
	for (int gen = 0; gen < generations; ++gen) {
		std::vector<std::vector<int>> newPopulatie;
		for (int i = 0; i < popSize; ++i) {
			auto parent1 = TournamentSelection(population, fitnesses);
			auto parent2 = TournamentSelection(population, fitnesses);
			auto child = OrderCrossover(parent1, parent2);
			if (rand() % 100 < 5) {
				SwapMutation(child);
			}
			RepairChromosome(child, constraints);
			newPopulatie.push_back(child);
		}
		population = newPopulatie;
		for (int i = 0; i < population.size(); ++i) {
			fitnesses[i] = Fitness(population[i], costs, constraints);
		}
		bestChromosome = population[0];
		bestFitness = fitnesses[0];
		for (int i = 1; i < fitnesses.size(); ++i) {
			if (fitnesses[i] < bestFitness) {
				bestFitness = fitnesses[i];
				bestChromosome = population[i];
			}
		}
	}

	std::cout << "\nThe best chromosome after generations: ";
	for (const auto& val : bestChromosome) {
		std::cout << val << " ";
	}

	std::cout << "\nFitness: " << bestFitness << std::endl;
	return 0;
}