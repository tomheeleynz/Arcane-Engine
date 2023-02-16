#include <pybind11/pybind11.h>

#include "Arcane/Core/InputManager.h"

PYBIND11_MODULE(ArcanePythonModule, m) {
	m.doc() = "Arcane python module for game scripting";
}