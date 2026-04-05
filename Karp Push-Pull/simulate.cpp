#include <bits/stdc++.h>
using namespace std;

struct RNG {
    uint64_t s[4];

    static inline uint64_t rotl(uint64_t x,int k){
        return (x<<k)|(x>>(64-k));
    }

    static uint64_t splitmix64(uint64_t &x){
        uint64_t z=(x+=0x9e3779b97f4a7c15ULL);
        z=(z^(z>>30))*0xbf58476d1ce4e5b9ULL;
        z=(z^(z>>27))*0x94d049bb133111ebULL;
        return z^(z>>31);
    }

    RNG(uint64_t seed){
        for(int i=0;i<4;i++) s[i]=splitmix64(seed);
    }

    inline uint64_t next(){
        const uint64_t result=rotl(s[0]+s[3],23)+s[0];
        const uint64_t t=s[1]<<17;

        s[2]^=s[0]; s[3]^=s[1];
        s[1]^=s[2]; s[0]^=s[3];

        s[2]^=t;
        s[3]=rotl(s[3],45);

        return result;
    }

    inline uint32_t next_uint(uint32_t n){
        return next() % n;
    }
};


struct Result {
    int rounds;
    long long calls;
};

Result simulate(uint32_t n, RNG &rng, vector<int> &state){

    fill(state.begin(), state.end(), -1);

    state[0] = 0; // source
    uint32_t informed = 1;

    int rounds = 0;
    long long calls = 0;

    int MAX_AGE = (int)(log2(log2(n)) + 5);

    vector<int> next_state(n);

    while(informed < n){

        rounds++;
        next_state = state;

        for(uint32_t i=0;i<n;i++){

            uint32_t j = rng.next_uint(n);
            calls++;

            int si = state[i];
            int sj = state[j];

            if(si >= 0 && sj == -1){
                next_state[j] = 0;
            }
            if(sj >= 0 && si == -1){
                next_state[i] = 0;
            }
        }

        for(uint32_t i=0;i<n;i++){
            if(next_state[i] >= 0){
                if(next_state[i] < MAX_AGE){
                    next_state[i]++;
                }
            }
        }

        state.swap(next_state);

        informed = 0;
        for(uint32_t i=0;i<n;i++){
            if(state[i] >= 0) informed++;
        }
    }

    return {rounds, calls};
}


int get_runs(uint32_t n){
    if(n < 1e4) return 100;
    if(n < 1e6) return 50;
    if(n < 1e7) return 20;
    if(n < 1e8) return 10;
    return 5;
}


int main(){

    uint64_t seed = chrono::high_resolution_clock::now()
        .time_since_epoch().count();

    RNG rng(seed);

    ofstream out("rumor_push_pull_loglog.csv");
    out<<"n,avg_rounds,avg_calls\n";

    uint64_t n = 100;

    while(n <= 1500000000ULL){

        cout<<"n = "<<n<<"\n";

        int runs = get_runs(n);

        vector<int> state(n);

        long long total_calls = 0;
        long long total_rounds = 0;

        for(int r=0;r<runs;r++){

            Result res = simulate(n, rng, state);

            total_calls += res.calls;
            total_rounds += res.rounds;
        }

        double avg_calls = (double)total_calls / runs;
        double avg_rounds = (double)total_rounds / runs;

        out<<n<<","<<avg_rounds<<","<<avg_calls<<"\n"<<std::flush;

        n = (uint64_t)(n * 1.3);
    }

    out.close();
}