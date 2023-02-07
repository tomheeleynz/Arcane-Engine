#include <pybind11/pybind11.h>

int add(int i, int j) {
	return i + j;
}

int subtract(int i, int j) {
	return i + j;
}

PYBIND11_MODULE(ArcanePythonModule, m) {
	m.doc() = "Example python module";
	m.def("add", &add, "A function that adds two numbers");
	m.def("subtract", &subtract, "A function that subtracts two numbers");
}