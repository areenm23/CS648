import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Read CSV file
df = pd.read_csv(r"D:\rumor_pull_results_avg.csv")

# Sort by n (important for smooth curve)
df = df.sort_values(by="n")

# Extract data
n = df["n"]
avg_days = df["avg_days"]

# Compute theoretical curve: 2 * log2(n)
n_array = np.array(n)
theory = 1.2 * np.log2(n_array)

# Plot
plt.figure()

# Actual data
plt.plot(n, avg_days, marker='o', label="Pull Model (Avg Days)")

# Theoretical curve
plt.plot(n_array, theory, linestyle='--', label=r"$1.2 \log_2(n)$")

# Labels and title
plt.xlabel("n (Number of Nodes)")
plt.ylabel("Average Days")
plt.title("Days vs n")

# Log scale for better visualization
plt.xscale("log")

# Grid and legend
plt.grid()
plt.legend()

# Show plot
plt.show()


