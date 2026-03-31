import numpy as np
import networkx as nx
import random
import math
import pandas as pd
from tqdm import tqdm

# ----------------------------
# 1. Generate Power-Law Weights
# ----------------------------
def generate_power_law_weights(n, beta, w_min=1):
    """
    Generate n samples from power-law distribution
    P(w) ~ w^(-beta)
    """
    r = np.random.random(n)
    weights = w_min * (1 - r) ** (-1 / (beta - 1))
    return weights


# ----------------------------
# 2. Generate Chung-Lu Graph
# ----------------------------
def generate_chung_lu_graph(n, beta):
    weights = generate_power_law_weights(n, beta)
    W = sum(weights)

    G = nx.Graph()
    G.add_nodes_from(range(n))

    for i in range(n):
        for j in range(i + 1, n):
            p = min(weights[i] * weights[j] / W, 1)
            if random.random() < p:
                G.add_edge(i, j)

    return G


# ----------------------------
# 3. Synchronous Rumor Spreading
# ----------------------------

def rumor_spread_push_pull(G):
    
    # Take giant component
    giant = max(nx.connected_components(G), key=len)
    G_sub = G.subgraph(giant).copy()

    n = G_sub.number_of_nodes()

    # Choose random start node
    start = random.choice(list(G_sub.nodes()))

    informed = set([start])
    rounds = 0

    while len(informed) < 0.9*n:
        new_informed = set(informed)

        # PUSH step (informed nodes push rumor)
        for u in informed:
            neighbors = list(G_sub.neighbors(u))
            if neighbors:
                v = random.choice(neighbors)
                new_informed.add(v)

        # PULL step (uninformed nodes pull rumor)
        uninformed = set(G_sub.nodes()) - informed
        for u in uninformed:
            neighbors = list(G_sub.neighbors(u))
            if neighbors:
                v = random.choice(neighbors)
                if v in informed:
                    new_informed.add(u)

        informed = new_informed
        rounds += 1

    return rounds, len(informed), n



# ----------------------------
# 4. Run Experiment
# ----------------------------
def experiment(n, beta, trials=5):
    results = []
    for _ in tqdm(range(trials)):
        G = generate_chung_lu_graph(n, beta)
        rounds, informed, total = rumor_spread_push_pull(G)
        results.append(rounds)
    return results


# ----------------------------
# 5. Example Run
# ----------------------------
if __name__ == "__main__":
    import math
    import numpy as np
    import pandas as pd

    beta = 2.5  # must satisfy 2 < beta < 3

    # list to store results
    results = []

    i = 100
    while i < 10_000_000:
        n = int(i)

        print(f"\nRunning experiment for n = {n}")
        trials = 0

        if n < 10000:
          trials = 50
        elif n < 100000:
           trials = 30
        else:
           trials = 10

        rounds_list = experiment(n, beta, trials)

        avg_rounds = np.mean(rounds_list)
        loglogn = math.log(math.log(n))
        ratio = avg_rounds / loglogn

        print("Rounds in each trial:", rounds_list)
        print("Average rounds:", avg_rounds)
        print("log log n =", loglogn)

        # store results
        results.append({
            "n": n,
            "average_rounds": avg_rounds,
            "log_log_n": loglogn,
            "avg_rounds/log_log_n": ratio
        })

        i = int(i * 1.15)

    # convert to dataframe
    df = pd.DataFrame(results)

    # save to excel
    df.to_excel("F:\\rumor_spread_results.xlsx", index=False)

    print("\nResults saved to rumor_spread_results.xlsx")