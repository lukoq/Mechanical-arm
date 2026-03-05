# Mechanical-Arm height-controller

This program implements a simple height controller for a mechanical robotic arm. This program implements a simple height controller for a mechanical robot arm. It continuously changes the position of the lower arm, while the upper arm tries to maintain a constant value calculated by the `caluculatedBeta()` function.

![ezgif-6121b9c48d443996](https://github.com/user-attachments/assets/9e8de3c6-b828-436e-8a87-25d5a2ebdcf7)

## Logic

$$
y = H + L1 * \sin(\theta_1) + L2 * \sin(\theta_2)
$$

$$
L2 * \sin(\theta_2) = y - H - L1 * \sin(\theta_1)
$$

$$
sin(\theta_2) = \frac{y - H - L1 * \sin(\theta_1)}{L2}
$$

$$
\theta_2 = \arcsin(\frac{y - H - L1 * \sin(\theta_1)}{L2})
$$

In the code
```c
int calculateBeta(float y, int a) {

  float theta1 = (100.0 + 90.0 - a) * PI / 180.0;
  float sinTheta2 = (y - H - L1 * sin(theta1)) / L2;
  
  if(sinTheta2 < -1.0 || sinTheta2 > 1.0){
    return -1;
  } //I check if sinus within in range <-1.0, 1.0>
  
  float theta2 = asin(sinTheta2);
  int b = round(degrees(theta2) + 135.0);
  
  return b;
}
```
