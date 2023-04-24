/**
 Загрузка данных в формате CSV из файла в массив - пример использования.

 См. комментарии в csvimport.h

 MIT License
 Copyright (c) 2023 Александр Коростелин https://github.com/flexits
**/

#include <stdio.h>
#include <stdlib.h>

#include "csvimport.h"

#define SRCF_FILE_NAME "durn.csv"   // файл таблицы с дробными значениями
#define SRCI_FILE_NAME "conn.csv"   // файл таблицы с целыми значениями

int main()
{
    // размер таблиц
    dimensns_t dataf_table_size = {0, 0};
    dimensns_t datai_table_size = {0, 0};

    // валидация содержимого файлов и вычисление размеров таблиц
    FILE *fptrf = fopen(SRCF_FILE_NAME, "r");
    FILE *fptri = fopen(SRCI_FILE_NAME, "r");
    if (!verify_sourcef(&dataf_table_size, fptrf) || !verify_source(&datai_table_size, fptri))
    {
        if (fptrf) fclose(fptrf);
        if (fptri) fclose(fptri);
        return -1;
    }

    // создаём массивы соответствующего размера и считываем содержимое файлов
    float(*dataf_table)[dataf_table_size.x] = calloc(dataf_table_size.x * dataf_table_size.y, sizeof(float));
    int(*datai_table)[datai_table_size.x] = calloc(datai_table_size.x * datai_table_size.y, sizeof(int));
    if (!load_arrayf(dataf_table_size, dataf_table, fptrf) || !load_array(datai_table_size, datai_table, fptri))
    {
        if (fptrf) fclose(fptrf);
        free(dataf_table);
        if (fptri) fclose(fptri);
        free(datai_table);
        return -1;
    }
    fclose(fptrf);
    fclose(fptri);

    // выведем на экран массивы со считанными данными
    print_tablef(dataf_table_size, dataf_table);
    print_table(datai_table_size, datai_table);

    // ...

    free(dataf_table);
    free(datai_table);
    return 0;
}
