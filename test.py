def print_file_as_bits(filename):
    with open(filename, 'rb') as f:
        byte = f.read(1)
        while byte:
            # Convert byte to 8-bit binary string
            print(f"{ord(byte):08b}", end='')
            byte = f.read(1)

# Example usage
print_file_as_bits('example.huffman')

