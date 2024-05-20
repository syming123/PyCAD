/*
* Python输出重定向
*/

#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <iostream>
#include <functional>
#include <thread>
#include <QWidget>
#include <QDebug>
#include <QTextCursor>
#include <vector>

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
        std::function<void(std::string)> printOut;
        std::function<void(std::string)> printErr;

        IOHandler()
        {
            // 导入需要的python库
            _sys = py::module_::import("sys");
            _pyconsole = py::module_::import("pyconsole");

            // 输出重定向到pyconsole.out
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
                printErr(etype + "\n" + evalue + "\n");
                std::cout << e.what() << std::endl;
            }
        }
    };


}

#endif