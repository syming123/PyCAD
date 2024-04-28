/*
* 声明全局变量（函数）
*/

#ifndef PYSHARING_H
#define PYSHARING_H
#include <iostream>
#include <functional>
#include <TopoDS.hxx>

namespace PySharing
{
    extern std::function<void(std::string)> printOut;
    extern std::function<void(std::string)> printErr;
    extern std::function<void(TopoDS_Shape)> showOCCShape;
    extern std::function<void(TopoDS_Shape)> changeShow;
}

#endif