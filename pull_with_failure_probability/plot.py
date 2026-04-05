import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv("output.csv")
f_values = sorted(df["f"].unique())
c_values = [1.73343, 2.10521, 2.69542, 3.91076, 7.55646]
plt.figure()

for i, f in enumerate(f_values):
    subset = df[df["f"] == f]
    x = subset["n"]
    y = subset["mean_rounds"]
    
    plt.plot(x, y, marker='o', label=f"Sim f={f}")
    x_theory = np.array(x)
    y_theory = c_values[i] * np.log2(x_theory)
    plt.plot(x_theory, y_theory, linestyle='--', label=f"{c_values[i]} log₂(n)")

plt.xlabel("n (Number of Nodes)")
plt.ylabel("Rounds")
plt.title("Rounds vs n")
plt.xscale("log")

plt.legend()
plt.grid()
plt.show()
df['x'] = 1 / (1 - df['f'])

n_counts = df['n'].value_counts()
best_n = n_counts.idxmax()

print(f"Using n = {best_n} (has {n_counts.max()} data points)")

subset = df[df['n'] == best_n].sort_values('x')
x = subset['x'].values
y = subset['mean_rounds'].values

m, c = np.polyfit(x, y, 1)
print(f"Best fit line: y = {m:.4f}x + {c:.4f}")

x_fit = np.linspace(min(x), max(x), 200)
y_fit = m * x_fit + c

plt.figure()
plt.plot(x, y, 'o', label='Data')
plt.plot(x_fit, y_fit, label=f'Fit: y = {m:.2f}x + {c:.2f}')
plt.xlabel('1 / (1 - f)')
plt.ylabel('Mean Rounds')
plt.title(f'Mean Rounds vs 1/(1-f) for n = {best_n}')
plt.legend()
plt.grid()

plt.show()