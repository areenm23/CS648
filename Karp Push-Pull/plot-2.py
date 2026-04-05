import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression

# Load data
df = pd.read_csv("median_counter_exact.csv")

# Average over runs
df_avg = df.groupby("n")["calls"].mean().reset_index()

# Compute feature
df_avg["n_loglogn"] = df_avg["n"] * np.log(np.log(df_avg["n"]))

# Linear regression
X = df_avg[["n_loglogn"]]
y = df_avg["calls"]

model = LinearRegression().fit(X, y)
y_pred = model.predict(X)

print("Slope:", model.coef_[0])
print("Intercept:", model.intercept_)
print("R^2:", model.score(X, y))

# Plot
plt.figure()
plt.scatter(df_avg["n_loglogn"], df_avg["calls"], label="Data")
plt.plot(df_avg["n_loglogn"], y_pred, label="Best fit")
plt.xlabel("n log log n")
plt.ylabel("calls")
plt.title("Calls vs n log log n")
plt.legend()
plt.grid()
plt.show()