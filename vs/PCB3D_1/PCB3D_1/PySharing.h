/*
* 声明全局变量（函数）
*/

#pragma once
#include <iostream>
#include <functional>
#include <TopoDS.hxx>

// 在此处声明需要使用的全局函数
namespace PySharing
{
    extern std::function<void(std::string)> printOut;
    extern std::function<void(std::string)> printErr;
    extern std::function<void(TopoDS_Shape)> showOCCShape;
    extern std::function<void(TopoDS_Shape)> changeShow;
}

