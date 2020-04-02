/*
* MIT License
*
*  Copyright (c) 2020 Caesar(792910363@qq.com)
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*  SOFTWARE.
*
*/
#include "GPSDataStreamWnd.h"
#include <QPushButton>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>


CGPSDataStreamWnd::CGPSDataStreamWnd(CNMEAParserQt *pNMEAParser, QWidget *parent) :
    QWidget(parent),
    m_pNMEAParser(pNMEAParser)
{
    CreateWidgets();
    connect(pNMEAParser, SIGNAL(NewMessageUpdateGPS(QByteArray &)), this, SLOT(OnNewMessageUpdateGPS(QByteArray &)));
}

CGPSDataStreamWnd::~CGPSDataStreamWnd(){
    delete  m_pTextBrowser;
}

void CGPSDataStreamWnd::CreateWidgets() {

    m_pTextBrowser = new QTextBrowser(this);

    m_pTextBrowser->setReadOnly(true);
    m_pTextBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pTextBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_pTextBrowser->setGeometry(QRect(0,0,width(),height()));


}

void CGPSDataStreamWnd::OnNewMessageUpdateGPS(QByteArray &array)
{
    QString string = QString::fromLatin1(array);

    m_pTextBrowser->insertPlainText(string);
    m_pTextBrowser->moveCursor(QTextCursor::End);//接收框始终定为在末尾一行
}

void CGPSDataStreamWnd::resizeEvent(QResizeEvent */*event*/)
{
    m_pTextBrowser->setGeometry(QRect(0,0,width(),height()));
}
/*
QSize CGPSDataStreamWnd::sizeHint () const{
    return QSize(300, 200);
}
*/

void CGPSDataStreamWnd::OnClearDataStreamHistory()
{
    m_pTextBrowser->clear();
}
