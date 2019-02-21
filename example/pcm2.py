from g726 import g726, AUDIO_ENCODING_LINEAR, G726_16
import sys

print(sys.argv)

input_f = open(sys.argv[1], "rb")
output_f = open(sys.argv[1].replace(".snd", ".g276"), "wb")

g726_ins = g726(AUDIO_ENCODING_LINEAR, G726_16)

raw = input_f.read()
raw_len = len(raw)
group_len = int(80 / 4 * 16 * 2)

add_len = int(raw_len % group_len)
if add_len > 0:
    print(add_len)
    raw = raw + bytearray(group_len - add_len)

output_f.write(g726_ins.pcm_to_g726(raw))

input_f.close()
output_f.close()
