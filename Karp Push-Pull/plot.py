import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os

file = "output.csv"
df = pd.read_csv(file)

# ---------- Aggregate ----------
agg = df.groupby("n").agg(
    avg_rounds=("avg_rounds", "mean"),
    std_rounds=("avg_rounds", "std"),
    min_rounds=("avg_rounds", "min"),
    max_rounds=("avg_rounds", "max"),
    avg_calls=("avg_calls", "mean"),
    std_calls=("avg_calls", "std"),
).reset_index()

# Add theoretical transforms
agg["log_n"] = np.log(agg["n"])
agg["loglog_n"] = np.log(np.log(agg["n"]))
agg["calls_per_n"] = agg["avg_calls"] / agg["n"]

# ---------- Create output dir ----------
out_dir = "plots"
os.makedirs(out_dir, exist_ok=True)

# ---------- Plot helper ----------
def save_plot(name):
    plt.tight_layout()
    plt.savefig(os.path.join(out_dir, name), dpi=200)
    plt.close()


plt.figure()
plt.errorbar(
    agg["log_n"],
    agg["avg_rounds"],
    yerr=agg["std_rounds"],
    fmt='o',
    capsize=4
)
plt.xlabel("log(n)")
plt.ylabel("Rounds")
plt.title("Rounds with Std Dev")
save_plot("rounds_error.png")

print("All plots saved in 'plots/' directory")