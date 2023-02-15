import os
from typing import List, Callable 
import PyFileClassificator as FC

def Classificate(src_dir: str, dilpicate_store_dir: str, progresser: Callable = None) -> List:
    if not os.path.isdir(src_dir):
        raise TypeError('Source directory not exists or it\'s not a directory')
    return FC.Classificate(src_dir, dilpicate_store_dir, progresser)

def GetSHA512StringValue(src_file: str) -> str:
    if not os.path.exists(src_file):
        raise TypeError('Source file path is not exists')
    
    if os.path.isdir(src_file):
        raise TypeError('Source file cannot be a directory')
