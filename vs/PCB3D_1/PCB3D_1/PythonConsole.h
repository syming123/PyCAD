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
	int markPosition;  // ��һ�ζ�ȡ��Ĺ��λ��
	bool editable;  // �ı��Ƿ�ɱ༭�����ݹ��λ���ж�
    std::vector<int> replaces;  // ��������ʱ��ʾ��'.'(dot)��λ�ã���ִ��ǰɾ��
    QTextCharFormat receiveFormat, writeFormat, errorFormat;  // ����,д��,����ʱ��������ʽ
    IOHandlerSpace::IOHandler* iohandler;
    py::scoped_interpreter* guard;


public slots:
	void cursorChange();


};

