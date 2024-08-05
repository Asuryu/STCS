# VUI

## Project Overview
This project consists of a WebSocket-based real-time data visualization application. The backend, written in Python, serves real-time temperature data through a WebSocket server. The frontend, developed with React and Recharts, connects to this server, receives data, and displays it in a series of line charts. Additionally, the application includes a dynamic error log display, adjustable refresh rate, and data range for better analysis.

## Prerequisites
- Python 3.x
- Node.js and npm (Node Package Manager)

## Setup and Running the Backend (Python WebSocket Server)

1. **Install Required Python Packages**:
   Before running the server, ensure you have the necessary Python packages. You can use `pip` to install any required packages. The server depends on `websockets` and `csv` for handling WebSocket connections and reading CSV files, respectively.

   ```bash
   pip install -r requirements.txt
   ```

2. **Prepare the CSV Data File**:
   Ensure there is a CSV file available at `./data.csv` that the server will read from. The file should contain rows of data with the following fields:
   - Four temperature readings (THERM_01, THERM_02, THERM_03, THERM_04)
   - Four heater statuses (HTR_01, HTR_02, HTR_03, HTR_04)
   - A timestamp
   - An environment status (Eclipse, Normal, Sun Exposure)
   - An error status (optional, could be "Sensor Error", "Heater Failure", or empty)

3. **Run the WebSocket Server**:
   Execute the Python script to start the server. The server listens for incoming WebSocket connections and streams the latest valid data from the CSV file.

   ```bash
   python3 server.py
   ```

## Setup and Running the Frontend (React Application)

1. **Install Node.js and npm**:
   Ensure that Node.js and npm are installed on your system. You can download them from [Node.js official website](https://nodejs.org/).

2. **Install Frontend Dependencies**:
   Navigate to the frontend project directory and install the required dependencies using npm.

   ```bash
   cd frontend_directory
   npm install
   ```

   Replace `frontend_directory` with the actual path to your frontend project.

3. **Run the React Application**:
   Start the React application, making sure the server is running.

   ```bash
   npm run dev
   ```

   This command will run the application on [http://localhost:8083](http://localhost:8083) by default.

## Notes
- Ensure that the WebSocket server is running and accessible at `ws://localhost:8084` when starting the React application.
- The frontend relies on a WebSocket connection to receive data. If the server is not running or the WebSocket connection fails, the application will display an error.

