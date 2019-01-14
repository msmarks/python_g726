# python_g726
a python3 library convert bwtween pcm with g726

## build
clone this repository, then
```
python setup.py build_ext --inplace
```

## example
convert 8000Hz Signed 16bits LSB PCM mono to g726 at rates of 16kbit/s 
```
from g726 import g726, AUDIO_ENCODING_LINEAR, G726_16

input_f = open("example/test.snd", "rb")
output_f = open("example/test_2.g726", "wb")

g726_ins = g726(AUDIO_ENCODING_LINEAR, G726_16)

output_f.write(g726_ins.pcm_to_g726(input_f.read()))

input_f.close()
output_f.close()
```

## usage
### 1. import library
```
import g726
```

### 2. create instance of g726
```
g726_ins = g726.g726(pcm_audio_type, g726_audio_type)
```
pcm_audio_type can be one of following

| pcm_audio_type              | description                    |
| --------------------------- | ------------------------------ |
| g726.AUDIO_ENCODING_LINEAR  | PCM 2's-complement (0-center)  |
| g726.AUDIO_ENCODING_ULAW    | ISDN u-law                     |
| g726.AUDIO_ENCODING_ALAW    | ISDN A-law                     |

g726_audio_type can be one of following

| pcm_audio_type  | description               |
| --------------- | --------------------------|
| g726.G726_16    | g726 at rates of 16kbit/s |
| g726.G726_24    | g726 at rates of 24kbit/s |
| g726.G726_32    | g726 at rates of 32kbit/s |
| g726.G726_40    | g726 at rates of 40kbit/s |

### 3. convert
g726 -> pcm
```
output = g726_ins.g726_to_pcm(input)
```
input is g726 bytearray audio data, length should be a multiple of 4, example:
g726_16: 2bits present a sample, so 4 bytes of g726_16 can present 16 samples of pcm.

| byte | bits order                      |               
| ---- | ------------------------------- |
| 0    | b00 b00 b01 b01 b02 b02 b03 b03 |
| 1    | b04 b04 b05 b05 b06 b06 b07 b07 |
| 2    | b08 b08 b09 b09 b10 b10 b11 b11 |
| 3    | b12 b12 b13 b13 b14 b14 b15 b15 |

g726_24: 3bits present a sample, so 4 bytes of g726_24 can present 10 samples of pcm.

| byte | bits order              |               
| ---- | ----------------------- |
| 0    | 00 00 b0 b0 b0 b1 b1 b1 |
| 1    | b2 b2 b2 b3 b3 b3 b4 b4 |
| 2    | b4 b5 b5 b5 b6 b6 b6 b7 |
| 3    | b7 b7 b8 b8 b8 b9 b9 b9 |

g726_32: 4bits present a sample, so 4 bytes of g726_32 can present 8 samples of pcm.

| byte | bits order              |               
| ---- | ----------------------- |
| 0    | b0 b0 b0 b0 b1 b1 b1 b1 |
| 1    | b2 b2 b2 b2 b3 b3 b3 b3 |
| 2    | b4 b4 b4 b4 b5 b5 b5 b5 |
| 3    | b6 b6 b6 b6 b7 b7 b7 b7 |

g726_40: 5bits present a sample, so 4 bytes of g726_40 can present 6 samples of pcm.

| byte | bits order              |               
| ---- | ----------------------- |
| 0    | 00 00 b0 b0 b0 b0 b0 b1 |
| 1    | b1 b1 b1 b1 b2 b2 b2 b2 |
| 2    | b2 b3 b3 b3 b3 b3 b4 b4 |
| 3    | b4 b4 b4 b5 b5 b5 b5 b5 |

output is bytearray may be ISDN u-law, ISDN A-law or AUDIO_ENCODING_LINEAR, every 16bits present a sample.

pcm -> g726
```
output = g726_ins.pcm_to_g726(input)
```

these apis may called one or more times. But for every new audio file, should get new instance of g726 class to reset some inner state.

## Thanks
Very thanks to this project https://github.com/quatanium/foscam-ios-sdk. I use the c version of g726 in this project.

## Other
This project is part of my DIY to make XiaoYi camera to speak and integrated into hass. It uses g726 16kbits to transform audio from App to Camera. G726 16kbits is comfirmed to work in my project, other rates I have not tested. If you find problem, please open is issues or pull request.
