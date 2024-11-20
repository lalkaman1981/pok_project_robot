
import binascii
import struct
def get_string(filepath):
    with open(filepath, 'rb') as f:
        hexdata = str(binascii.hexlify(f.read()))

    length = len(hexdata)
    output_string = ""
    for i in range(34, length - 1, 288 + 208):

        row = hexdata[i:i + 208]

        output_string += "#".join([f"{num + 1}P{struct.unpack('<H', bytes.fromhex(row[i:i + 4]))[0]}" for num, i in enumerate(range(16, 208, 12))])
        output_string += f"#T{struct.unpack('<H', bytes.fromhex(row[:4]))[0]}\n"

    return output_string

print(get_string(r"C:\Users\bohda\OneDrive\Документи\project\H5S\H5S\4.Appendix-20241104T205511Z-001\4.Appendix\4.1Action Group File(High Speed Gait）\No.7 Push Up.rob"))