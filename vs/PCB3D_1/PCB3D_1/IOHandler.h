/*
* Python����ض���
*/

#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <iostream>
#include <thread>
#include <QWidget>
#include <QDebug>
#include <QTextCursor>
#include <vector>
#include "PySharing.h"

namespace py = pybind11;

namespace IOHandlerSpace
{
    using namespace py::literals;
    class IOHandler
    {
    private:
        py::module_ _sys;
        py::module_ _pyconsole;
        py::object _stdout;


    public:
        IOHandler()
        {
            // ������Ҫ��python��
            _sys = py::module_::import("sys");
            _pyconsole = py::module_::import("pyconsole");

            // ����ض���pyconsole.out
            _stdout = _sys.attr("stdout");
            _sys.attr("stdout") = _pyconsole.attr("out");
        }

        ~IOHandler()
        {
            _sys.attr("stdout") = _stdout;
        }

        void exec(std::string input)
        {
            try {
                py::exec(input);
            }
            catch (py::error_already_set &e) {
                std::string etype = e.type().str();
                std::string evalue = e.value().str();
                PySharing::printErr(etype + "\n" + evalue + "\n");
                std::cout << e.what() << std::endl;
            }
        }
    };


}

