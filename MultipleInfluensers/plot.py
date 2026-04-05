import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression

# Load data
df = pd.read_csv("planted_influence_results.csv")

x = df["m"].values.reshape(-1, 1)
y = df["average_rounds"].values

# Transform x -> log(m)
X_log = np.log(x)

# Fit linear model: y = a log(m) + b
model = LinearRegression()
model.fit(X_log, y)

a = model.coef_[0]
b = model.intercept_

print(f"Best fit line: y = {a:.4f} log(m) + {b:.4f}")

# Generate smooth curve for plotting
x_fit = np.linspace(x.min(), x.max(), 200).reshape(-1, 1)
y_fit = model.predict(np.log(x_fit))

# Plot
plt.figure()
plt.scatter(x, y, label="Data")
plt.plot(x_fit, y_fit, label=f"Fit: y = {a:.2f} log(m) + {b:.2f}")

plt.xscale("log")
plt.xlabel("m")
plt.ylabel("Average Rounds")
plt.title("Average Rounds vs m")
plt.legend()
plt.grid()
plt.show()