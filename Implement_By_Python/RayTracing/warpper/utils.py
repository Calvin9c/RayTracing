import os

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