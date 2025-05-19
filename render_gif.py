import math
import subprocess

from PIL import Image

X_SIZE = 128
Y_SIZE = 64
N = 20
ZOOM = 0.7
N_FRAMES = 50
FRAME_TIMEOUT = 1000 / 30.0


def render_frame(cnt):
    img = Image.new('1', (X_SIZE, Y_SIZE))
    pixels = img.load()

    phi = 2.0 * math.pi * cnt / N_FRAMES
    for i in range(N + 1):
        for j in range(N + 1):
            x = (i - N / 2.0) * 2 / N
            y = (j - N / 2.0) * 2 / N
            d = math.sqrt(x ** 2 + y ** 2)
            z = (math.sqrt(2) - d) * math.sin(phi - d * 5) * ZOOM

            rotation = phi / 4
            xp = X_SIZE * (1 + (x * math.sin(rotation) + y * math.cos(rotation)) * ZOOM) / 2
            yp = Y_SIZE * (1 + (z + (x * math.cos(rotation) - y * math.sin(rotation)) / 2) * ZOOM) / 2

            pixels[int(xp), int(yp)] = 1

    return img


def render_anim():
    return [render_frame(i) for i in range(N_FRAMES)]


def save(images):
    images[0].save(
        'anim.gif',
        save_all=True,
        append_images=images[1:],
        duration=FRAME_TIMEOUT,
        loop=0,
    )


def show():
    subprocess.run(['open', '-a', 'Safari', 'anim.gif'])


if __name__ == "__main__":
    images = render_anim()
    save(images)
    show()

