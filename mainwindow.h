#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QTextEdit>
#include <QLabel>
#include <QFileDialog>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QIcon>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QPushButton *openFirstFramePushButton;
    QPushButton *openSecondFramePushButton;
    QRadioButton *sobelContourRadioButton;
    QRadioButton *cannyContourRadioButton;
    QCheckBox *showInititalFramesCheckBox;
    QCheckBox *showDifferenceBetweenFramesCheckBox;
    QCheckBox *showApplyingAndOperationResultCheckBox;
    QCheckBox *showResultOfMorphologicalFiltrationCheckBox;
    QCheckBox *showFrameContourCheckBox;
    QCheckBox *showStrobApplyingResultCheckBox;
    QTextEdit *firstFramePathTextEdit;
    QTextEdit *secondFramePathTextEdit;
    QPushButton *startPushButton;
    cv::Mat frame1;
    cv::Mat frame2;
    cv::Mat framesDifference;//межкадровая разность
    cv::Mat frameContour;//контур кадра
    cv::Mat colsHistogram;//гистограмма по столбцам
    cv::Mat rowsHistogram;//гистограмма по строкам
    cv::Mat morphFiltResult;//результат применения морфологической фильтрации
    cv::Mat andOperationResult;//результат применения логического &
    enum contourAlgorithm {SOBEL,CANNY};//выбор алгоритма по выделению контура
    QString firstFramePath;
    QString secondFramePath;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
// создание пользовательского интерфейса
    void createWidgets();
    void createLayout();
    void createConnections();
// работа с OpenCV
    void loadInitialFrames();
    void calculateDifferenceBetweenFrames();
    void calculateFrameContour(contourAlgorithm alg);
    void applyBitwiseAnd();
    void applyMorphologyFilter();
    void calculateHistograms();
    void showIntitalFrames();
    void showDifferenceBetweenFrames();
    void showBitwiseAndResult();
    void showContourOfFrame();
    void showMorphologyFilterResult();
    void showHistograms();
    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};



#endif // MAINWINDOW_H
