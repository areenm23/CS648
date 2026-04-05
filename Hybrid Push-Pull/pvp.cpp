#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;
using u64 = uint64_t;

const int N = 10000000;
const int ITER = 20;

struct Xoshiro256pp {
    u64 s[4];

    static inline u64 rotl(const u64 x, int k) {
        return (x << k) | (x >> (64 - k));
    }

    u64 next() {
        const u64 result = rotl(s[0] + s[3], 23) + s[0];

        const u64 t = s[1] << 17;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;
        s[3] = rotl(s[3], 45);

        return result;
    }

    void seed(u64 seed_val) {
        auto splitmix64 = [](u64 &x) {
            u64 z = (x += 0x9e3779b97f4a7c15ULL);
            z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
            z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
            return z ^ (z >> 31);
        };

        u64 x = seed_val;
        for (int i = 0; i < 4; i++) s[i] = splitmix64(x);
    }

    inline int next_int(int n) {
        return next() % n;
    }
};


struct Result {
    int days;
    double avg_infection_time;
    vector<long long> useful_calls;
    vector<long long> wasted_calls;
};

Result run_push(Xoshiro256pp &rng) {
    vector<char> informed(N, 0);
    vector<int> infection_time(N, -1);

    informed[0] = 1;
    infection_time[0] = 0;

    int informed_count = 1;
    int day = 0;

    vector<long long> useful, wasted;

    while (informed_count < N) {
        long long useful_today = 0, wasted_today = 0;

        vector<int> newly_informed;

        for (int i = 0; i < N; i++) {
            if (!informed[i]) continue;

            int target = rng.next_int(N);

            if (!informed[target]) {
                newly_informed.push_back(target);
                useful_today++;
            } else {
                wasted_today++;
            }
        }

        for (int v : newly_informed) {
            if (!informed[v]) {
                informed[v] = 1;
                infection_time[v] = day + 1;
                informed_count++;
            }
        }

        useful.push_back(useful_today);
        wasted.push_back(wasted_today);

        day++;
    }

    double avg_time = 0;
    for (int i = 0; i < N; i++) avg_time += infection_time[i];
    avg_time /= N;

    return {day, avg_time, useful, wasted};
}

Result run_pull(Xoshiro256pp &rng) {
    vector<char> informed(N, 0);
    vector<int> infection_time(N, -1);

    informed[0] = 1;
    infection_time[0] = 0;

    int informed_count = 1;
    int day = 0;

    vector<long long> useful, wasted;

    while (informed_count < N) {
        long long useful_today = 0, wasted_today = 0;

        vector<int> newly_informed;

        for (int i = 0; i < N; i++) {
            if (informed[i]) continue;

            int target = rng.next_int(N);

            if (informed[target]) {
                newly_informed.push_back(i);
                useful_today++;
            } else {
                wasted_today++;
            }
        }

        for (int v : newly_informed) {
            if (!informed[v]) {
                informed[v] = 1;
                infection_time[v] = day + 1;
                informed_count++;
            }
        }

        useful.push_back(useful_today);
        wasted.push_back(wasted_today);

        day++;
    }

    double avg_time = 0;
    for (int i = 0; i < N; i++) avg_time += infection_time[i];
    avg_time /= N;

    return {day, avg_time, useful, wasted};
}

int main() {
    ofstream push_file("push_results.csv");
    ofstream pull_file("pull_results.csv");

    push_file << "iter,days,avg_infection_time,day,useful_calls,wasted_calls\n";
    pull_file << "iter,days,avg_infection_time,day,useful_calls,wasted_calls\n";

    for (int it = 0; it < ITER; it++) {
        Xoshiro256pp rng;
        rng.seed(1234 + it);

        auto res_push = run_push(rng);
        for (int d = 0; d < res_push.useful_calls.size(); d++) {
            push_file << it << "," << res_push.days << "," << res_push.avg_infection_time
                      << "," << d << "," << res_push.useful_calls[d]
                      << "," << res_push.wasted_calls[d] << "\n";
        }

        rng.seed(5678 + it);

        auto res_pull = run_pull(rng);
        for (int d = 0; d < res_pull.useful_calls.size(); d++) {
            pull_file << it << "," << res_pull.days << "," << res_pull.avg_infection_time
                      << "," << d << "," << res_pull.useful_calls[d]
                      << "," << res_pull.wasted_calls[d] << "\n";
        }
    }

    push_file.close();
    pull_file.close();

    return 0;
}