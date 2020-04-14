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
#ifndef GPSPLOTWND_H
#define GPSPLOTWND_H

#include <QWidget>
#include "NMEAParserQt.h"
#include "qcustomplot.h"      //包含Qcustomplot库

class CGPSPlotWnd : public QWidget
{
private:
    Q_OBJECT

    CNMEAParserQt *             m_pNMEAParser;              ///< The main parser

    QCustomPlot *              m_pCustomPlot;               ///< QCustomPlot

    long last_index;
    QVector<double> display_x;
    QVector<double> display_y;

private:
    void Position_Data_Preprocess(double x, double y);
    void Position_Draw_Plot(QCustomPlot *customPlot, QVector<double> x, QVector<double> y);

public:
    explicit CGPSPlotWnd(CNMEAParserQt *pNMEAParser, QWidget *parent = nullptr);
    virtual ~CGPSPlotWnd();

    QCustomPlot * GetCustomPlot(void) { return m_pCustomPlot; }



signals:

public slots:
    void OnClearPlotHistory();

protected slots:
    void OnNewPositionUpdateGPS();
//    QSize sizeHint() const;


protected:
    void CreateWidgets();
    virtual void resizeEvent(QResizeEvent *event) override;

private:


};

#endif // GPSPLOTWND_H
