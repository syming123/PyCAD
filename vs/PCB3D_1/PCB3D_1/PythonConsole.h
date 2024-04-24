#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include "IOHandler.h"
#include "PySharing.h"
#include <iostream>
#include <thread>
#include <QWidget>
#include <QDebug>
#include <QTextCursor>
#include "ui_PythonConsole.h"


namespace py = pybind11;

class PythonConsole : public QWidget
{
	Q_OBJECT


public:
	PythonConsole(QWidget *parent = Q_NULLPTR);
	~PythonConsole();
	bool eventFilter(QObject* watched, QEvent* event);
	void exec();
	void printOut(std::string str);
	void printErr(std::string str);
    void putPrompt();
	void putMessage(std::string msg);


private:
	Ui::PythonConsole ui;
	int markPosition;  // 上一次读取后的光标位置
	bool editable;  // 文本是否可编辑，根据光标位置判断
    std::vector<int> replaces;  // 多行输入时提示符'.'(dot)的位置，在执行前删掉
    QTextCharFormat receiveFormat, writeFormat, errorFormat;  // 接收,写入,错误时的文字样式
    IOHandlerSpace::IOHandler* iohandler;
    py::scoped_interpreter* guard;


public slots:
	void cursorChange();


};

