from glob import glob
from distutils.core import setup, Extension
from pybind11.setup_helpers import Pybind11Extension

def main():
    setup(name="ArcanePythonModule",
          version="1.0.0",
          description="Python interface for the myadd C library function",
          author="Thomas",
          author_email="tom@tom.org",
          ext_modules=[Pybind11Extension("ArcanePythonModule",["C:/Projects/Arcane-Engine/Arcane/src/Arcane/Scripting/PythonBindings.cpp"])],
          )


if __name__ == "__main__":
    main()