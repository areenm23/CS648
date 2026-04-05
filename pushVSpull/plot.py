import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Load the CSV file
df = pd.read_csv("rumor_push_pull_clean.csv")

# Get unique algorithms
algos = ['push', 'pull']

def fit_and_plot(x, y, label):
    logx = np.log(x)
    coeffs = np.polyfit(logx, y, 1)  # y = a*log(n) + b
    a, b = coeffs

    # Generate smooth line
    x_fit = np.linspace(min(x), max(x), 200)
    y_fit = a * np.log(x_fit) + b

    print(f"{label}: y = {a:.4f} * log(n) + {b:.4f}")
    return x_fit, y_fit

plt.figure()
for algo in algos:
    subset = df[df['algo'] == algo].sort_values('n')
    x = subset['n'].values
    y = subset['X_mean'].values

    plt.plot(x, y, 'o', label=f"{algo} data")

    x_fit, y_fit = fit_and_plot(x, y, f"X ({algo})")
    plt.plot(x_fit, y_fit, '-', label=f"{algo} fit")

plt.xscale('log')
plt.xlabel("n")
plt.ylabel("Average infection day (mean)")
plt.title("n vs X (with best fit)")
plt.legend()
plt.grid()
plt.show()

plt.figure()
for algo in algos:
    subset = df[df['algo'] == algo].sort_values('n')
    x = subset['n'].values
    y = subset['Y_mean'].values

    plt.plot(x, y, 'o', label=f"{algo} data")

    x_fit, y_fit = fit_and_plot(x, y, f"Y ({algo})")
    plt.plot(x_fit, y_fit, '-', label=f"{algo} fit")

plt.xscale('log')
plt.xlabel("n")
plt.ylabel("Number of rounds (mean)")
plt.title("n vs number of rounds")
plt.legend()
plt.grid()
plt.show()

plt.figure()
for algo in algos:
    subset = df[df['algo'] == algo].sort_values('n')
    x = subset['n'].values
    y = (subset['C_mean'] / subset['n']).values

    plt.plot(x, y, 'o', label=f"{algo} data")

    x_fit, y_fit = fit_and_plot(x, y, f"C ({algo})")
    plt.plot(x_fit, y_fit, '-', label=f"{algo} fit")

plt.xscale('log')
plt.xlabel("n")
plt.ylabel("Calls per node (mean)")
plt.title("n vs (total number of calls) / n (with best fit)")
plt.legend()
plt.grid()
plt.show()