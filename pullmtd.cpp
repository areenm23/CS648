#include <bits/stdc++.h>
#include <random>
#include <cstdint>
using namespace std;
using namespace std::chrono;

/* ---------------- RNG (as requested) ---------------- */

uint32_t randomNumber() {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<uint32_t> dist(
        0, std::numeric_limits<uint32_t>::max()
    );
    return dist(rng);
}

inline int randInt(int n) {
    return randomNumber() % n;
}

/* ---------------- Adaptive iterations ---------------- */

int iterations_for_n(int n) {
    if (n <= 1000)        return 1000;
    if (n <= 10000)       return 400;
    if (n <= 100000)      return 150;
    if (n <= 1000000)     return 80;
    if (n <= 10000000)    return 50;
    if (n <= 100000000)   return 25;
    return 10;
}

/* ---------------- PULL rumor spreading ---------------- */

void run_pull(
    int n,
    int &days,
    long long &total_calls,
    long long &useful_calls,
    long long &wasted_calls
) {
    vector<char> informed(n, 0);
    informed[0] = 1;
    int informed_count = 1;

    days = 0;
    total_calls = useful_calls = wasted_calls = 0;

    while (informed_count < n) {
        days++;
        vector<int> newly_informed;

        for (int u = 0; u < n; u++) {
            if (informed[u]) continue;

            int v;
            do {
                v = randInt(n);
            } while (v == u);

            total_calls++;

            if (informed[v]) {
                useful_calls++;
                newly_informed.push_back(u);
            } else {
                wasted_calls++;
            }
        }

        for (int u : newly_informed) {
            if (!informed[u]) {
                informed[u] = 1;
                informed_count++;
            }
        }
    }
}

/* ---------------- Main driver (averages only) ---------------- */

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int n_start = 1500000;
    const int n_max   = 200000000;   // ⚠️ be careful with RAM
    const double growth = 1.3;

    ofstream out("rumor_pull_results_avg.csv");
    out << "n,iterations,avg_days,avg_total_calls,avg_useful_calls,avg_wasted_calls,avg_runtime_ms\n";

    for (int n = n_start; n <= n_max; n = int(ceil(n * growth))) {
        int m = iterations_for_n(n);

        long double sum_days = 0;
        long double sum_total_calls = 0;
        long double sum_useful_calls = 0;
        long double sum_wasted_calls = 0;
        long double sum_runtime = 0;

        for (int it = 0; it < m; it++) {
            int days;
            long long total_calls, useful_calls, wasted_calls;

            auto t1 = steady_clock::now();
            run_pull(n, days, total_calls, useful_calls, wasted_calls);
            auto t2 = steady_clock::now();

            long long runtime_ms =
                duration_cast<milliseconds>(t2 - t1).count();

            sum_days += days;
            sum_total_calls += total_calls;
            sum_useful_calls += useful_calls;
            sum_wasted_calls += wasted_calls;
            sum_runtime += runtime_ms;
        }

        out << n << ","
            << m << ","
            << (sum_days / m) << ","
            << (sum_total_calls / m) << ","
            << (sum_useful_calls / m) << ","
            << (sum_wasted_calls / m) << ","
            << (sum_runtime / m) << "\n";

        cerr << "Finished n = " << n
             << " (iterations = " << m << ")\n";
    }

    out.close();
    return 0;
}
