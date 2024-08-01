import asyncio
import websockets
import random
from datetime import datetime

def generate_random_data():
    environments = ["Eclipse", "Normal", "Sun Exposure"]
    errors = ["", "Sensor Error", "Heater Failure"]
    
    temperature_data = [
        random.randint(-10, 10),  # Random temperature for THERM-01
        random.randint(-10, 10),  # Random temperature for THERM-02
        random.randint(-10, 10),  # Random temperature for THERM-03
        random.randint(-10, 10),  # Random temperature for THERM-04
        random.choice(["On", "Off"]),  # Random heater status for HTR-01
        random.choice(["On", "Off"]),  # Random heater status for HTR-02
        random.choice(["On", "Off"]),  # Random heater status for HTR-03
        random.choice(["On", "Off"]),  # Random heater status for HTR-04
        datetime.now().isoformat(),  # Current timestamp
        random.choice(environments),  # Random environment
        random.choice(errors)  # Random error status
    ]
    
    return str(temperature_data)

async def handle_client(websocket, path):
    print(f"Connection from: {websocket.remote_address}")
    try:
        while True:
            data = generate_random_data()
            await websocket.send(data)
            await asyncio.sleep(0.2)
    except websockets.exceptions.ConnectionClosed:
        print(f"Connection with {websocket.remote_address} closed.")

async def server_program():
    host = "localhost"
    port = 8084

    server = await websockets.serve(handle_client, host, port)
    print(f"Server is running on: {host}:{port}")

    await server.wait_closed()

if __name__ == "__main__":
    asyncio.run(server_program())
