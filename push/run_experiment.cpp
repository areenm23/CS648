#include <iostream>
#include <fstream>
#include <cstdlib>
#include <utility>
#include <string>
#include <set>
int main(){
    int N = 20, n=2, day=0;
    std::string dumpText="";
    std::set<int> nodesWithRumor;
    std::set<std::pair<int, int>> edges; // first < second always
    std::ofstream file("data.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the text file." << std::endl;
        return 1;
    }
    while (n<=N){
        nodesWithRumor.clear();
        nodesWithRumor.insert(1);
        day = 0; dumpText = "";
        while(nodesWithRumor.size() < n){
            day++;
            auto todaysNodes = nodesWithRumor;
            for(const int i : todaysNodes){
                int j = (rand() % (n-1)) + 1; 
                if(j>=i) j+= 1; // 1,...i-1,i+1,...,n
                dumpText += std::to_string(i) + " -> " + std::to_string(j) + " ";
                nodesWithRumor.insert(j);
                if (i<j)
                    edges.insert(std::make_pair(i, j));
                else if (j<i)
                    edges.insert(std::make_pair(j, i));
            }
            dumpText += "\n";
        }
        file << n << ", " << day << std::endl;
        file << dumpText;
        n++;
    }
    file.close();
    return 0;
}
/*
Example of how data is formatted for 4 nodes in 2 days
4, 2
1 -> 2
1 -> 3 2-> 4
*/