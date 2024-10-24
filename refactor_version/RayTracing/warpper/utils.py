import os
import subprocess
import importlib.util
from pathlib import Path

def is_file(pth: str) -> bool:
    
    if pth.endswith('/'):
        return False

    _, ext = os.path.splitext(pth)
    if ext:
        return True
    else:
        return False

def is_dir(pth: str) -> bool:
    
    if pth.endswith('/'):
        return True
    
    _, ext = os.path.splitext(pth)
    if ext:
        return False
    else:
        return True

def mkdir(*directories) -> None:
    for directory in directories:
        if not os.path.exists(directory) and os.path.isdir(directory):
            os.makedirs(directory)

def load_cpp_module(so_filename: str):
    """Helper function to dynamically load a .so file."""
    cpp_module_path = Path(__file__).parent.parent / 'cpp_impl' / 'build' / so_filename

    # Check if the .so file exists
    if not cpp_module_path.exists():
        print(f"Warning: {cpp_module_path} does not exist.")
        print("Attempting to build the C++ implementation by running 'make'...")

        # Run the Makefile to build the .so file
        makefile_dir = Path(__file__).parent.parent / 'cpp_impl'
        result = subprocess.run(['make'], cwd=makefile_dir, capture_output=True, text=True)

        if result.returncode != 0:
            raise RuntimeError(f"Failed to build the C++ implementation:\n{result.stderr}")
        else:
            print("Build successful, loading the .so file...")

    # Dynamically import the C++ module
    spec = importlib.util.spec_from_file_location(f"cpp_impl.build.{so_filename.split('.')[0]}", cpp_module_path)
    cpp_impl = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(cpp_impl)
    return cpp_impl