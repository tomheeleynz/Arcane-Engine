#include <pybind11/pybind11.h>

int add(int i, int j) {
	return i + j;
}

int subtract(int i, int j) {
	return i - j;
}

PYBIND11_MODULE(ArcanePythonModule, m) {
	m.doc() = "Arcane Python Module for Game Engine";
	m.def("add", &add, "Add two integers together");
	m.def("subtract", &subtract, "Subtract two integers together");
}