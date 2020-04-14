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
#include "GPSPlotWnd.h"
#include <QPushButton>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "NMEACalculate.h"

CGPSPlotWnd::CGPSPlotWnd(CNMEAParserQt *pNMEAParser, QWidget *parent) :
    QWidget(parent),
    m_pNMEAParser(pNMEAParser)
{
    CreateWidgets();
    connect(pNMEAParser, SIGNAL(NewPositionUpdateGPS()), SLOT(OnNewPositionUpdateGPS()));
}

CGPSPlotWnd::~CGPSPlotWnd(){
    delete  m_pCustomPlot;
}

void CGPSPlotWnd::CreateWidgets() {

    m_pCustomPlot = new QCustomPlot(this);

    m_pCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);/*  可使用鼠标进行缩放 */
    m_pCustomPlot->setGeometry(QRect(0,0,width(),height()));
}

void CGPSPlotWnd::OnNewPositionUpdateGPS()
{
    //
    // Draw up position status
    //
    // Get GGA data (has the position)
    //
    CNMEAParserData::GGA_DATA_T ggaData;
    m_pNMEAParser->GetGPGGA(ggaData);
    // If no position information in the GPGGA sentence, then try the GNGGA..
    if(ggaData.m_dLatitude == 0.0 && ggaData.m_dLongitude == 0.0 && ggaData.m_dLatitude == 0.0) {
        m_pNMEAParser->GetGNGGA(ggaData);
    }
    // Again, no information, try Galileo
    else if(ggaData.m_dLatitude == 0.0 && ggaData.m_dLongitude == 0.0 && ggaData.m_dLatitude == 0.0) {
        m_pNMEAParser->GetGAGGA(ggaData);
    }

    //lla2ecef(if gps quality not equip zero)
    if(ggaData.m_nGPSQuality != 0){
        double pos[3],xyz[3];
        CNMEACalculate NMEACalculate;

        pos[0] = NMEACalculate.nmea_degree2radian(ggaData.m_dLatitude);//degree2radian
        pos[1] = NMEACalculate.nmea_degree2radian(ggaData.m_dLongitude);
        pos[2] = ggaData.m_dAltitudeMSL + ggaData.m_dGeoidalSep; /* 高程采用海拔高度+高程异常值 */
        NMEACalculate.pos2ecef(pos,xyz);
        QString ecefstr = QString(
                "ecef(xyz):%1,%2,%3\r\n"
                ).arg(xyz[0], 0, 'f', 4, QChar('0'))
                .arg(xyz[1], 0, 'f', 4, QChar('0'))
                .arg(xyz[2], 0, 'f', 4, QChar('0'))
                ;
        qDebug() << ecefstr;
        Position_Data_Preprocess(xyz[0], xyz[1]);
    }
}

void CGPSPlotWnd::resizeEvent(QResizeEvent */*event*/)
{
    m_pCustomPlot->setGeometry(QRect(0,0,width(),height()));
}
/*
QSize CGPSDataStreamWnd::sizeHint () const{
    return QSize(300, 200);
}
*/

void CGPSPlotWnd::OnClearPlotHistory()
{
    last_index = 0;
    display_x.clear();
    display_y.clear();
    m_pCustomPlot->removeGraph(0);
}

//数据准备
void CGPSPlotWnd::Position_Data_Preprocess(double x, double y)
{
    //y轴的显示范围最大为65535，该值可以根据传感器的取值范围做调整
//    if( y < 65536 && y > 1 && x > 1)
    {
        display_x.append(x);
        display_y.append(y);
        if(display_x.size()> 1)
        {
            Position_Draw_Plot(m_pCustomPlot,display_x,display_y);
        }
        if(display_x.size() == 1)
        {
            m_pCustomPlot->addGraph(0);
            m_pCustomPlot->xAxis->setRange(display_x.at(0)-0.5,display_x.at(0)+0.5);
            m_pCustomPlot->yAxis->setRange(display_y.at(0)-0.5,display_y.at(0)+0.5);
        }
    }
}

//曲线绘制
void CGPSPlotWnd::Position_Draw_Plot(QCustomPlot *customPlot, QVector<double> x, QVector<double> y)
{
    customPlot->graph(0)->setData(x, y);
    //设置坐标轴自适应
    customPlot->graph(0)->rescaleAxes(true);
    //设置Y轴
    customPlot->graph(0)->rescaleValueAxis(true);
    //设置X轴
    customPlot->graph(0)->rescaleKeyAxis(true);
    //设置四周坐标框
    customPlot->axisRect()->setupFullAxesBox(true);
    //设置线型
    //customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);/* 无连线 */
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssTriangle);/* 点型：三角形 */
    //开始画图
    customPlot->replot();
}
