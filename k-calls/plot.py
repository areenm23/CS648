import numpy as np
import matplotlib.pyplot as plt
import re
filename = "output.txt"
with open(filename) as f:
    lines = [line.strip() for line in f if line.strip()]
header = lines[0].split(",")
k_values = []
for h in header[1:]:
    m = re.search(r'k=(\d+)', h)
    if m:
        k_values.append(int(m.group(1)))
N = []
data = {k: [] for k in k_values}
for line in lines[1:]:
    parts = [p.strip() for p in line.split(",")]
    N.append(float(parts[0]))
    for i, k in enumerate(k_values):
        data[k].append(float(parts[i + 1]))
N = np.array(N)
plt.figure(figsize=(8,6))
logN = np.log(N)
for k in k_values:
    y = np.array(data[k])
    # best-fit y = a log(N) + b
    a, b = np.polyfit(logN, y, 1)
    print(f"k={k}: T(N) ≈ {a:.4f} log(N) + {b:.4f}")
    fit = a*logN + b
    plt.scatter(N, y, label=f"k={k} data")
    plt.plot(N, fit, linestyle="--", label=f"k={k} fit")
plt.xscale("log")
plt.xlabel("N")
plt.ylabel("Time / Rounds")
plt.title("Rumor Spreading vs N (Log-scale)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
