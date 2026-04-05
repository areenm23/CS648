#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

struct Xoshiro256PlusPlus {
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
 Xoshiro256PlusPlus() {
 std::random_device rd;
 for (int i = 0; i < 4; i++) s[i] = rd();
 }
};
static Xoshiro256PlusPlus rng;
inline int randInt(int n) {
 return rng.next() % n;
}
struct Stats {
 long double sum = 0, sum_sq = 0;
 long double mn = 1e18, mx = -1e18;
 int cnt = 0;
 void add(long double x) {
 sum += x;
 sum_sq += x * x;
 mn = min(mn, x);
 mx = max(mx, x);
 cnt++;
 }
 long double mean() const { return sum / cnt; }
 long double stddev() const {
 long double m = mean();
 return sqrt(max((long double)0, sum_sq / cnt - m * m));
 }
};
void run_pull(int n,
 int &days,
 long long &total_calls,
 long long &useful_calls,
 long long &wasted_calls,
 vector<int> &infection_day)
{
 vector<char> informed(n, 0);
 informed[0] = 1;
 infection_day.assign(n, -1);
 infection_day[0] = 0;
 int informed_count = 1;
 days = 0;
 total_calls = useful_calls = wasted_calls = 0;
 while (informed_count < n) {
 days++;
 vector<int> newly;
 for (int u = 0; u < n; u++) {
 if (informed[u]) continue;
 int v;
 do { v = randInt(n); } while (v == u);
 total_calls++;
 if (informed[v]) {
 useful_calls++;
 newly.push_back(u);
 } else {
 wasted_calls++;
 }
 }
 for (int u : newly) {
 if (!informed[u]) {
 informed[u] = 1;
 infection_day[u] = days;
 informed_count++;
 }
 }
 }
}
void run_push(int n,
 int &days,
 long long &total_calls,
 long long &useful_calls,
 long long &wasted_calls,
 vector<int> &infection_day)
{
 vector<char> informed(n, 0);
 informed[0] = 1;
 infection_day.assign(n, -1);
 infection_day[0] = 0;
 int informed_count = 1;
 days = 0;
 total_calls = useful_calls = wasted_calls = 0;
 while (informed_count < n) {
 days++;
 vector<int> newly;
 for (int u = 0; u < n; u++) {
 if (!informed[u]) continue;
 int v;
 do { v = randInt(n); } while (v == u);
 total_calls++;
 if (!informed[v]) {
 useful_calls++;
 newly.push_back(v);
 } else {
 wasted_calls++;
 }
 }
 for (int v : newly) {
 if (!informed[v]) {
 informed[v] = 1;
 infection_day[v] = days;
 informed_count++;
 }
 }
 }
}
int get_runs(uint32_t n){
 if(n < 1e4) return 100;
 if(n < 1e6) return 50;
 if(n < 1e7) return 20;
 if(n < 1e8) return 10;
 return 5;
}

int main() {
 ios::sync_with_stdio(false);
 cin.tie(nullptr);
 ofstream out("rumor_push_pull_clean.csv");
 out << "n,algo,"
 << "Y_mean,Y_min,Y_max,Y_std,"
 << "C_mean,C_min,C_max,C_std,"
 << "X_mean,X_min,X_max,X_std\n";
 for (int n = 690443973*1.3; n <= 3000000000; n = int(ceil(n * 1.3))) {
 int iterations = get_runs(n);
 for (int mode = 0; mode < 2; mode++) {
 Stats Y, C, X;
 for (int it = 0; it < iterations; it++) {
 int days;
 long long total, useful, wasted;
 vector<int> inf_day;
 if (mode == 0)
 run_push(n, days, total, useful, wasted, inf_day);
 else
 run_pull(n, days, total, useful, wasted, inf_day);
 
 Y.add(days);
 
 C.add(total);
 
 for (int i = 1; i < n; i++) {
 X.add(inf_day[i]);
 }
 }
 out << n << ","
 << (mode == 0 ? "push" : "pull") << ","
 << Y.mean() << ","
 << Y.mn << ","
 << Y.mx << ","
 << Y.stddev() << ","
 << C.mean() << ","
 << C.mn << ","
 << C.mx << ","
 << C.stddev() << ","
 << X.mean() << ","
 << X.mn << ","
 << X.mx << ","
 << X.stddev() << "\n"<<std::flush;
 }
 cerr << "Done n = " << n << "\n";
 }
 out.close();
}