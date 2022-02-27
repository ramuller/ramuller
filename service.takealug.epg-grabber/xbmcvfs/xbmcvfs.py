import os
import shutil

def delete(path):
    print('Delete file : {}'.format(path))
    try:
        os.remove(path)
    except IsADirectoryError:
        shutil.rmtree(path)
        

def copy(src, dest):
    return shutil.copyfile(src, dest)
