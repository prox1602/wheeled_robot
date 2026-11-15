## 🛠️ Hardware Architecture & Power Management

The robot's hardware architecture is designed around cost-effective rapid-prototyping modules, integrated into a robust differential drive platform.

**Core Computing & Sensors:**
* **Microcontroller:** ESP32-S3 (Dual-core, managing real-time tasks and wireless communication).
* **IMU:** Bosch Sensortec BMI270 (I2C). A modern, ultra-low power 6-axis inertial sensor chosen over legacy modules for superior noise performance and high-precision spatial orientation.
* **Odometry:** Dual Slotted Optical Interrupters (Photoelectric speed sensors) reading wheel-mounted encoder disks. Signals are processed via ESP32 hardware interrupts.

**Actuation & Drivetrain:**
* **Motors:** 5V DC Micro Gearmotors with a 48:1 reduction ratio, configured in a differential drive setup.
* **Motor Driver:** *(Tutaj wpisz swój sterownik, np. L298N lub TB6612FNG)* driven by high-frequency PWM signals.

**Power Delivery & Integration:**
* **Battery Pack:** Custom 2S Li-Ion power source (2x 18650 cells in series, yielding 7.4V - 8.4V).
* **Battery Management System (BMS):** Integrated 2S 10A BMS module to ensure safe charging, over-discharge protection, and cell balancing.
* **Mainboard:** All modules and power distribution lines are manually routed and soldered on a 50x70mm double-sided universal PCB.

## 🚀 Future Roadmap (To-Do)
The hardware is prepared for future environmental mapping capabilities, which are currently under development:
* [ ] **Active Obstacle Avoidance:** Integration of the HC-SR04 Ultrasonic Distance Sensor.
* [ ] **Environment Scanning:** Mounting the HC-SR04 on an SG-90 180° Micro Servo to create a sweeping 2D LIDAR-like depth map.