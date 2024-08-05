#! /usr/bin/python
import asyncio
import websockets
import csv
from datetime import datetime

def get_latest_csv_line(file_path):
    with open(file_path, 'r') as f:
        last_line = None
        reader = csv.reader(f)
        for last_line in reader:
            pass
        
        return_list = []
        
        for item in last_line:
            return_list.append(item.strip())
    return return_list

def validate_data(data):
    try:
        # Check the number of fields
        if len(data) != 11:
            return False

        # Check heater statuses
        for i in range(4, 8):
            if data[i] not in ["On", "Off"]:
                return False

        # Check timestamp
        datetime.fromisoformat(data[8])

        # Check environment
        if data[9] not in ["Eclipse", "Normal", "Sun Exposure"]:
            return False

        # Check error status
        if data[10] not in ["null", "Sensor Error", "Heater Failure"]:
            return False

        return True
    except (ValueError, IndexError):
        return False

async def handle_client(websocket, path, file_path):
    print(f"Connection from: {websocket.remote_address}")
    refresh_rate = 0.2
    
    async def receive_refresh_rate():
        nonlocal refresh_rate
        while True:
            try:
                message = await websocket.recv()
                try:
                    new_refresh_rate = float(message)
                    if 0.2 <= new_refresh_rate <= 1:
                        refresh_rate = new_refresh_rate
                        print(f"Updated refresh rate to: {refresh_rate} seconds")
                    else:
                        print("Received refresh rate out of range. Ignoring.")
                except ValueError:
                    print("Received invalid refresh rate. Ignoring.")
            except websockets.exceptions.ConnectionClosed:
                print(f"Connection with {websocket.remote_address} closed.")
                break

    asyncio.create_task(receive_refresh_rate())

    try:
        while True:
            data = get_latest_csv_line(file_path)
            if data:
                print(data)
                if validate_data(data):
                    await websocket.send(str(data))
                else:
                    print("Invalid data format, skipping sending data.")
            await asyncio.sleep(refresh_rate)
    except websockets.exceptions.ConnectionClosed:
        print(f"Connection with {websocket.remote_address} closed.")

async def server_program(file_path):
    host = "localhost"
    port = 8084

    server = await websockets.serve(lambda ws, path: handle_client(ws, path, file_path), host, port)
    print(f"Server is running on: {host}:{port}")

    await server.wait_closed()

if __name__ == "__main__":
    file_path = '../../TSL/data.csv'
    asyncio.run(server_program(file_path))
