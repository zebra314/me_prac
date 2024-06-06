# MYBOT

An arduino robot built for NYCU ME 2024 mechanical practice course. It is a car-like robot that consists of a four wheel drive system and a 2 DOF robotic arm. The robot is running on FreeRTOS and controlled by smartphone via bluetooth.

![run](./data/run.gif)

## Hardware

- 1x Arduino Mega 2560
- 1x HC-05 Bluetooth Module
- 4x CHR-GM37 Motor with Encoder
- 2x RDS3235 35kg Servo
- 2x L298N Motor Driver
- 6x 18650 Battery
- 2x 18650 Battery Holder
- 2x Laser cutting aluminum plate

## Schematic

My teammate <a href="https://github.com/lulumy1015" target="_blank">@lulumy1015</a>
is responsible for the schematic design. The schematic is shown below.
![Schematic](./data/schematic.JPG)

## Control Method

### Bluetooth communicate

My teammate <a href="https://github.com/dragonwu0919" target="_blank">@dragonwu0919</a> is responsible for the bluetooth communication and the remote controller UI design.

The remote control program includes vehicle movement, arm control, and speed control. Steering involves clockwise/counterclockwise rotation for precise control. Arm functions include resetting, positioning, taking, and dropping ping pong balls. Speed settings help with alignment. Signals are periodically transmitted for movement and initiated by button press for arm control. This periodic transmission is managed by a timer, triggering Bluetooth transmission and countdown cycles.

![](./data/remote_controller_gui.png)

### Setup

```mermaid
flowchart TD;
    A[Initial Serial port] --> B[Connect bluetooth];
    B --> C;
    C --> D;
    D --> E[Connect bluetooth];

subgraph C[Connect plate]
    F[Connect FR wheel ]--> G[Connect FL wheel];
    G--> H[Connect BR wheel];
    H--> I[Connect BL wheel];
end

subgraph D[Connect arm]
    J[Connect base servo]--> K[Create freeRTOS task for base servo];
    K--> L[Connect upper servo];
    L--> M[Create freeRTOS task for upper servo];
end
```

### Main Control Loop

```mermaid
flowchart TD;
    B[Check timeout];
    C[Read bluetooth command];

    B --> C;
    C --> B;

subgraph B[Check timeout]
    E[Get time value and flag from plate] --> ifPlate[If the plate idle reach timeout?];
    ifPlate-->|Yes| F[Stop the plate];
    ifPlate -->|No| G[Do nothing];

    H[Get time value and flag from arm] --> ifArm[If the arm idle reach timeout?];
    ifArm -->|Yes| I[Freeze the arm];
    ifArm -->|No| J[Do nothing];
end

subgraph C[Process command]
    K[Read bluetooth command];
    K --> L[Parse command];
    L --> M[Execute command];
    M --> N[Reset timeout];
end
```

### Arm Control

```mermaid
flowchart TD;

ifReach[Check if reach the target];
ifLimit[Check if reach the limit];
ifAcce[Check if the target is close to the current position];
A[Determine the direction];
B[Actuate the servo];
C[Set the taget equals to the limit];
D[Deaccelerate the servo];

ifReach --> |Yes| ifReach;
ifReach --> |No| ifLimit;

ifLimit --> |Yes| C;
C --> ifAcce;
ifLimit --> |No| ifAcce;

ifAcce --> |Yes| D;
D --> A;
ifAcce --> |No| A;

A --> B;
B --> ifReach;
```

### COMP_PID

To control the wheel, we use a COMP_PID controller. The COMP_PID controller is a PID controller that can adjust the weight of the position and velocity error. The controller is shown below.

The plate is able to move by a designated velocity and position.
I write a script the make the plate to move to a target point in the map. However, it is not used in the final version, because we found out that our pilot is just as good as the script.  

```mermaid
graph TD;
    A[Input: target velocity and position] --> B[Compute current time];
    B --> C[Compute time difference];
    C --> D[Compute position error];
    C --> E[Compute velocity error];
    D --> F[Compute position error derivative];
    E --> G[Compute velocity error derivative];
    F --> H[Integrate position error];
    G --> I[Integrate velocity error];
    H --> J[Compute maximum position error];
    J --> K[Compute position output];
    I --> L[Compute velocity output];
    K --> M[Compute position weight];
    M --> N[Compute velocity weight];
    L --> N;
    N --> O[Compute combined output];
    O --> P[Update previous time and error values];
```

$$
\begin{align*}
\text{pos weight base} &= 100 \\
\text{pos error percent} &= \frac{|\text{pos error}|}{\text{pos error max}} \quad \text{(Value domain: } [0, 1]) \\
\text{vel weight} &= \frac{{\text{pos weight base}}^{\text{pos error percent}} - 1}{\text{pos weight base} - 1} \quad \text{(Value domain: } [0, 1]) \\
\text{pos weight} &= 1 - \text{vel weight}
\end{align*}
$$

Some experiment data is shown below.
![Experiment Data](./data/pid_comp.png)

## Usage

It is recommended to use the docker image to compile the code, you can also use the platformio extension in vscode.

For docker user, you can use the following commands:

To build the docker image

```sh
make install
```

To build the code and upload to the arduino

```sh
make
```

To compile the code only

```sh
make build
```

To upload the code to the arduino only

```sh
make upload
```

To attach the docker container

```sh
make attach
```

## Record Photos

Some old version arms and mechanical parts.

![arm_old](./data/arm_old.jpg)

![arm_test_1](./data/arm_test_1.jpg)

![arm_test_2](./data/arm_test_2.jpg)

![motor_wheel](./data/motor_to_wheel.jpg)

Making the robot.

![making_1](./data/record_1.jpg)

Before the final competition.

![take](./data/test_take_ball.jpg)

![fix](./data/fixing_the_mech.jpg)

![before](./data/before_contest.JPG)

![me](./data/me.JPG)

We.

![we](./data/group.JPG)
