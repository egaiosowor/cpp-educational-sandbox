# 3D Solar System Viewer (The Mindblower)

Welcome to the ultimate advanced project! This project shatters the illusion that 3D programming is impossible for beginners. By utilizing Raylib's built-in 3D camera system, we can render spheres in 3D space with just a few lines of code.

![Solar System Viewer](app.png)

## About the Visualizer
This is a simple orbital mechanics simulator. It renders the Sun at the center of the universe, and dynamically calculates the orbital position of the Earth around the Sun, and the Moon around the Earth.

- **Controls:** Click and drag with your mouse to orbit the 3D camera around the Solar System!

## The CS Concepts

- **Coordinate Spaces (X, Y, Z):** Transitioning from 2D pixel space `(x,y)` to a fully continuous 3D world space `(x,y,z)`.
- **Spatial Vectors:** Storing positions as `Vector3` structures and pushing them through the rendering pipeline.
- **Trigonometry over Time:** The entire simulation is driven by multiplying elapsed time (`GetFrameTime()`) into an `angle` variable, and then using `std::sin()` and `std::cos()` to continuously calculate circular orbital paths. 
