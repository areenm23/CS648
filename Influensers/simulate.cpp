#include <bits/stdc++.h>
using namespace std;

struct xoshiro256pp {
    uint64_t s[4];

    static uint64_t rotl(const uint64_t x, int k) {
        return (x << k) | (x >> (64 - k));
    }

    uint64_t next() {
        const uint64_t result = rotl(s[0] + s[3], 23) + s[0];

        const uint64_t t = s[1] << 17;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;
        s[3] = rotl(s[3], 45);

        return result;
    }

    // Uniform [0,1)
    double next_double() {
        return (next() >> 11) * (1.0 / (1ULL << 53));
    }
};

uint64_t splitmix64(uint64_t &x) {
    uint64_t z = (x += 0x9e3779b97f4a7c15);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    return z ^ (z >> 31);
}

xoshiro256pp init_rng(uint64_t seed) {
    xoshiro256pp rng;
    for (int i = 0; i < 4; i++) {
        rng.s[i] = splitmix64(seed);
    }
    return rng;
}

vector<double> generate_power_law_weights(int n, double beta, xoshiro256pp &rng) {
    vector<double> weights(n);
    for (int i = 0; i < n; i++) {
        double r = rng.next_double();
        weights[i] = pow(1.0 - r, -1.0 / (beta - 1));
    }
    return weights;
}

vector<vector<int>> generate_chung_lu_graph(int n, double beta, xoshiro256pp &rng) {
    auto weights = generate_power_law_weights(n, beta, rng);
    double W = accumulate(weights.begin(), weights.end(), 0.0);

    vector<vector<int>> G(n);

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double p = min(weights[i] * weights[j] / W, 1.0);
            if (rng.next_double() < p) {
                G[i].push_back(j);
                G[j].push_back(i);
            }
        }
    }

    return G;
}

vector<int> get_giant_component(const vector<vector<int>> &G) {
    int n = G.size();
    vector<bool> visited(n, false);
    vector<int> largest;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            vector<int> comp;
            queue<int> q;
            q.push(i);
            visited[i] = true;

            while (!q.empty()) {
                int u = q.front(); q.pop();
                comp.push_back(u);

                for (int v : G[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }

            if (comp.size() > largest.size())
                largest = comp;
        }
    }
    return largest;
}

int rumor_spread_push_pull(const vector<vector<int>> &G, xoshiro256pp &rng) {
    vector<int> giant = get_giant_component(G);
    int n = giant.size();

    unordered_set<int> informed;

    int start = giant[rng.next() % n];
    informed.insert(start);

    int rounds = 0;

    while ((int)informed.size() < 0.9 * n) {
        unordered_set<int> new_informed = informed;

        for (int u : informed) {
            if (!G[u].empty()) {
                int v = G[u][rng.next() % G[u].size()];
                new_informed.insert(v);
            }
        }

        for (int u : giant) {
            if (!informed.count(u)) {
                if (!G[u].empty()) {
                    int v = G[u][rng.next() % G[u].size()];
                    if (informed.count(v)) {
                        new_informed.insert(u);
                    }
                }
            }
        }

        informed = move(new_informed);
        rounds++;
    }

    return rounds;
}

vector<int> experiment(int n, double beta, int trials, xoshiro256pp &rng) {
    vector<int> results;

    for (int i = 0; i < trials; i++) {
        auto G = generate_chung_lu_graph(n, beta, rng);
        int rounds = rumor_spread_push_pull(G, rng);
        results.push_back(rounds);
    }

    return results;
}

int main() {
    double beta = 2.5;

    uint64_t seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    auto rng = init_rng(seed);

    ofstream file("output.csv");
    file << "n,average_rounds,log_log_n,ratio\n";

    int i = 547252*1.3;

    while (i < 10000000) {
        int n = i;
        cout << "\nRunning experiment for n = " << n << endl;

        int trials;
        if (n < 10000) trials = 50;
        else if (n < 100000) trials = 30;
        else trials = 10;

        auto rounds_list = experiment(n, beta, trials, rng);

        double avg = accumulate(rounds_list.begin(), rounds_list.end(), 0.0) / trials;
        double loglogn = log(log(n));
        double ratio = avg / loglogn;

        cout << "Average rounds: " << avg << endl;
        cout << "log log n: " << loglogn << endl;

        file << n << "," << avg << "," << loglogn << "," << ratio << "\n";

        file.flush();

        i = int(i * 1.15);
    }

    file.close();

    cout << "\nResults saved to rumor_spread_results.csv\n";

    return 0;
}