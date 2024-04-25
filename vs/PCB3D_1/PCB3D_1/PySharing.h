/*
* ����ȫ�ֱ�����������
*/

#pragma once
#include <iostream>
#include <functional>
#include <TopoDS.hxx>

// �ڴ˴�������Ҫʹ�õ�ȫ�ֺ���
namespace PySharing
{
    extern std::function<void(std::string)> printOut;
    extern std::function<void(std::string)> printErr;
    extern std::function<void(TopoDS_Shape)> showOCCShape;
    extern std::function<void(TopoDS_Shape)> changeShow;
}

