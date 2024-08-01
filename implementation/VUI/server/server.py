import asyncio
import websockets

async def handle_client(websocket, path):
    print(f"Connection from: {websocket.remote_address}")
    try:
        while True:
            data = "ola mundo"
            await websocket.send(data)  # send data to the client
            await asyncio.sleep(2)
    except websockets.exceptions.ConnectionClosed:
        print(f"Connection with {websocket.remote_address} closed.")

async def server_program():
    host = "localhost"
    port = 8084  # initiate port no above 1024

    server = await websockets.serve(handle_client, host, port)
    print(f"Server is running on: {host}:{port}")

    await server.wait_closed()

if __name__ == '__main__':
    asyncio.run(server_program())