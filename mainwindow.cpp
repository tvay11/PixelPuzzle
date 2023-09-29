#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <algorithm>
#include <random>
#include <iostream>
#include <QThread>
#include <QTimer>
#include <cmath>
#include <QElapsedTimer>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          openButton(new QPushButton("Open File", this))
{
    setFixedSize(2000, 1300);
    openButton->setGeometry(20, 20, 100, 30);
    connect(openButton, &QPushButton::clicked, this, &MainWindow::openFile);

    QPushButton *randomizeButton = new QPushButton("Randomize", this);
    randomizeButton->setGeometry(220, 20, 100, 30);
    connect(randomizeButton, SIGNAL(clicked()), this, SLOT(randomizeGallery()));

    QPushButton *bubbleSortButton = new QPushButton("Bubble Sort", this);
    bubbleSortButton->setGeometry(420, 20, 100, 30);
    connect(bubbleSortButton, SIGNAL(clicked()), this, SLOT(startBubbleSort()));

    QPushButton *insertionSortButton = new QPushButton("Insertion Sort", this);
    insertionSortButton->setGeometry(620, 20, 100, 30);
    connect(insertionSortButton, SIGNAL(clicked()), this, SLOT(startInsertionSort()));


    QPushButton *selectionSortButton = new QPushButton("Selection Sort", this);
    selectionSortButton->setGeometry(820, 20, 100, 30);
    connect(selectionSortButton, SIGNAL(clicked()), this, SLOT(startSelectionSort()));

    QPushButton *mergeSortButton = new QPushButton("Merge Sort", this);
    mergeSortButton->setGeometry(1020, 20, 100, 30);
    connect(mergeSortButton, SIGNAL(clicked()), this, SLOT(startMergeSort()));

    QPushButton *heapSortButton = new QPushButton("Heap Sort", this);
    heapSortButton->setGeometry(1220, 20, 100, 30);
    connect(heapSortButton, SIGNAL(clicked()), this, SLOT(startHeapSort()));

    QPushButton *quickSortButton = new QPushButton("Quick Sort", this);
    quickSortButton->setGeometry(1420, 20, 100, 30);
    connect(quickSortButton, SIGNAL(clicked()), this, SLOT(startQuickSort()));

    sortTimer = new QTimer(this);

    timerLabel = new QLabel("0 ms", this);
    timerLabel->setGeometry(1850, 10, 200, 30);
}

MainWindow::~MainWindow()
{
}

void MainWindow::openFile()
{
    if (isSorting) return;

    if (imageOpened) {
        gallery.clearGallery();
        imageOpened = false;
        clearLabels();
    }


    QString fileName = QFileDialog::getOpenFileName(this, "Open Image File", "", "Images (*.png *.jpg *.bmp)");
    if (fileName.isEmpty()) return;

    if (!originalImage.load(fileName)) {
        QMessageBox::warning(this, "Invalid File", "Could not open the selected file.");
        return;
    }
    QPixmap scaledImage = originalImage.scaled(1920,1080, Qt::KeepAspectRatio);


    pieceWidth = scaledImage.width() / 6;
    pieceHeight = scaledImage.height() / 6;

    // original size
//    pieceWidth = originalImage.width() / 6;
//    pieceHeight = originalImage.height() / 6;
    totalCount = 36;

    for (int x = 0; x < 6; ++x) {
        for (int y = 0; y < 6; ++y) {
            QPixmap piece = scaledImage.copy(y * pieceWidth, x * pieceHeight, pieceWidth, pieceHeight);

            Picture* picturePiece = new Picture(piece, x * sqrt(totalCount) + y);
            gallery.addPicture(picturePiece);
        }
    }

    drawGallery();

    imageOpened = true;
}

void MainWindow::startBubbleSort() {
    if (isSorting) return;
    isSorting = true;
    i = j = 0;
    sortingTimer.start();
    sortTimer->disconnect();
    connect(sortTimer, SIGNAL(timeout()), this, SLOT(bubbleSortGallery()));
    sortTimer->start(50);
}

void MainWindow::startSelectionSort() {
    if (isSorting) return;
    isSorting = true;
    i = 0;
    sortingTimer.start();
    sortTimer->disconnect();
    connect(sortTimer, SIGNAL(timeout()), this, SLOT(selectionSortGallery()));
    sortTimer->start(50);
}

void MainWindow::bubbleSortGallery() {
    if (!isSorting) return;
    int n = gallery.album.size();
    if (i < n - 1) {
        if (j < n - i - 1) {
            if (gallery.album[j]->getId() > gallery.album[j + 1]->getId()) {
                swap(gallery.album[j], gallery.album[j + 1]);
            }
            j++;
        } else {
            i++;
            j = 0;
        }
        drawGallery();
        qint64 elapsedTime = sortingTimer.elapsed();
        timerLabel->setText(QString("Sorting time: %1 ms").arg(elapsedTime));
    } else {
        sortTimer->stop();
        i = j = 0;
        isSorting = false;
    }
}

void MainWindow::selectionSortGallery() {
    if (!isSorting) return;
    int n = gallery.album.size();
    if (i < n - 1) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (gallery.album[j]->getId() < gallery.album[minIndex]->getId()) {
                minIndex = j;
            }
        }
        swap(gallery.album[i], gallery.album[minIndex]);
        i++;
        drawGallery();
        qint64 elapsedTime = sortingTimer.elapsed();
        timerLabel->setText(QString("Sorting time: %1 ms").arg(elapsedTime));
    } else {
        sortTimer->stop();
        i = 0;
        isSorting = false;
    }
}


void MainWindow::insertionSortGallery() {
    int n = gallery.album.size();
    if (i < n) {
        int j = i;
        while (j > 0 && gallery.album[j]->getId() < gallery.album[j - 1]->getId()) {
            swap(gallery.album[j], gallery.album[j - 1]);
            j--;
        }
        drawGallery();
        i++;
        qint64 elapsedTime = sortingTimer.elapsed();
        timerLabel->setText(QString("Sorting time: %1 ms").arg(elapsedTime));
    } else {
        sortTimer->stop();
        i = 0;
        isSorting = false;
        qDebug() << "OK";
    }
}

void MainWindow::drawGallery() {
    int numCols = sqrt(totalCount);

    for (int i = 0; i < totalCount; ++i) {
        Picture* picture = gallery.getPicture(i);
        if (picture != nullptr) {
            QLabel* label = new QLabel(this);
            label->setPixmap(picture->getPixmap());
            label->setGeometry(50 + (i % numCols) * pieceWidth, 50 + (i / numCols) * pieceHeight, pieceWidth, pieceHeight);
            labelVector.push_back(label);
            label->show();
        } else {
            qDebug() << "Null picture at index " << i;
        }
    }
}

void MainWindow::clearLabels() {
    for(QLabel* label : labelVector) {
        if(label != nullptr) {
            delete label;
        }
    }
    labelVector.clear();
}

void MainWindow::randomizeGallery() {
    cout<< "random" << endl;
    random_device rd;
    mt19937 g(rd());
    shuffle(gallery.album.begin(), gallery.album.end(), g);
    drawGallery();
}

void MainWindow::startInsertionSort() {
    if (isSorting) return;
    isSorting = true;
    sortingTimer.start();
    sortTimer->disconnect();
    connect(sortTimer, SIGNAL(timeout()), this, SLOT(insertionSortGallery()));
    sortTimer->start(50);
    isSorting = false;
}

void MainWindow::startMergeSort()
{
    if (isSorting) return;
    isSorting = true;

    currentSize = 1;
    leftStart = 0;

    sortingTimer.restart();
    sortTimer->disconnect();
    connect(sortTimer, SIGNAL(timeout()), this, SLOT(mergeSortGallery()));
    sortTimer->start(50);
    isSorting = false;
}

void MainWindow::mergeSortGallery()
{
    int n = gallery.album.size();


    if (leftStart >= n) {
        currentSize *= 2;
        leftStart = 0;

        if (currentSize >= n) {
            sortTimer->stop();
            isSorting = false;
            return;
        }
    }

    int mid = min(leftStart + currentSize - 1, n - 1);
    int rightEnd = min(leftStart + 2 * currentSize - 1, n - 1);

    merge(gallery.album, leftStart, mid, rightEnd);

    drawGallery();

    qint64 elapsedTime = sortingTimer.elapsed();
    timerLabel->setText(QString("Sorting time: %1 ms").arg(elapsedTime));

    leftStart += 2 * currentSize;
}

void MainWindow::merge(vector<Picture*>& vec, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Picture*> L(n1);
    vector<Picture*> R(n2);

    for (int i = 0; i < n1; i++) {
        L[i] = vec[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = vec[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (L[i]->getId() <= R[j]->getId()) {
            vec[k] = L[i];
            i++;
        } else {
            vec[k] = R[j];
            j++;
        }
        k++;
        drawGallery();
    }

    while (i < n1) {
        vec[k] = L[i];
        i++;
        k++;
        drawGallery();
    }

    while (j < n2) {
        vec[k] = R[j];
        j++;
        k++;
        drawGallery();
    }
}

void MainWindow::heapify(vector<Picture*>& vec, int n, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && vec[l]->getId() > vec[largest]->getId())
        largest = l;

    if (r < n && vec[r]->getId() > vec[largest]->getId())
        largest = r;

    if (largest != i) {
        swap(vec[i], vec[largest]);

        drawGallery();
        qint64 elapsedTime = sortingTimer.elapsed();
        timerLabel->setText(QString("Sorting time: %1 ms").arg(elapsedTime));

        heapify(vec, n, largest);
    }
}

void MainWindow::heapSortGallery()
{
    int n = gallery.album.size();

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(gallery.album, n, i);

    for (int i = n - 1; i >= 0; i--) {
        swap(gallery.album[0], gallery.album[i]);

        drawGallery();
        qint64 elapsedTime = sortingTimer.elapsed();
        timerLabel->setText(QString("Sorting time: %1 ms").arg(elapsedTime));

        heapify(gallery.album, i, 0);
    }

    sortTimer->stop();
    isSorting = false;
}


void MainWindow::startHeapSort()
{
    if (isSorting) return;
    isSorting = true;
    sortingTimer.start();
    sortTimer->disconnect();
    connect(sortTimer, SIGNAL(timeout()), this, SLOT(heapSortGallery()));
    sortTimer->start(50);
}

void MainWindow::startQuickSort()
{
    if (isSorting) return;

    isSorting = true;
    low = 0;
    high = gallery.album.size() - 1;

    sortingTimer.start();
    sortTimer->disconnect();
    connect(sortTimer, SIGNAL(timeout()), this, SLOT(quickSortGallery()));
    sortTimer->start(50);
    isSorting = false;
}

void MainWindow::quickSortGallery()
{
    cout<<"running"<<endl;
    if (low < high) {
        int pi = partition(gallery.album, low, high);

        drawGallery();
        qint64 elapsedTime = sortingTimer.elapsed();
        timerLabel->setText(QString("Sorting time: %1 ms").arg(elapsedTime));

        low = low;
        high = pi - 1;
        quickSortGallery();

        low = pi + 1;
        high = high;
        quickSortGallery();
    }
    else {
        sortTimer->stop();
        isSorting = false;
    }
}

int MainWindow::partition(vector<Picture*>& vec, int low, int high)
{
    int pivot = vec[high]->getId();
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (vec[j]->getId() < pivot) {
            i++;
            swap(vec[i], vec[j]);
            drawGallery();
            qint64 elapsedTime = sortingTimer.elapsed();
            timerLabel->setText(QString("Sorting took %1 ms").arg(elapsedTime));
        }
    }
    swap(vec[i + 1], vec[high]);

    return (i + 1);
}

