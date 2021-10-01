#include "my_encryption.h"
#include <QDebug>

My_Encrytion::My_Encrytion()
{
}

My_Encrytion::~My_Encrytion()
{
    //qDebug() << "destroyed shuffle";
}

void My_Encrytion::encrypt(QString& str, double arr[size_array])
{
    QChar symbol_array[size_array];            //Массив символов QChar
    int int_arr_symbol[size_array];            //Массив типа int кодов символов unicode
    double encrypt_code[size_array];           //Промежуточный массив для шифрования кодов символов

    //Перевод символов строки в массив double
    for (int i=0; i<str.size(); i++)
         {
          symbol_array[i]=str[i];
          int_arr_symbol[i]=(symbol_array[i].unicode());
          arr[i]=static_cast<double>(int_arr_symbol[i]);
          encrypt_code[i]=arr[i];
         }
    for (int i=0; i<str.size(); i++)
    {
        arr[i]=pow(encrypt_code[i],4)*17+42;
    }
}

QString My_Encrytion::decrypt(double arr[size_array], QString& str, int size)
{
    int int_arr_symbol[size_array];            //Массив типа int кодов символов unicode
    double decrypt_code[size_array];           //Промежуточный массив для шифрования кодов символов
    for (int i=0; i<size; i++)
    {
        decrypt_code[i]=arr[i];
    }
    for (int i=0; i<size; i++)
    {
        arr[i]=pow(((decrypt_code[i]-42)/17),0.25);
    }
    //Перевод массива double в символы строки
    for (int i=0; i<size; i++)
         {
            int_arr_symbol[i]=static_cast<int>(arr[i]);
            str[i]=QChar(int_arr_symbol[i]);
            arr[i]=0;                  //Очистка
         }
    return str;
}

void My_Encrytion::write_data(QDataStream* arg_stream, double arr[size_array], int size)
{
    double temp_array[size_array];
    int filler=size;
    while ((filler%8)!=0)      //размер результрующего массива
    {
        filler+=1;             //дополнение количества элементов до размера блока
    }
    for (int i=0; i<filler; i++)
    {
        temp_array[i]=arr[i];
    }
    for (int i=0; i<filler; i++)
    {
        arr[i+1]=temp_array[i];
    }
    arr[0]=pow(static_cast<double>(size),4)*17+42;
    for (int i=0; i<(filler+1); i++)
    {
        *arg_stream << (arr[i]);
    }
}

int My_Encrytion::read_data(QDataStream* arg_stream, double arr[size_array])
{
    *arg_stream >> (arr[0]);
    int size=static_cast<int>(pow(((arr[0]-42)/17),0.25));
    int filler=size;
    while ((filler%8)!=0)      //размер результрующего массива
    {
        filler+=1;             //дополнение количества элементов до размера блока
    }
    for (int i=0; i<(filler); i++)
    {
        *arg_stream >> (arr[i]);
    }
    return size;
}


void My_Encrytion:: shuffle_data(double arr[size_array], int size, int size_of_block)
{
    int filler_size=size;                         //первоначальная инициализация размером входящего массива
    while ((filler_size%size_of_block)!=0)        //дополнение количества элементов до кратного размеру блока данных
    {
        filler_size+=1;                           //размер результирующего массива
    }
    int number_blocks=(filler_size+1)/8-1;        //вычисление количества блоков
    int half1=size_of_block/2;                    //Вычисление середины блока
    int half2;                                    //Середина количества блоков
    double shuffle_inside_array[size_array];      //Промежуточный массив для перемешивания внутри блоков
    double shuffle_temp_block_arr1[size_array];   //промежуточный одномерный массив - результат до перемешивания блоков
    for (int i=0; i<300; i++)                     //Первоначальная инициализация всего массива
    {
        shuffle_inside_array[i]=0;
        shuffle_temp_block_arr1[i]=0;
        if (i>=size)
        {
            std::uniform_int_distribution<int>distribution(52898870,60000000);      //Инициализация остальных элементов входящего массива
            arr[i] = distribution(*QRandomGenerator::global());
        }
    }
    for (int i=0; i<size_array; i++)
    {
        shuffle_inside_array[i]=arr[i];           //Заполнение промежуточного массива
    }
    for (int i=0; i<(number_blocks+1); i++)       //Перемешивание внутри каждого блока
    {
        for (int j=0; j<half1; j++)
        {
            shuffle_temp_block_arr1[2*j+size_of_block*i]=shuffle_inside_array[j+half1+size_of_block*i];
            shuffle_temp_block_arr1[2*j+1+size_of_block*i]=shuffle_inside_array[j+size_of_block*i];
        }
    }
    if (number_blocks>0)                          //если количество блоков больше 1
    {
        double array_shuffle_of_block1[size_array][8];   //промежуточный массив двумерный для перемешивания блоков
        double array_shuffle_of_block2[size_array][8];   //промежуточный массив двумерный для перемешивания блоков
        double shuffle_temp_block_arr2[size_array];      //промежуточный одномерный массив - результат после перемешивания блоков
        for (int i=0; i<size_array; i++)                 //Инициализация
        {
            for (int j=0; j<size_of_block; j++)
            {
                array_shuffle_of_block1[i][j]=0;
                array_shuffle_of_block2[i][j]=0;
            }
            shuffle_temp_block_arr2[i]=0;
        }
        for (int i=0; i<(number_blocks+1); i++)   //Распределение блоков в двумерный массив перед перемешиванием
        {
            for (int j=0; j<size_of_block; j++)
            {
                array_shuffle_of_block1[i][j]=shuffle_temp_block_arr1[size_of_block*i+j];
            }
        }
        if (number_blocks%2==0)                  //если количество блоков чётное
        {
            half2=(number_blocks/2);             //вычисление середины в количестве блоков
        }
        else                                     //если количество блоков нечетное
        {
            half2=(number_blocks/2)+1;           //вычисление середины в количестве блоков
        }
        for (int i=0; i<(half2+1); i++)          //Перемешивание самих блоков
        {
            for (int j=0; j<size_of_block; j++)
            {
                array_shuffle_of_block2[2*i][j]=array_shuffle_of_block1[i+half2][j];
                array_shuffle_of_block2[2*i+1][j]=array_shuffle_of_block1[i][j];
            }
        }
        for (int i=0; i<(number_blocks+1); i++)   //Распределение блоков в одномерный массив после перемешивания
        {
            for (int j=0; j<size_of_block; j++)
            {
                shuffle_temp_block_arr2[size_of_block*i+j]=array_shuffle_of_block2[i][j];
            }
        }
        for (int i=0; i<filler_size; i++)         //Результат перемешивания блоков и внутри блоков
        {
            arr[i]=shuffle_temp_block_arr2[i];
        }
    }
    else                                          //Если количество блоков меньше 1
    {
        for (int i=0; i<filler_size; i++)         //Конечный результат перемешивания
        {
            arr[i]=shuffle_temp_block_arr1[i];
        }
    }
}

void My_Encrytion:: reverse_shuffle_data(double arr[size_array], int size, int size_of_block)
{
    int filler_size=size;                       //первоначальная инициализация размера результрующего массива
    while ((filler_size%size_of_block)!=0)      //размер результрующего массива
    {
        filler_size+=1;                         //дополнение количества элементов до размера блока
    }
    int number_blocks=(filler_size+1)/8-1;      //вычисление количества блоков
    int half1=size_of_block/2;                  //Вычисление середины блока
    int half2;                                  //Середина количества блоков
    double shuffle_inside_array[300];           //Промежуточный массив для перемешивания внутри блоков
    double shuffle_temp_block_arr1[300];        //промежуточный одномерный массив - результат до перемешивания блоков
    for (int i=0; i<size_array; i++)                   //Первоначальная инициализация всего массива
    {
        shuffle_inside_array[i]=0;
        shuffle_temp_block_arr1[i]=0;
    }
    for (int i=0; i<size_array; i++)                   //Заполнение промежуточного массива
    {
        shuffle_inside_array[i]=arr[i];
    }
    for (int i=0; i<(number_blocks+1); i++)     //Обратное перемешивание внутри каждого блока
    {
        for (int j=0; j<half1; j++)
        {
            shuffle_temp_block_arr1[j+half1+size_of_block*i]=shuffle_inside_array[2*j+size_of_block*i];
            shuffle_temp_block_arr1[j+size_of_block*i]=shuffle_inside_array[2*j+1+size_of_block*i];
        }
    }
    if (number_blocks>0)                          //Если количество блоков больше 1
    {
        double array_shuffle_of_block1[size_array][8];   //промежуточный массив двумерный для перемешивания блоков
        double array_shuffle_of_block2[size_array][8];   //промежуточный массив двумерный для перемешивания блоков
        double shuffle_temp_block_arr2[size_array];      //промежуточный одномерный массив - результат после перемешивания блоков

        for (int i=0; i<300; i++)                 //Инициализация
        {
            for (int j=0; j<size_of_block; j++)
            {
                array_shuffle_of_block1[i][j]=0;
                array_shuffle_of_block2[i][j]=0;
            }
            shuffle_temp_block_arr2[i]=0;
        }
        for (int i=0; i<(number_blocks+1); i++)     //Распределение блоков в двумерный массив перед перемешиванием
        {
            for (int j=0; j<size_of_block; j++)
            {
                array_shuffle_of_block1[i][j]=shuffle_temp_block_arr1[size_of_block*i+j];
            }
        }
        if (number_blocks%2==0)                     //если количество блоков чётное
        {
            half2=(number_blocks/2);                //вычисление середины в количестве блоков
        }
        else                                        //если количество блоков нечетное
        {
            half2=(number_blocks/2)+1;              //вычисление середины в количестве блоков
        }
        for (int i=0; i<(half2+1); i++)             //Обратное перемешивание самих блоков!!!!!!!!!!!!!!!!!
        {
            for (int j=0; j<size_of_block; j++)
            {
                array_shuffle_of_block2[i+half2][j]=array_shuffle_of_block1[2*i][j];
            }
        }
        for (int i=0; i<(half2); i++)
        {
            for (int j=0; j<size_of_block; j++)
            {
                array_shuffle_of_block2[i][j]=array_shuffle_of_block1[2*i+1][j];
            }
        }
        for (int i=0; i<(number_blocks+1); i++)     //Распределение блоков в одномерный массив после перемешивания
        {
            for (int j=0; j<size_of_block; j++)
            {
                shuffle_temp_block_arr2[size_of_block*i+j]=array_shuffle_of_block2[i][j];
            }
        }
        for (int i=0; i<filler_size; i++)           //Результат перемешивания блоков и внутри блоков
        {
            arr[i]=shuffle_temp_block_arr2[i];
        }
    }
    else    //Если количество блоков меньше 1
    {
        for (int i=0; i<filler_size; i++)           //Конечный результат перемешивания
        {
            arr[i]=shuffle_temp_block_arr1[i];
        }
    }
}
