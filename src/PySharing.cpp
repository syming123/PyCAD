#include "PySharing.h"

// �ڴ˴�������Ҫʹ�õ�ȫ�ֺ���
namespace PySharing
{
    std::function<void(std::string)> printOut;
    std::function<void(std::string)> printErr;
    std::function<void(TopoDS_Shape)> showOCCShape;
    std::function<void(TopoDS_Shape)> changeShow;
}