import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Read CSV
df = pd.read_csv(r"D:\results_exact.csv")

# Unique f values
f_values = sorted(df["f"].unique())

# Given constants (aligned with f values)
c_values = [1.73343, 2.10521, 2.69542, 3.91076, 7.55646]

# Create plot
plt.figure()

# Plot simulation curves
for i, f in enumerate(f_values):
    subset = df[df["f"] == f]
    
    x = subset["n"]
    y = subset["mean_rounds"]
    
    plt.plot(x, y, marker='o', label=f"Sim f={f}")

    # Plot theoretical curve
    x_theory = np.array(x)
    y_theory = c_values[i] * np.log2(x_theory)
    
    plt.plot(x_theory, y_theory, linestyle='--', label=f"{c_values[i]} log₂(n)")

# Labels
plt.xlabel("n (Number of Nodes)")
plt.ylabel("Rounds")
plt.title("Rounds vs n")

# Log scale for x-axis
plt.xscale("log")

# Legend & grid
plt.legend()
plt.grid()

# Show
plt.show()
