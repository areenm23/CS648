import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os

file = "rumor_spread_results.csv"

output_dir = "plots"
os.makedirs(output_dir, exist_ok=True)

df = pd.read_csv(file)

n = df["n"].values
rounds = df["average_rounds"].values
loglog = df["log_log_n"].values

name = file.split(".")[0]

# ---- BEST FIT LINE ----
coeffs = np.polyfit(loglog, rounds, 1)
slope, intercept = coeffs

# predicted values
rounds_fit = slope * loglog + intercept

# R^2 calculation
ss_res = np.sum((rounds - rounds_fit) ** 2)
ss_tot = np.sum((rounds - np.mean(rounds)) ** 2)
r2 = 1 - (ss_res / ss_tot)

# ---- PLOT ----
plt.figure()
plt.scatter(loglog, rounds, label="Data")
plt.plot(loglog, rounds_fit, label="Best fit")

plt.xlabel("log_log_n")
plt.ylabel("average_rounds")
plt.title(f"{name}: rounds vs loglog(n)\nR² = {r2:.4f}")

plt.legend()
plt.savefig(f"{output_dir}/{name}_loglog_fit.png")
plt.close()

# ---- PRINT EQUATION ----
print("Best fit line:")
print(f"average_rounds = {slope:.4f} * log_log_n + {intercept:.4f}")
print(f"R^2 = {r2:.4f}")