import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

push_df = pd.read_csv("push_results.csv")
pull_df = pd.read_csv("pull_results.csv")

def compute_percentage(df):
    df = df.copy()
    total = df["useful_calls"] + df["wasted_calls"]
    df["useful_pct"] = df["useful_calls"] / total
    df["wasted_pct"] = df["wasted_calls"] / total
    return df

push_df = compute_percentage(push_df)
pull_df = compute_percentage(pull_df)

push_group = push_df.groupby("day").mean().reset_index()
pull_group = pull_df.groupby("day").mean().reset_index()

max_day = max(push_group["day"].max(), pull_group["day"].max())

push_group["x"] = push_group["day"] / max_day
pull_group["x"] = pull_group["day"] / max_day

x_dense = np.linspace(0, 1, 1000)

push_useful = np.interp(x_dense, push_group["x"], push_group["useful_pct"])
pull_useful = np.interp(x_dense, pull_group["x"], pull_group["useful_pct"])

push_wasted = np.interp(x_dense, push_group["x"], push_group["wasted_pct"])
pull_wasted = np.interp(x_dense, pull_group["x"], pull_group["wasted_pct"])

def find_intersection(x, y1, y2):
    diff = y1 - y2
    idx = np.where(np.diff(np.sign(diff)))[0]

    if len(idx) == 0:
        return None, None

    i = idx[0]

    x1, x2 = x[i], x[i+1]
    y1_1, y1_2 = y1[i], y1[i+1]
    y2_1, y2_2 = y2[i], y2[i+1]

    t = (y2_1 - y1_1) / ((y1_2 - y1_1) - (y2_2 - y2_1))
    x_star = x1 + t * (x2 - x1)
    y_star = y1_1 + t * (y1_2 - y1_1)

    return x_star, y_star

ix_useful, iy_useful = find_intersection(x_dense, push_useful, pull_useful)
ix_wasted, iy_wasted = find_intersection(x_dense, push_wasted, pull_wasted)

plt.figure(figsize=(8,5))

plt.plot(x_dense, push_useful, label="PUSH")
plt.plot(x_dense, pull_useful, label="PULL")

if ix_useful is not None:
    plt.scatter(ix_useful, iy_useful, color="red", s=80, zorder=5)
    plt.axvline(ix_useful, linestyle="--", color="red")
    plt.axhline(iy_useful, linestyle="--", color="red")

    plt.text(ix_useful, iy_useful,
             f"({ix_useful:.3f}, {iy_useful:.3f})",
             fontsize=9, ha='left', va='bottom')

plt.xlabel("Normalized Time (t / T)")
plt.ylabel("Useful Calls Fraction")
plt.title("Useful Calls Efficiency (Push vs Pull)")
plt.legend()
plt.grid()

plt.savefig("useful_intersection.png", dpi=300)
plt.close()


plt.figure(figsize=(8,5))

plt.plot(x_dense, push_wasted, label="PUSH")
plt.plot(x_dense, pull_wasted, label="PULL")

if ix_wasted is not None:
    plt.scatter(ix_wasted, iy_wasted, color="red", s=80, zorder=5)
    plt.axvline(ix_wasted, linestyle="--", color="red")
    plt.axhline(iy_wasted, linestyle="--", color="red")

    plt.text(ix_wasted, iy_wasted,
             f"({ix_wasted:.3f}, {iy_wasted:.3f})",
             fontsize=9, ha='left', va='bottom')

plt.xlabel("Normalized Time (t / T)")
plt.ylabel("Wasted Calls Fraction")
plt.title("Wasted Calls (Push vs Pull)")
plt.legend()
plt.grid()

plt.savefig("wasted_intersection.png", dpi=300)
plt.close()

with open("intersection_analysis.txt", "w", encoding="utf-8") as f:

    f.write("=== Intersection Analysis ===\n\n")

    if ix_useful is not None:
        f.write(f"Useful intersection:\n")
        f.write(f"  time ≈ {ix_useful:.5f}\n")
        f.write(f"  efficiency ≈ {iy_useful:.5f}\n\n")

    if ix_wasted is not None:
        f.write(f"Wasted intersection:\n")
        f.write(f"  time ≈ {ix_wasted:.5f}\n")
        f.write(f"  fraction ≈ {iy_wasted:.5f}\n\n")

    f.write("Interpretation:\n")
    f.write(
        "Intersection marks where push and pull have equal efficiency.\n"
        "Before this, push dominates.\n"
        "After this, pull dominates.\n"
    )

print("Done: graphs + intersection marked clearly.")