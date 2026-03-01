# Mechanical Arm path-planner

The program sets the mechanical arm frame at the specified coordinates in the given order. 
Each position is defined as a 3D coordinate vector consisting of:

* **Distance** (cm) – radial distance from the base
* **Height** (cm) – vertical position
* **Rotation** (degrees) – base rotation angle

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

The height of my arm is 6.5 cm with both joints connected at 8 cm each.

First, I calculate the relative distance of my arm to the target point based on the Pythagorean theorem. The point `(0, 0)` is exactly 6.5 cm below the axis of rotation of the bottom servo.  

$$
D = \sqrt{x^2 + y^2}
$$

Where:
- $D$ – distance to the target point  
- $x$ – horizontal distance to the point  
- $y$ – vertical distance to the point  

In the code
```c
float relY = y - H;
float distSq = x*x + relY*relY;
float dist = sqrt(distSq);

if(dist > (L1 + L2) || dist < abs(L1 - L2)) {
  return false;
} //I check the distance in case it is too short or too long
```
