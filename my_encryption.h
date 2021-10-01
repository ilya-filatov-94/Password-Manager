#ifndef MY_ENCRYPTION_H
#define MY_ENCRYPTION_H


#include <QString>
#include <QChar>
#include "qmath.h"
#include <QDataStream>
#include <QRandomGenerator64>

const int size_array=300;
class My_Encrytion
{

protected:

    My_Encrytion();
    virtual ~My_Encrytion();

    void shuffle_data(double arr[], int, int);              //1-ый аргумент входящий массив, 2-ой размер массива, 3-ий размер блока данных
    void reverse_shuffle_data(double arr[], int, int);      //1-ый аргумент входящий массив, 2-ой размер массива, 3-ий размер блока данных

    void encrypt(QString& str, double arr[]);                //1-ый аргумент входящая строка, 2-ой аргумент выходящий массив
    QString decrypt(double arr[], QString&, int);            //1-ый аргумент входящая строка, 2-ой аргумент выходящий массив

    void write_data(QDataStream*, double arr[], int);
    int read_data(QDataStream*, double arr[]);
};


#endif // MY_ENCRYPTION_H
