import wave
import struct
import math

def generate_tone(filename, frequency, duration_sec, volume=0.5, sample_rate=44100):
    num_samples = int(duration_sec * sample_rate)
    with wave.open(filename, 'w') as w:
        w.setnchannels(1) # mono
        w.setsampwidth(2) # 2 bytes per sample (16-bit)
        w.setframerate(sample_rate)
        
        for i in range(num_samples):
            # Sine wave
            value = math.sin(2.0 * math.pi * frequency * (i / float(sample_rate)))
            # Envelope (fade out slightly)
            envelope = 1.0 - (i / float(num_samples))
            # Scale to 16-bit int
            sample = int(value * envelope * volume * 32767.0)
            data = struct.pack('<h', sample)
            w.writeframesraw(data)

# High beep for paddle
generate_tone('paddle_hit.wav', 880.0, 0.1)
# Low boop for wall/score
generate_tone('wall_hit.wav', 220.0, 0.3)
