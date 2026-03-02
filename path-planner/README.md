# Mechanical-Arm path-planner

The program sets the mechanical arm frame at the specified coordinates in the given order. 
Each position is defined as a 3D coordinate vector consisting of:

* **Distance** (cm) – radial distance from the base
* **Height** (cm) – vertical position
* **Rotation** (degrees) – base rotation angle

In this example, I used this program to draw a specific pattern on paper.

![ezgif-5582c96486a57031](https://github.com/user-attachments/assets/2412837a-eb8a-4013-a47e-2ea9657ef9fd)

## Coordinate Definition

Target positions are stored in a 2D float array.
Each entry represents a single frame position in the following format:

```c
{ distance_cm, height_cm, rotation_deg }
```

### Example:

```c
float targetsArr[][3] = {
  {12.0, 0.0, 40.0},
  {12.0, 0.0, 170.0},
  ...
};
```

## Logic

The height of my arm is 6.5 cm with both joints ($L_1$, $L_2$) connected at 8 cm each.

First, I calculate the relative distance of my arm to the target point based on the Pythagorean theorem. The point `(0, 0)` is exactly 6.5 cm below the axis of rotation of the bottom servo.  

$$
D = \sqrt{x^2 + y^2}
$$

Where:
- $D$ – distance to the target point  
- $x$ – horizontal distance to the point  
- $y$ – vertical distance to the point  

We have a triangle built from three sides: the lower arm ($L_1$), the upper arm ($L_2$), and the distance to the target ($D$).

<img width="1280" height="720" alt="triangle" src="https://github.com/user-attachments/assets/0cf88d6f-7f6e-469c-9fd5-7e9ef42faf70" />

The angle $\theta_1$ is the angle between $L_1$ arm and the line parallel to the base.

$$
\theta_1 = \sigma + \omega 
$$

$$
L_2^2 = D^2 + L_1^2 - 2DL_1\cos(\sigma)
$$

$$
\cos(\sigma) = \frac{L_1^2 + L_2^2 - D^2}{2L_1L_2}
$$

$$
\tan(\omega) = \frac{y}{x}
$$

Therefore,

$$
\theta_1 = \arccos(\cos(\sigma)) + \arctan(\tan(\omega))
$$

The angle $\theta_2$ is the bend angle of both arms.

$$
D^2 = L_1^2 + L_2^2 - 2L_1L_2\cos(\theta_2)
$$

$$
\cos(\theta_2) = \frac{L_1^2 + L_2^2 - D^2}{2L_1L_2}
$$

$$
\theta_2 = \arccos(\cos(\theta_2))
$$

My servos MG90S are not calibrated correctly. The lower servo is set vertically at 100 degrees. The upper servo is set horizontally at 135 degrees. I was forced to add bias to them.

$$
\alpha = 90^\circ - (\theta_1 + 100^\circ)
$$

Furthermore, the upper servo mechanism is not dependent on the upper arm, so I have to set its angle relative to the horizontal base.

$$
\beta = (\theta_1 - \theta_2) + 135^\circ
$$

In the code
```c
bool moveToCoords(float x, float y, float gamma) {
  float relY = y - H;
  float distSq = x*x + relY*relY;
  float dist = sqrt(distSq);

  if(dist > (L1 + L2) || dist < abs(L1 - L2)) {
    return false;
  } //I check the distance in case it is too short or too long

  float cosT2 = constrain((distSq - L1*L1 - L2*L2)/(2.0*L1 * L2), -1.0, 1.0);
  float t2 = acos(cosT2);
  
  float cosT1_part = constrain((L1*L1 + distSq - L2*L2)/(2.0*L1 * dist), -1.0, 1.0);

  float t1 = atan2(relY, x) + acos(cosT1_part);
  int a = round(100.0 + 90.0 - degrees(t1));

  float t2_horizon = t1 - t2; 
  int b = round(degrees(t2_horizon) + 135.0);

  int g = constrain(round(gamma), 0, 180);
  a = constrain(a, 45, 180);
  b = applyBetaLimits(a, b); 

  targetAlpha = a;     
  targetBeta = b;
  targetGamma = g;

  return true;
}
```
