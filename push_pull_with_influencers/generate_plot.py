import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load Excel file
file_path = r"C:\Users\vikas saini\Downloads\data3.xlsx"
df = pd.read_excel(file_path)

# Extract columns
n = df["n"]
avg_rounds = df["avg_days"]

# Compute 5*log(log(n))
loglog_n = 5 * np.log(np.log(n))

# Plot
plt.figure()
plt.plot(n, avg_rounds, marker='o', label="avg_rounds")
plt.plot(n, loglog_n, marker='x', label="5 log(log n)")

# Labels
plt.xlabel("n")
plt.ylabel("Value")
plt.title("avg_rounds vs n")

# Grid and legend
plt.grid(True)
plt.legend()

# Show plot
plt.show()