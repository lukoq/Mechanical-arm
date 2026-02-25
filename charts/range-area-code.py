import matplotlib.pyplot as plt
import numpy as np

# Data
alpha = np.array([45, 60, 75, 90, 105, 120, 135, 140, 150, 160, 170, 175, 180])
beta_min = np.array([135, 120, 105, 90, 75, 60, 45, 40, 40, 40, 40, 60, 70])
beta_max = np.array([180, 180, 180, 180, 180, 180, 175, 170, 160, 150, 140, 135, 130])

plt.figure(figsize=(10, 6))

plt.fill_between(alpha, beta_min, beta_max, color='lightgreen', alpha=0.5, label='Safe zone')

# Limits
plt.plot(alpha, beta_min, 'r--', label='Lower limit')
plt.plot(alpha, beta_max, 'b--', label='Upper limit')
plt.scatter(alpha, beta_min, color='red', s=30)
plt.scatter(alpha, beta_max, color='blue', s=30)

# Phases
plt.annotate('Due to collision with the lower leg', xy=(90, 90), xytext=(60, 60),
             arrowprops=dict(facecolor='black', shrink=0.05))

plt.annotate('Due to lack of servo range', xy=(155, 40), xytext=(145, 20),
             arrowprops=dict(facecolor='red', shrink=0.05))

plt.annotate('Due to collision with the floor', xy=(178, 65), xytext=(150, 100),
             arrowprops=dict(facecolor='brown', shrink=0.05))

plt.title('C-space range map : α (Bottom servo) vs β (Top servo)')
plt.xlabel('Bottom servo angle (degrees)')
plt.ylabel('Top servo angle (degrees)')
plt.xlim(40, 185)
plt.ylim(0, 190)
plt.grid(True, linestyle=':', alpha=0.6)
plt.legend()

plt.show()
