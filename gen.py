import sys
import random

def generate_random_order(n, file_path):
    numbers = list(range(1, n + 1))
    random.shuffle(numbers)
    
    with open(file_path, 'w') as file:
        for number in numbers:
            file.write(f"{number}\n")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <n> <output_file>")
        sys.exit(1)
    
    n = int(sys.argv[1])
    output_file = sys.argv[2]
    generate_random_order(n, output_file)