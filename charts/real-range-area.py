import matplotlib.pyplot as plt
import numpy as np

H = 6.5  # Height
L1 = 8.0  # Lower leg length
L2 = 10.0  # Upper leg length

# Data
data = [
    (45, 135, 180), (60, 120, 180), (75, 105, 180), (90, 90, 180),
    (105, 75, 180), (120, 60, 180), (135, 45, 175), (140, 40, 170),
    (150, 40, 160), (160, 40, 150), (170, 40, 140), (175, 60, 135),
    (180, 70, 130)
]


def get_coords(alpha, beta):
    t1 = np.radians(195 - alpha)
    t2 = np.radians(beta - 135)

    X = L1 * np.cos(t1) + L2 * np.cos(t2)
    Y = H + L1 * np.sin(t1) + L2 * np.sin(t2)
    return X, Y


plt.figure(figsize=(10, 6))

for alpha, b_min, b_max in data:
    betas = np.linspace(b_min, b_max, 20)
    xs, ys = [], []
    for b in betas:
        x, y = get_coords(alpha, b)
        if y >= 0:
            xs.append(x)
            ys.append(y)
    plt.plot(xs, ys, 'g.', alpha=0.3)

plt.axhline(0, color='red', lw=2, label='Desk')
plt.plot(0, 7, 'ko', label='Bottom servo position (~6.5 cm above the desk)')
plt.vlines(0, 0, 7, colors='black', linestyles='--')




plt.title("Real range map area")
plt.xlabel("Distance [cm]")
plt.ylabel("Height [cm]")
plt.axis('equal')
plt.grid(True)
plt.legend()
plt.show()
