#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Сегментация движущихся объектов");
    createWidgets();
    createLayout();
    createConnections();
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    cv::destroyAllWindows();
}

void MainWindow::createWidgets()
{
    openFirstFramePushButton = new QPushButton(QIcon("://open-file-icon.png"),"");
    openFirstFramePushButton->setFixedWidth(30);
    openSecondFramePushButton = new QPushButton(QIcon("://open-file-icon.png"),"");
    openSecondFramePushButton->setFixedWidth(30);
    sobelContourRadioButton = new QRadioButton("алгоритм Собеля");
    sobelContourRadioButton->setChecked(true);
    cannyContourRadioButton = new QRadioButton("алгоритм Кэнни");;
    showInititalFramesCheckBox = new QCheckBox("Показать исходные изображения");
    showInititalFramesCheckBox->setChecked(true);
    showDifferenceBetweenFramesCheckBox = new QCheckBox("Показать межкадровую разность");
    showApplyingAndOperationResultCheckBox = new QCheckBox("Показать применение объединения по &&");
    showResultOfMorphologicalFiltrationCheckBox = new QCheckBox("Показать применение морфологического фильтра");
    showStrobApplyingResultCheckBox = new QCheckBox("Показать результат наложения строба на объект");
    showStrobApplyingResultCheckBox->setChecked(true);
    showFrameContourCheckBox = new QCheckBox("Показать контур кадра");
    firstFramePathTextEdit = new QTextEdit();
    firstFramePathTextEdit->setReadOnly(true);
    firstFramePathTextEdit->setFixedHeight(30);
    secondFramePathTextEdit = new QTextEdit();
    secondFramePathTextEdit->setReadOnly(true);
    secondFramePathTextEdit->setFixedHeight(30);
    startPushButton = new QPushButton("Старт");
}

void MainWindow::createLayout()
{
    QWidget *mainWindowWidget = new QWidget();
    QGridLayout *mainWindowWidgetLayout = new QGridLayout();
    QLabel *title = new QLabel("Сегментация движущихся объектов на основе оценки энергии движения");
    title->setFont(QFont("Times",14,QFont::Bold,true));
    QGroupBox *openFramesGroupBox = new QGroupBox("Задание фреймов");
    QGridLayout *openFramesGroupBoxLayout = new QGridLayout();
    openFramesGroupBoxLayout->addWidget(firstFramePathTextEdit,0,0,1,2);
    openFramesGroupBoxLayout->addWidget(openFirstFramePushButton,0,2,1,1);
    openFramesGroupBoxLayout->addWidget(secondFramePathTextEdit,0,3,1,2);
    openFramesGroupBoxLayout->addWidget(openSecondFramePushButton,0,5,1,1);
    openFramesGroupBox->setLayout(openFramesGroupBoxLayout);
    QGroupBox *optionsGroupBox = new QGroupBox("Опции показа");
    QGridLayout *optionsGroupBoxLayout = new QGridLayout();
    optionsGroupBoxLayout->addWidget(showInititalFramesCheckBox,0,0,1,2);
    optionsGroupBoxLayout->addWidget(showDifferenceBetweenFramesCheckBox,0,2,1,2);
    optionsGroupBoxLayout->addWidget(showApplyingAndOperationResultCheckBox,1,0,1,2);
    optionsGroupBoxLayout->addWidget(showResultOfMorphologicalFiltrationCheckBox,1,2,1,2);
    optionsGroupBoxLayout->addWidget(showStrobApplyingResultCheckBox,2,0,1,2);
    optionsGroupBoxLayout->addWidget(showFrameContourCheckBox,2,2,1,2);
    optionsGroupBox->setLayout(optionsGroupBoxLayout);
    QGroupBox *algContourGroupBox = new QGroupBox("Выбор алгоритма для получения контура");
    QGridLayout *algContourGroupBoxLayout = new QGridLayout();
    algContourGroupBoxLayout->addWidget(sobelContourRadioButton,0,0,1,2);
    algContourGroupBoxLayout->addWidget(cannyContourRadioButton,0,2,1,2);
    algContourGroupBox->setLayout(algContourGroupBoxLayout);
    mainWindowWidgetLayout->addWidget(title,0,1,1,2,Qt::AlignCenter);
    mainWindowWidgetLayout->addWidget(openFramesGroupBox,1,0,1,4);
    mainWindowWidgetLayout->addWidget(algContourGroupBox,2,0,1,4);
    mainWindowWidgetLayout->addWidget(optionsGroupBox,3,0,1,4);
    mainWindowWidgetLayout->addWidget(startPushButton,4,0,1,4,Qt::AlignCenter);
    mainWindowWidget->setLayout(mainWindowWidgetLayout);
    //установка стилей
    mainWindowWidget->setStyleSheet("background-color: #0e7fc9; color: white; font-size: 12pt");
    openFramesGroupBox->setStyleSheet("background-color: #7ec4f2");
    optionsGroupBox->setStyleSheet("background-color: #7ec4f2");
    algContourGroupBox->setStyleSheet("background-color: #7ec4f2");
    firstFramePathTextEdit->setStyleSheet("background-color: white; color: black");
    secondFramePathTextEdit->setStyleSheet("background-color: white; color: black");
    setCentralWidget(mainWindowWidget);
}

void MainWindow::createConnections()
{
    connect(startPushButton,&QPushButton::clicked,[&](){
        if(!firstFramePathTextEdit->toPlainText().isEmpty() && !secondFramePathTextEdit->toPlainText().isEmpty()){
            loadInitialFrames();
            calculateDifferenceBetweenFrames();
            if(sobelContourRadioButton->isChecked()){
                calculateFrameContour(contourAlgorithm::SOBEL);
            }
            if(cannyContourRadioButton->isChecked()){
                calculateFrameContour(contourAlgorithm::CANNY);
            }
            applyBitwiseAnd();
            applyMorphologyFilter();
            calculateHistograms();
        } else {
            QMessageBox warningBox;
            warningBox.setText("Файл не выбран !");
            warningBox.setStandardButtons(QMessageBox::Ok);
            warningBox.setIcon(QMessageBox::Warning);
            int box_result = warningBox.exec();
            switch (box_result) {
            case QMessageBox::Ok:
                firstFramePathTextEdit->setStyleSheet("background-color:red");
                secondFramePathTextEdit->setStyleSheet("background-color:red");
                QTimer::singleShot(500,this,[=]{
                    firstFramePathTextEdit->setStyleSheet("background-color:white");
                    secondFramePathTextEdit->setStyleSheet("background-color:white");
                });
                break;
            default:
                break;
            }
        }
    });
    connect(openFirstFramePushButton,&QPushButton::clicked,[&](){
        firstFramePath = QFileDialog::getOpenFileName(this,"Открыть первое изображение","","Image files: (*.jpg *.png)");
        firstFramePathTextEdit->setText(firstFramePath);
    });
    connect(openSecondFramePushButton,&QPushButton::clicked,[&](){
        secondFramePath = QFileDialog::getOpenFileName(this,"Открыть второе изображение","","Image files: (*.jpg *.png)");
        secondFramePathTextEdit->setText(secondFramePath);
    });
}

void MainWindow::loadInitialFrames()
{
    frame1 = cv::imread(firstFramePath.toStdString().c_str(),cv::IMREAD_GRAYSCALE);
    frame2 = cv::imread(secondFramePath.toStdString().c_str(), cv::IMREAD_GRAYSCALE);
    //применение медианного фильтра для сглаживания
    cv::medianBlur(frame1, frame1, 3);
    cv::medianBlur(frame2, frame2, 3);
    if(showInititalFramesCheckBox->isChecked()){
        showIntitalFrames();
    }
}

void MainWindow::calculateDifferenceBetweenFrames()
{
    cv::absdiff(frame2, frame1, framesDifference);//вычисление межкадровой разности
    cv::medianBlur(framesDifference, framesDifference, 3);//применение медианного фильтра к межкадровой разности
    cv::threshold(framesDifference, framesDifference, 50, 255, cv::THRESH_BINARY_INV);//бинарное квантование межкадровой разности
    if(showDifferenceBetweenFramesCheckBox->isChecked()){
        showDifferenceBetweenFrames();
    }
}

void MainWindow::calculateFrameContour(contourAlgorithm alg)
{
    switch (alg) {
    case contourAlgorithm::SOBEL:
        cv::Sobel(frame1, frameContour, -1, 1, 1, 3);//получение контура с помощью оператора Собеля
        break;
    case contourAlgorithm::CANNY:
        cv::Canny(frame1, frameContour, 50, 200);//получение контура с помощью фильтра Кэнни
        break;
    default:
        cv::Canny(frame1, frameContour, 50, 200);//получение контура с помощью фильтра Кэнни
        break;
    }
    cv::threshold(frameContour, frameContour, 50, 255, cv::THRESH_BINARY_INV);//бинаризация контурного препарата
    if(showFrameContourCheckBox->isChecked()){
        showContourOfFrame();
    }
}

void MainWindow::showContourOfFrame()
{
    cv::imshow("Contour of frame 1", frameContour);
    cv::waitKey(400);
}

void MainWindow::showMorphologyFilterResult()
{
    cv::imshow("Morphology filter result", morphFiltResult);
    cv::waitKey(400);
}

void MainWindow::showHistograms()
{
    cv::imshow("Histogram for columns", colsHistogram);
    cv::waitKey(400);
    cv::imshow("Histogram for rows", rowsHistogram);
    cv::waitKey();
}


void MainWindow::applyBitwiseAnd()
{
    cv::bitwise_and(framesDifference, frameContour, andOperationResult);
    if(showApplyingAndOperationResultCheckBox->isChecked()){
        showBitwiseAndResult();
    }
}


void MainWindow::applyMorphologyFilter()
{
    //сначала применяется операция открытия
    cv::Mat struct_element1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    cv::morphologyEx(andOperationResult, morphFiltResult, cv::MORPH_OPEN, struct_element1);
    //затем операция закрытия
    cv::Mat struct_element2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
    cv::morphologyEx(morphFiltResult, morphFiltResult, cv::MORPH_CLOSE, struct_element2);
    if(showResultOfMorphologicalFiltrationCheckBox->isChecked()){
        showMorphologyFilterResult();
    }
}

void MainWindow::calculateHistograms()
{
    //гистограмма для строк
    cv::Mat transposedMorphFiltResult(morphFiltResult.cols, morphFiltResult.rows, CV_8U);//транспонированная матрица изображения после морфологической фильтрации
    cv::transpose(morphFiltResult, transposedMorphFiltResult);
    cv::Mat hist_r = cv::Mat::zeros(1, transposedMorphFiltResult.cols, CV_64FC1);
    for (int i = 0; i < transposedMorphFiltResult.cols; i++) {
        for (int j = 0; j < transposedMorphFiltResult.rows; j++) {
            int pix = transposedMorphFiltResult.at<unsigned char>(j, i);
            if (pix != 0) {
                hist_r.at<double>(0, i) += 1.0;
            }
        }
    }
    double min_r = 0, max_r = 0;
    cv::minMaxLoc(hist_r, &min_r, &max_r); // глобальный минимум и максимум
    hist_r = hist_r / max_r; // нормировка по высоте
    cv::Mat rows_hist_img = cv::Mat::zeros(transposedMorphFiltResult.rows, transposedMorphFiltResult.cols, CV_8U);
    for (int i = 0; i < transposedMorphFiltResult.cols; i++) {
        for (int j = 0; j < transposedMorphFiltResult.rows; j++) {
            if (hist_r.at<double>(0, i) * transposedMorphFiltResult.rows > j) {
                rows_hist_img.at<unsigned char>(transposedMorphFiltResult.rows - 1 - j, i) = 255;
            }
        }
    }
    cv::bitwise_not(rows_hist_img, rows_hist_img); // инвертируем изображение
    rowsHistogram = rows_hist_img.clone();


    //гистограмма для столбцов
    cv::Mat hist_c = cv::Mat::zeros(1, morphFiltResult.cols, CV_64FC1);
    for (int i = 0; i < morphFiltResult.cols; i++) {
        for (int j = 0; j < morphFiltResult.rows; j++) {
            int pix = morphFiltResult.at<unsigned char>(j, i);
            if (pix != 0) {
                hist_c.at<double>(0, i) += 1.0;
            }
        }
    }
    double min_c = 0, max_c = 0;
    cv::minMaxLoc(hist_c, &min_c, &max_c); // глобальный минимум и максимум
    hist_c = hist_c / max_c; // нормировка по высоте

    cv::Mat cols_hist_img = cv::Mat::zeros(morphFiltResult.rows, morphFiltResult.cols, CV_8U);

    for (int i = 0; i < morphFiltResult.cols; i++) {
        for (int j = 0; j < morphFiltResult.rows; j++) {
            if (hist_c.at<double>(0, i) * morphFiltResult.rows > j) {
                cols_hist_img.at<unsigned char>(morphFiltResult.rows - 1 - j, i) = 255;
            }
        }
    }
    cv::bitwise_not(cols_hist_img, cols_hist_img); // инвертируем изображение
    colsHistogram = cols_hist_img.clone();

    if(showStrobApplyingResultCheckBox->isChecked()){
        showHistograms();
    }
}

void MainWindow::showBitwiseAndResult()
{
    cv::imshow("Applying &", andOperationResult);
    cv::waitKey(400);
}

void MainWindow::showIntitalFrames()
{
    cv::imshow("Frame 1",frame1);
    cv::waitKey(400);
    cv::imshow("Frame 2", frame2);
    cv::waitKey(400);
}

void MainWindow::showDifferenceBetweenFrames()
{
    cv::imshow("Difference between frames", framesDifference);
    cv::waitKey(400);
}


