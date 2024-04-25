/*
* Python控制台的UI
* 基于QT组件
*/

#include "PythonConsole.h"

PythonConsole::PythonConsole(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.plainTextEdit->installEventFilter(this);

	QFont font = QFont("Consolas",10);
	ui.plainTextEdit->setFont(font);
	receiveFormat.setForeground(QBrush(Qt::black));
	writeFormat.setForeground(QBrush(Qt::blue));
	errorFormat.setForeground(QBrush(Qt::red));

	connect(ui.plainTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChange()));

	// 为Python绑定输出函数
	PySharing::printOut = [=](std::string str) {
		printOut(str);
	};
	PySharing::printErr = [=](std::string str) {
		printErr(str);
	};

	// 启动内嵌Python解释器
	guard = new py::scoped_interpreter{};

	// 使用IOHandler执行Python语句，并捕获Python标准输出和错误输出
	iohandler = new IOHandlerSpace::IOHandler{};

	// 输入初始提示信息
	putMessage("python console started...\n");
}


PythonConsole::~PythonConsole()
{
	delete iohandler;
	delete guard;
}


void PythonConsole::exec()
{
	QTextCursor cursor = ui.plainTextEdit->textCursor();

	// 清除选择标记
	cursor.clearSelection();
	ui.plainTextEdit->setTextCursor(cursor);

	// 选取新加入（手动输入）的内容
	cursor.movePosition(QTextCursor::End);
	int endPosition = cursor.position();
	cursor.setPosition(markPosition, QTextCursor::MoveAnchor);
	cursor.setPosition(endPosition, QTextCursor::KeepAnchor);
	QString text1 = cursor.selectedText();
	std::string stdText = text1.toStdString();

	//删掉用Alt+Enter换行时产生的多余字符，并补上换行符
	for (int i = stdText.length() - 1; i >= 0; i--)
	{
		if (stdText[i] == -30)
		{
			stdText[i] = '\n';
		}
		else if (stdText[i] < 0)
		{
			stdText.erase(i, 1);
		}
	}
	//将提示符'.'删掉
	for (int i = replaces.size() - 1; i >= 0; i--)
	{
		stdText.erase(replaces[i], 1);
	}
	replaces.clear();

	
#if 0
	// 启动一个线程执行Python语句  !!GIL ERROR!!
	std::thread t([=]() {
		py::gil_scoped_acquire lock;
		py::gil_scoped_release rel;
		// 执行
		iohandler->exec(stdText);

		// 在下一行添加提示标志
		putPrompt();
	});
	t.detach();
#endif

#if 1
	// 执行
	iohandler->exec(stdText);

	// 在下一行添加提示标志
	putPrompt();
#endif
}

// 正常输出
void PythonConsole::printOut(std::string str)
{
	// 显示提示信息
	ui.plainTextEdit->setCurrentCharFormat(receiveFormat);
	ui.plainTextEdit->appendPlainText(QString::fromStdString(str));
	ui.plainTextEdit->update();

	// 输出时禁止输入
	ui.plainTextEdit->setReadOnly(true);
}

// 错误输出
void PythonConsole::printErr(std::string str)
{
	// 显示提示信息
	ui.plainTextEdit->setCurrentCharFormat(errorFormat);
	ui.plainTextEdit->appendPlainText(QString::fromStdString(str));
	ui.plainTextEdit->update();

	// 输出时禁止输入
	ui.plainTextEdit->setReadOnly(true);
}


// 打印提示符并允许编辑
void PythonConsole::putPrompt()
{
	// 打印提示符
	ui.plainTextEdit->setCurrentCharFormat(receiveFormat);
	ui.plainTextEdit->appendPlainText(QString::fromStdString(">>> "));
	ui.plainTextEdit->update();

	// 设置光标位置
	QTextCursor markCursor = ui.plainTextEdit->textCursor();
	markCursor.movePosition(QTextCursor::End);
	markPosition = markCursor.position();
	ui.plainTextEdit->setTextCursor(markCursor);

	ui.plainTextEdit->setCurrentCharFormat(writeFormat);
}

// 在控制台中打印提示信息
void PythonConsole::putMessage(std::string msg)
{
	printOut(msg);
	putPrompt();
}


void PythonConsole::cursorChange()  // 光标位置发生改变时判断是否处于可编辑区域
{
	QTextCursor cursor = ui.plainTextEdit->textCursor();
	//int position = cursor.position();
	int start = cursor.selectionStart();
	int end = cursor.selectionEnd();
	if(start >= markPosition && end >=markPosition)
	{
		ui.plainTextEdit->setReadOnly(false);
		ui.plainTextEdit->setCurrentCharFormat(writeFormat);
	}
	else
	{
		ui.plainTextEdit->setReadOnly(true);
	}
	
	
	//qDebug() << "mark position:" << markPosition << "\tposition:" << position << endl;
	//qDebug() << "start:" << start << "\tend:" << end << endl;

}


bool PythonConsole::eventFilter(QObject* watched, QEvent* event)  // 设置键盘操作
{
	if (watched == ui.plainTextEdit && event->type() == QEvent::KeyPress)
	{
		QKeyEvent* keyEv = (QKeyEvent*)event;
		if (keyEv->key() == Qt::Key_Enter || keyEv->key() == Qt::Key_Return)
		{
			if (keyEv->modifiers() == Qt::AltModifier) 
			{
				// Enter+Alt换行
				ui.plainTextEdit->setCurrentCharFormat(receiveFormat);
				ui.plainTextEdit->appendPlainText("... ");
				ui.plainTextEdit->update();
				ui.plainTextEdit->setCurrentCharFormat(writeFormat);

				// 存储提示符'.'的位置
				QTextCursor markCursor = ui.plainTextEdit->textCursor();
				markCursor.movePosition(QTextCursor::End);
				int endRelativePos = markCursor.position() - markPosition;
				replaces.push_back(endRelativePos - 4);
				replaces.push_back(endRelativePos - 3);
				replaces.push_back(endRelativePos - 2);
				replaces.push_back(endRelativePos - 1);
			}
			else
			{
				// Enter键执行
				exec();
			}
			return true;
		}
		if (keyEv->key() == Qt::Key_Backspace)
		{
			// 防止Backspace删掉最后一个字符导致无法输入
			QTextCursor cursor = ui.plainTextEdit->textCursor();
			int position = cursor.position();
			if (position == markPosition)
				return true;
			else
				return false;
		}
		if (keyEv->key() == Qt::Key_Left)
		{
			// 防止左方向键使光标越过边界导致失去光标
			QTextCursor cursor = ui.plainTextEdit->textCursor();
			int position = cursor.position();
			if (position == markPosition)
				return true;
			else
				return false;
		}
		if (keyEv->key() == Qt::Key_Up)
		{
			// 防止上方向键使光标越过边界导致失去光标
			return true;
		}
	}
	return false;
}


