import sys

# Available at setup time due to pyproject.toml
from pybind11 import get_cmake_dir
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

__version__ = "0.0.1"

# The main interface is through Pybind11Extension.
# * You can add cxx_std=11/14/17, and then build_ext can be removed.
# * You can set include_pybind11=false to add the include directory yourself,
#   say from a submodule.
#
# Note:
#   Sort input source files if you glob sources to ensure bit-for-bit
#   reproducible builds (https://github.com/pybind/python_example/pull/53)

ext_modules = [
    Pybind11Extension("ArcanePythonModule",
        ["./src/Arcane/Scripting/PythonBindings.cpp"]),
]

setup(
    name="ArcanePythonModule",
    version=__version__,
    author="Thomas Heeley",
    description="Python module for game engine",
    long_description="",
    ext_modules=ext_modules,
)