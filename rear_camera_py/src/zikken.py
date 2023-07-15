import time

from picamera2.request import Helpers
from picamera2 import Picamera2, Preview
import time
from picamera2.encoders import H264Encoder

def test1():
    """画像
    https://www.kosh.dev/article/14/
    """
    camera = Picamera2()
    capture_config = camera.create_still_configuration()
    camera.configure(capture_config)
    camera.start()
    time.sleep(1)

    h = Helpers(camera)

    with open("buffers.pkl", "rb") as tf:
        (b, ), meta = pickle.load(tf)

    img = h.make_image(b, capture_config['main'])
    h.save(img, meta, 'sample.png')

def test2():
    """画像
    https://raspida.com/cameramodule-v3-1st-impression
    """
    picam2 = Picamera2()
    camera_config = picam2.create_preview_configuration()
    picam2.configure(camera_config)
    picam2.start_preview(Preview.QTGL)
    picam2.start()
    time.sleep(2)
    picam2.capture_file("test.jpg")

def test3():
    """動画
    https://dev.classmethod.jp/articles/raspberrypi-camera-freenove/
    """
    picam2 = Picamera2()
    video_config = picam2.create_video_configuration()
    picam2.configure(video_config)
    encoder = H264Encoder(bitrate=10000000)
    output = "test.h264"
    picam2.start_recording(encoder, output)
    time.sleep(10)
    picam2.stop_recording()

if __name__ == "__main__":
    test1()
    test2()
    test3()