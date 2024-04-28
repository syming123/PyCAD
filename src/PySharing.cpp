#include "PySharing.h"

// 在此处定义需要使用的全局函数
namespace PySharing
{
    std::function<void(std::string)> printOut;
    std::function<void(std::string)> printErr;
    std::function<void(TopoDS_Shape)> showOCCShape;
    std::function<void(TopoDS_Shape)> changeShow;
}