import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression

df = pd.read_csv("output.csv")

x = df["m"].values.reshape(-1, 1)
y = df["average_rounds"].values

X_log = np.log(x)


model = LinearRegression()
model.fit(X_log, y)

a = model.coef_[0]
b = model.intercept_

print(f"Best fit line: y = {a:.4f} log(m) + {b:.4f}")


x_fit = np.linspace(x.min(), x.max(), 200).reshape(-1, 1)
y_fit = model.predict(np.log(x_fit))


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