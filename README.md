# Spacecraft Thermal Control System (STCS)

## Overview

The Spacecraft Thermal Control System (STCS) project aims to develop a comprehensive thermal control system for spacecraft. It includes a Temperature Simulation Library (TSL) for simulating thermal conditions, a Thermal Control Function (TCF) for maintaining desired temperatures using PID control, and a Visualization User Interface (VUI) for monitoring and analyzing temperature data.

## Table of Contents

- [Introduction](#introduction)
- [Installation](#installation)
- [Components](#components)
  - [Temperature Simulation Library (TSL)](#temperature-simulation-library-tsl)
  - [Thermal Control Function (TCF)](#thermal-control-function-tcf)
  - [Visualization User Interface (VUI)](#visualization-user-interface-vui)

## Introduction

The STCS project is designed to simulate, control, and visualize the thermal environment of a spacecraft. The system is divided into three main components:

- **Temperature Simulation Library (TSL):** Simulates temperature variations and heater operations.
- **Thermal Control Function (TCF):** Implements PID control to maintain specified temperatures.
- **Visualization User Interface (VUI):** Displays temperature data and heater status, providing real-time monitoring and analysis.


## Installation

To set up the STCS project you just need to clone the repository:
   ```sh
   git clone https://bitbucket.critical.pt/scm/~tg-silva/spacecraft-thermal-control-system.git STCS
   ```

## Components
### Temperature Simulation Library (TSL)
The TSL simulates the thermal environment of a spacecraft, including normal periods, eclipses, and sun exposure. It generates temperature data for thermistors and controls heater status based on simulation parameters.

### Thermal Control Function (TCF)
The TCF uses PID control to maintain desired temperatures for each thermistor. It reads temperature data from the TSL and adjusts heater statuses accordingly to achieve the setpoints.
ola
### Visualization User Interface (VUI)
The VUI provides real-time visualization of temperature data and heater statuses. It plots temperature variations and heater operations, allowing users to monitor and analyze the thermal control system's performance.
