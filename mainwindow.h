#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include "picture.h"
#include "gallery.h"
#include <QElapsedTimer>

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void openFile();
    void bubbleSortGallery();
    void insertionSortGallery();
    void selectionSortGallery();
    void randomizeGallery();
    void startInsertionSort();
    void startBubbleSort();
    void startSelectionSort();
    void startMergeSort();
    void mergeSortGallery();
    void merge(std::vector<Picture*>& vec, int left, int mid, int right);
    void heapify(vector<Picture *> &vec, int n, int i);
    void heapSortGallery();
    void startHeapSort();
    void startQuickSort();
    void quickSortGallery();
    int partition(std::vector<Picture*>& vec, int low, int high);


private:
    QLabel *timerLabel;
    QElapsedTimer sortingTimer;
    QTimer *sortTimer;
    void drawGallery();
    int i = 0, j = 0;
    bool isSorting = false;
    QPushButton *openButton;
    QPixmap originalImage;
    Gallery gallery;
    bool imageOpened = false;
    int pieceWidth, pieceHeight, totalCount;
    int low; //for quicksort
    int high; //for quicksort
    int currentSize; //for merge sort
    int leftStart; // for merge sort
    vector<QLabel*> labelVector;
    void clearLabels();
};

#endif // MAINWINDOW_H
