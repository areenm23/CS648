#include <bits/stdc++.h>
using namespace std;

struct RNG{

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

        s[2]^=s[0];
        s[3]^=s[1];
        s[1]^=s[2];
        s[0]^=s[3];

        s[2]^=t;
        s[3]=rotl(s[3],45);

        return result;
    }

    inline uint32_t next_uint(uint32_t n){
        return next()%n;
    }
};

inline bool bit_get(const vector<uint64_t> &bits,uint32_t i){
    return bits[i>>6]&(1ULL<<(i&63));
}

inline void bit_set(vector<uint64_t> &bits,uint32_t i){
    bits[i>>6]|=(1ULL<<(i&63));
}

void simulate_push(
        uint32_t n,
        int runs,
        RNG &rng,
        vector<uint64_t> &bits,
        vector<uint32_t> &informed,
        vector<uint32_t> &new_nodes,
        vector<double> &avg_inf,
        vector<double> &avg_success,
        vector<double> &avg_calls
){

    const int MAX_ROUNDS = 100;

    avg_inf.assign(MAX_ROUNDS,0);
    avg_success.assign(MAX_ROUNDS,0);
    avg_calls.assign(MAX_ROUNDS,0);

    for(int r=0;r<runs;r++){

        fill(bits.begin(),bits.end(),0ULL);

        informed.clear();
        new_nodes.clear();

        bit_set(bits,0);
        informed.push_back(0);

        uint32_t informed_count=1;

        int round=0;

        while(informed_count<n && round<MAX_ROUNDS){

            new_nodes.clear();

            uint32_t current=informed.size();

            int success=0;

            for(uint32_t i=0;i<current;i++){

                uint32_t target=rng.next_uint(n);

                if(!bit_get(bits,target)){
                    bit_set(bits,target);
                    new_nodes.push_back(target);
                    success++;
                }
            }

            for(uint32_t v:new_nodes)
                informed.push_back(v);

            informed_count+=new_nodes.size();

            avg_inf[round]+=informed_count;
            avg_success[round]+=success;
            avg_calls[round]+=current;

            round++;
        }
    }

    for(int i=0;i<avg_inf.size();i++){
        avg_inf[i]/=runs;
        avg_success[i]/=runs;
        avg_calls[i]/=runs;
    }
}

void simulate_pull(
        uint32_t n,
        int runs,
        RNG &rng,
        vector<uint64_t> &bits,
        vector<uint32_t> &informed,
        vector<uint32_t> &new_nodes,
        vector<double> &avg_inf,
        vector<double> &avg_success,
        vector<double> &avg_calls
){

    const int MAX_ROUNDS = 100;

    avg_inf.assign(MAX_ROUNDS,0);
    avg_success.assign(MAX_ROUNDS,0);
    avg_calls.assign(MAX_ROUNDS,0);

    for(int r=0;r<runs;r++){

        fill(bits.begin(),bits.end(),0ULL);

        informed.clear();
        new_nodes.clear();

        bit_set(bits,0);
        informed.push_back(0);

        uint32_t informed_count=1;

        int round=0;

        while(informed_count<n && round<MAX_ROUNDS){

            new_nodes.clear();

            int success=0;

            for(uint32_t i=0;i<n;i++){

                if(bit_get(bits,i)) continue;

                uint32_t target=rng.next_uint(n);

                if(bit_get(bits,target)){
                    bit_set(bits,i);
                    new_nodes.push_back(i);
                    success++;
                }
            }

            for(uint32_t v:new_nodes)
                informed.push_back(v);

            informed_count+=new_nodes.size();

            avg_inf[round]+=informed_count;
            avg_success[round]+=success;
            avg_calls[round]+=n-informed_count+success;

            round++;
        }
    }

    for(int i=0;i<avg_inf.size();i++){
        avg_inf[i]/=runs;
        avg_success[i]/=runs;
        avg_calls[i]/=runs;
    }
}

int main(){

    const uint32_t n = 1000000;
    const int runs = 100;

    uint64_t seed=
        chrono::high_resolution_clock::now()
        .time_since_epoch().count();

    RNG rng(seed);

    vector<uint64_t> bits((n+63)/64);
    vector<uint32_t> informed;
    vector<uint32_t> new_nodes;

    informed.reserve(n);

    vector<double> push_inf,push_success,push_calls;
    vector<double> pull_inf,pull_success,pull_calls;

    simulate_push(n,runs,rng,bits,informed,new_nodes,
                  push_inf,push_success,push_calls);

    simulate_pull(n,runs,rng,bits,informed,new_nodes,
                  pull_inf,pull_success,pull_calls);

    ofstream push_out("push_daily.csv");
    ofstream pull_out("pull_daily.csv");

    push_out<<"round,informed,success_calls,total_calls\n";
    pull_out<<"round,informed,success_calls,total_calls\n";

    for(int i=0;i<push_inf.size();i++){

        push_out<<i<<","
                <<push_inf[i]<<","
                <<push_success[i]<<","
                <<push_calls[i]<<"\n";

        pull_out<<i<<","
                <<pull_inf[i]<<","
                <<pull_success[i]<<","
                <<pull_calls[i]<<"\n";
    }

    push_out.close();
    pull_out.close();
}