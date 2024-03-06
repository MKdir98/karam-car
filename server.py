import asyncio
import websockets
import pyaudio
import numpy as np

CHUNK = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 44100

p = pyaudio.PyAudio()
stream = p.open(format=FORMAT,
                channels=CHANNELS,
                rate=RATE,
                output=True,
                frames_per_buffer=CHUNK)


async def echo(websocket, path):
    while True:
        data = await websocket.recv()
        play_audio(data)

        print(f"Received: {data}")


def play_audio(data):
    audio_data = np.frombuffer(data, dtype=np.int16)
    stream.write(audio_data)


start_server = websockets.serve(echo, "0.0.0.0", 8666)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()
