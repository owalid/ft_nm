import os
from colorama import Fore
import subprocess

def compile_all():
    os.system("rm -rf build")
    os.system("mkdir -p build")

    for i in range(1, 4):
        os.system(f"gcc test{i}.c -o build/test{i}_elf64 -m64") # Compile as elf64
        # os.system(f"gcc test{i}.c -o build/test{i}_elf32 -m32") # Compile as elf32

def process_command(number, platform):
    try:
        result_nm = os.system(f'nm build/test{number}_{platform} > .tmp_nm')
        result_mine = os.system(f'../ft_nm build/test{number}_{platform} > .tmp_mine')
        diff = subprocess.run(f'diff .tmp_nm .tmp_mine', shell=True)
    except Exception as e:
        print(Fore.RED + f"Error: {e}", end='')
        print(Fore.RED, end='')
        print('============= FAIL ===============')
        print(f'binary number {number}, platform {platform}')
        # print(result_mine)
        # print(result_nm)
        print(e)
        print('============= FAIL ===============')
        return
    
    if diff == '':
        print(Fore.GREEN, end='')
        print(f'binaryNumber {number} platform {platform}: OK\'')
    else:
        print(Fore.RED, end='')
        print('============= FAIL ===============')
        print(f'binary number {number}, platform {platform}')
        # print(result_mine)
        # print(result_nm)
        print(diff)
        print('============= FAIL ===============')

if __name__ == "__main__":
    compile_all()

    # for platform in ["elf64", "elf32"]:
    for platform in ["elf64"]:
        for i in range(1, 4):
            process_command(i, platform)

