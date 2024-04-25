/*
* Python����̨��UI
* ����QT���
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

	// ΪPython���������
	PySharing::printOut = [=](std::string str) {
		printOut(str);
	};
	PySharing::printErr = [=](std::string str) {
		printErr(str);
	};

	// ������ǶPython������
	guard = new py::scoped_interpreter{};

	// ʹ��IOHandlerִ��Python��䣬������Python��׼����ʹ������
	iohandler = new IOHandlerSpace::IOHandler{};

	// �����ʼ��ʾ��Ϣ
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

	// ���ѡ����
	cursor.clearSelection();
	ui.plainTextEdit->setTextCursor(cursor);

	// ѡȡ�¼��루�ֶ����룩������
	cursor.movePosition(QTextCursor::End);
	int endPosition = cursor.position();
	cursor.setPosition(markPosition, QTextCursor::MoveAnchor);
	cursor.setPosition(endPosition, QTextCursor::KeepAnchor);
	QString text1 = cursor.selectedText();
	std::string stdText = text1.toStdString();

	//ɾ����Alt+Enter����ʱ�����Ķ����ַ��������ϻ��з�
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
	//����ʾ��'.'ɾ��
	for (int i = replaces.size() - 1; i >= 0; i--)
	{
		stdText.erase(replaces[i], 1);
	}
	replaces.clear();

	
#if 0
	// ����һ���߳�ִ��Python���  !!GIL ERROR!!
	std::thread t([=]() {
		py::gil_scoped_acquire lock;
		py::gil_scoped_release rel;
		// ִ��
		iohandler->exec(stdText);

		// ����һ�������ʾ��־
		putPrompt();
	});
	t.detach();
#endif

#if 1
	// ִ��
	iohandler->exec(stdText);

	// ����һ�������ʾ��־
	putPrompt();
#endif
}

// �������
void PythonConsole::printOut(std::string str)
{
	// ��ʾ��ʾ��Ϣ
	ui.plainTextEdit->setCurrentCharFormat(receiveFormat);
	ui.plainTextEdit->appendPlainText(QString::fromStdString(str));
	ui.plainTextEdit->update();

	// ���ʱ��ֹ����
	ui.plainTextEdit->setReadOnly(true);
}

// �������
void PythonConsole::printErr(std::string str)
{
	// ��ʾ��ʾ��Ϣ
	ui.plainTextEdit->setCurrentCharFormat(errorFormat);
	ui.plainTextEdit->appendPlainText(QString::fromStdString(str));
	ui.plainTextEdit->update();

	// ���ʱ��ֹ����
	ui.plainTextEdit->setReadOnly(true);
}


// ��ӡ��ʾ��������༭
void PythonConsole::putPrompt()
{
	// ��ӡ��ʾ��
	ui.plainTextEdit->setCurrentCharFormat(receiveFormat);
	ui.plainTextEdit->appendPlainText(QString::fromStdString(">>> "));
	ui.plainTextEdit->update();

	// ���ù��λ��
	QTextCursor markCursor = ui.plainTextEdit->textCursor();
	markCursor.movePosition(QTextCursor::End);
	markPosition = markCursor.position();
	ui.plainTextEdit->setTextCursor(markCursor);

	ui.plainTextEdit->setCurrentCharFormat(writeFormat);
}

// �ڿ���̨�д�ӡ��ʾ��Ϣ
void PythonConsole::putMessage(std::string msg)
{
	printOut(msg);
	putPrompt();
}


void PythonConsole::cursorChange()  // ���λ�÷����ı�ʱ�ж��Ƿ��ڿɱ༭����
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


bool PythonConsole::eventFilter(QObject* watched, QEvent* event)  // ���ü��̲���
{
	if (watched == ui.plainTextEdit && event->type() == QEvent::KeyPress)
	{
		QKeyEvent* keyEv = (QKeyEvent*)event;
		if (keyEv->key() == Qt::Key_Enter || keyEv->key() == Qt::Key_Return)
		{
			if (keyEv->modifiers() == Qt::AltModifier) 
			{
				// Enter+Alt����
				ui.plainTextEdit->setCurrentCharFormat(receiveFormat);
				ui.plainTextEdit->appendPlainText("... ");
				ui.plainTextEdit->update();
				ui.plainTextEdit->setCurrentCharFormat(writeFormat);

				// �洢��ʾ��'.'��λ��
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
				// Enter��ִ��
				exec();
			}
			return true;
		}
		if (keyEv->key() == Qt::Key_Backspace)
		{
			// ��ֹBackspaceɾ�����һ���ַ������޷�����
			QTextCursor cursor = ui.plainTextEdit->textCursor();
			int position = cursor.position();
			if (position == markPosition)
				return true;
			else
				return false;
		}
		if (keyEv->key() == Qt::Key_Left)
		{
			// ��ֹ�����ʹ���Խ���߽絼��ʧȥ���
			QTextCursor cursor = ui.plainTextEdit->textCursor();
			int position = cursor.position();
			if (position == markPosition)
				return true;
			else
				return false;
		}
		if (keyEv->key() == Qt::Key_Up)
		{
			// ��ֹ�Ϸ����ʹ���Խ���߽絼��ʧȥ���
			return true;
		}
	}
	return false;
}


