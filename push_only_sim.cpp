#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <algorithm>
#include <unordered_set>
using namespace std;
#define BAR_WIDTH 50
/*
 Push-only rumor spreading simulation

 Model:
 - N people
 - Initially 1 informed
 - Each day, every informed person contacts one random person
 - If target is uninformed, they become informed next day
 - Repeat until all are informed
 - Run numSimulations times
 - Output CSV:
   Day,TotalNewInformed,Denominator
*/
int simulateRumorSpread(int population, int k) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(0, population - 1);

    int days = 0;
    int totalInformed = 1;

    vector<bool> informed(population, false);
    informed[0] = true;

    while (totalInformed < population) {
        days++;

        vector<int> newlyInformed;

        for (int i = 0; i < population; i++) {
            if (!informed[i]) continue;

            unordered_set<int> contacted;

            while (contacted.size() < k) {
                int target = dist(gen);

                if (target == i) continue;

                if (contacted.insert(target).second) {
                    if (!informed[target])
                        newlyInformed.push_back(target);
                }
            }
        }

        for (int p : newlyInformed) {
            if (!informed[p]) {
                informed[p] = true;
                totalInformed++;
            }
        }
    }

    return days;
}
int main() {
   	int numSimulations = 10000, pop = 500, step = 500, fin = 10000; 
	int k[5] = {1, 2, 4, 8, 16}; double avg[5]= {0,0,0,0,0};
	ofstream file("rumor_output.txt");
	file << "N, k=1, k=2, k=4, k=8, k=16\n" << std::flush;
	for(int population = pop; population <= fin; population+=step)
	{
		avg[0]=0;avg[1]=0;avg[2]=0;avg[3]=0;avg[4]=0;
		for(int i=0; i<numSimulations; i++){
			for(int j=0; j<5; j++)
				avg[j] += simulateRumorSpread(population, k[j]);
			
			std::cout<<"\r[";
			for(int j=0; j<BAR_WIDTH * (i+1) / numSimulations;j++)
				std::cout<<"=";
			if(i+1<numSimulations) std::cout<<">";
			else std::cout<<"=";
			for(int j=BAR_WIDTH * (i+1) / numSimulations; j<BAR_WIDTH; j++)
				std::cout<<" ";
			std::cout<<"] "<<(i+1)<<"/"<<numSimulations<<" N="<<population;
			std::cout<<std::flush;
		}
		std::cout<<"\n";
		file << population <<", ";
		for(int j=0; j<5; j++){
			avg[j] = avg[j] / numSimulations;
			file << avg[j] << (j<4?", ":"\n");
		}
		file <<std::flush;
	}
	file.close();
	cout << "\nSimulation complete. Results written to rumor_output.txt\n";
	return 0;
}
