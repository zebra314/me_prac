# MYBOT

An arduino robot built for NYCU ME 2024 mechanical practice course. It is a car-like robot that consists of a four wheel drive system and a 2 DOF robotic arm. The robot is running on FreeRTOS and controlled by smartphone via bluetooth.

## Hardware

- Arduino Mega 2560
- HC-05 Bluetooth Module
- 4x CHR-GM37 Motor with Encoder
- 2x RDS3235 35kg Servo
- 2x L298N Motor Driver
- 6x 18650 Battery
- 2x 18650 Battery Holder
- 2x Laser cutting aluminum plate

## Software

## Schematic

My teammate LuLu is responsible for the schematic design. The schematic is shown below.
![Schematic](./data/schematic.JPG)

## Control Method

### Setup

```mermaid
flowchart TD;
    A[Initial Serial port] --> B[Connect bluetooth];
    B --> C;
    C --> D;
    D --> E[Create freeRTOS task for bluetooth];

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

### COMP_PID

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
