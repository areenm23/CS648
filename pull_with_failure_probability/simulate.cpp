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

        s[2]^=s[0];
        s[3]^=s[1];
        s[1]^=s[2];
        s[0]^=s[3];

        s[2]^=t;
        s[3]=rotl(s[3],45);

        return result;
    }

    /* random integer in [0,n) */
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

int repetitions(uint64_t n){

    if(n<=1e3) return 200;
    if(n<=1e4) return 150;
    if(n<=1e5) return 100;
    if(n<=1e6) return 60;
    if(n<=1e7) return 30;
    if(n<=5e7) return 15;
    return 8;
}

int simulate(uint32_t n,double f,RNG &rng,
             vector<uint64_t> &bits,
             vector<uint32_t> &informed,
             vector<uint32_t> &new_nodes){

    uint64_t fail_threshold=(uint64_t)(f*(long double)UINT64_MAX);

    fill(bits.begin(),bits.end(),0ULL);

    informed.clear();
    new_nodes.clear();

    bit_set(bits,0);
    informed.push_back(0);

    uint32_t informed_count=1;

    int rounds=0;

    while(informed_count<n){

        rounds++;
        new_nodes.clear();

        uint32_t current=informed.size();

        for(uint32_t i=0;i<current;i++){

            if(rng.next()<fail_threshold) continue;

            uint32_t target=rng.next_uint(n);

            if(!bit_get(bits,target)){
                bit_set(bits,target);
                new_nodes.push_back(target);
            }
        }

        for(uint32_t v:new_nodes)
            informed.push_back(v);

        informed_count+=new_nodes.size();
    }

    return rounds;
}

int main(){

    ios::sync_with_stdio(false);

    vector<double> fvals={0,0.2,0.4,0.6,0.8};

    vector<uint64_t> ns;

    double cur=100.0;

    while(cur<=200000000.0){
        ns.push_back((uint64_t)cur);
        cur*=1.2;
    }

    uint64_t seed=
        chrono::high_resolution_clock::now()
        .time_since_epoch().count();

    RNG rng(seed);

    uint32_t max_n=ns.back();

    vector<uint64_t> bits((max_n+63)/64);

    vector<uint32_t> informed;
    vector<uint32_t> new_nodes;

    informed.reserve(max_n);
    new_nodes.reserve(max_n/4);

    ofstream out("results_exact.csv");

    out<<"n,f,runs,mean_rounds,std_rounds,ratio_log2n,ratio_theory\n";

    for(uint64_t n:ns){

        double log2n=log2((double)n);

        int runs=repetitions(n);

        for(double f:fvals){

            vector<int> vals(runs);

            for(int i=0;i<runs;i++){
                vals[i]=simulate(n,f,rng,bits,informed,new_nodes);
            }

            double mean=0;

            for(int x:vals) mean+=x;

            mean/=runs;

            double var=0;

            for(int x:vals)
                var+=(x-mean)*(x-mean);

            var/=(runs>1 ? runs-1 : 1);

            double std=sqrt(var);

            double theory=log2n/(1.0-f);

            double ratio1=mean/log2n;
            double ratio2=mean/theory;

            cout<<"n="<<n
                <<" f="<<f
                <<" runs="<<runs
                <<" mean="<<mean
                <<" std="<<std
                <<endl;

            out<<n<<","
               <<f<<","
               <<runs<<","
               <<mean<<","
               <<std<<","
               <<ratio1<<","
               <<ratio2<<"\n";
        }

        cout<<"--------------------------------\n";
    }

    out.close();
}