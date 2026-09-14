// 0/1 Knapsack Problem - Exhaustive Search
// For each problem size n the algorithm:
//  1. Randomly generate n items (weight, value) and knapsack capacity.
//  2. Sole the problem by exhaustively trying all 2^n subsets (every combination of
//  (include/exclude), keeping the best valid subset with the highest total value.
//  3. Measure how long the exhaustive search takes with chrono.

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;

struct Instance
{
    std::vector<int> weights;
    std::vector<int> values;
    int capacity;
};

// Randomly generate a 0/1 kanpsack of size n.
Instance generateInstance(int n, unsigned seed)
{
    // Random number engine seeded per call so each n gets a different but
    // reproducible instance. Each item gets a random weight (1–50) and
    // value (10–100).
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> weightDist(1, 50);
    std::uniform_int_distribution<int> valueDist(10, 100);

    Instance inst;
    inst.weights.resize(n);
    inst.values.resize(n);
    double totalWeight = 0;

    for (int i = 0; i < n; ++i)
    {
        inst.weights[i] = weightDist(rng);
        inst.values[i] = valueDist(rng);
        totalWeight += inst.weights[i];
    }

    // Capacity ~ 50% of total weight
    inst.capacity = static_cast<int>(totalWeight * 0.5);
    return inst;
}

// Solve 0/1 knapsack by exhaustive search over all 2^n subsets.
// Bit i of 'mask' (0 or 1) tells whether item i is included.
double knapsackExhaustive(const Instance &inst)
{
    int n = static_cast<int>(inst.weights.size());
    long long numSubsets = 1LL << n;   // 2^n
    long long bestValue = 0;

    for (long long mask = 0; mask < numSubsets; ++mask) {
        long long totalWeight = 0;
        long long totalValue = 0;
        for (int i = 0; i < n; ++i) {
            if (mask & (1LL << i)) {
                totalWeight += inst.weights[i];
                totalValue += inst.values[i];
                if (totalWeight > inst.capacity) break; // early exit, still O(n) worst case
            }
        }
        if (totalWeight <= inst.capacity && totalValue > bestValue) {
            bestValue = totalValue;
        }
    }
    return bestValue;
}

int main() {
    const double timeLimit = 30.0;   // seconds
    const int startN = 3;
    const unsigned seedBase = 42;

    std::vector<int> ns;
    std::vector<long long> numSubsetsVec;
    std::vector<int> capacities;
    std::vector<long long> bestValues;
    std::vector<double> times;

    std::cout << std::setw(3) << "n" << " | "
              << std::setw(14) << "2^n subsets" << " | "
              << std::setw(8) << "capacity" << " | "
              << std::setw(10) << "best value" << " | "
              << std::setw(10) << "time (s)" << "\n";
    std::cout << std::string(60, '-') << "\n";

    int n = startN;
    while (true) {
        Instance inst = generateInstance(n, seedBase + n);

        auto start = std::chrono::high_resolution_clock::now();
        long long bestValue = knapsackExhaustive(inst);
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();

        long long numSubsets = 1LL << n;

        ns.push_back(n);
        numSubsetsVec.push_back(numSubsets);
        capacities.push_back(inst.capacity);
        bestValues.push_back(bestValue);
        times.push_back(elapsed);

        std::cout << std::setw(3) << n << " | "
                  << std::setw(14) << numSubsets << " | "
                  << std::setw(8) << inst.capacity << " | "
                  << std::setw(10) << bestValue << " | "
                  << std::setw(10) << std::fixed << std::setprecision(4) << elapsed << "\n";

        if (elapsed > timeLimit) {
            std::cout << "\nStopping: n=" << n << " exceeded the "
                      << (int)timeLimit << "s time limit.\n";
            break;
        }
        ++n;
    }

    // Save results to CSV
    std::ofstream out("knapsack_results_cpp.csv");
    out << "n,num_subsets,capacity,best_value,time_seconds\n";
    for (size_t i = 0; i < ns.size(); ++i) {
        out << ns[i] << "," << numSubsetsVec[i] << "," << capacities[i] << ","
            << bestValues[i] << "," << std::fixed << std::setprecision(6) << times[i] << "\n";
    }
    out.close();
    std::cout << "\nResults saved to knapsack_results_cpp.csv\n";

    // Report largest n solved within budget
    int biggestN = -1;
    double biggestTime = 0;
    for (size_t i = 0; i < ns.size(); ++i) {
        if (times[i] <= timeLimit && ns[i] > biggestN) {
            biggestN = ns[i];
            biggestTime = times[i];
        }
    }
    if (biggestN != -1) {
        std::cout << "Largest problem size solvable within 30s: n = " << biggestN
                  << " (took " << std::fixed << std::setprecision(4) << biggestTime << " s)\n";
    }

    return 0;
}
