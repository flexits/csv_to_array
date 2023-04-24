 /**
 Загрузка входных данных в формате CSV из файла в массив.

 Каждая строка файла представляет собой строку таблицы.
 Файл не должен содержать строку заголовка.
 Файл может содержать пустые строки, кроме первой.

 Допустимые разделители полей: запятая и точка с запятой для
 таблиц с целочисленными значениями; только точка с запятой
 для таблиц с дробными числами.
 Допустимые десятичные разделители: запятая и точка (только для
 таблиц с дробными значениями).
 В одной таблице допускается смешение разделителей.

 Каждая строка должна содержать одинаковое количество полей;
 пустые поля не допускаются.

 Допускается различное количество строк и столбцов.

 Пример содержимого файла (в квадратных скобках непечатные служебные символы):
 2;0;0;1;[перевод строки]
 0;0;0;1;[перевод строки]
 1;1;0;1;[перевод строки]
 3;0;0;1;

 Допускается отсутствие разделителя после последнего поля в строке.

 Кавычки, хотя и допустимы в файлах CSV в общем случае,
 интерпретируются программой как недопустимое содержимое.

 Подробности о формате CSV можно получить в стандарте RFC 4180.
 https://www.rfc-editor.org/rfc/rfc4180.txt

 MIT License
 Copyright (c) 2023 Александр Коростелин https://github.com/flexits
 **/

#ifndef CSVIMPORT_H_INCLUDED
#define CSVIMPORT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

/*
 * Ограничения входных файлов
 */
#define MAX_DIGITS 5  // максимальное число символов в ячейке
#define X_SIZE_MIN 3  // минимальное число столбцов
#define X_SIZE_MAX 12 // максимальное число столбцов
#define Y_SIZE_MIN 3  // минимальное число строк
#define Y_SIZE_MAX 12 // максимальное число строк

/*
 * Размер таблицы: x - число столбцов; y - число строк
 */
typedef struct Dimensions
{
    int x;
    int y;
} dimensns_t;

/*
 * Проверяет корректность CSV-содержимого файла и вычисляет размеры таблицы.
 * Возвращает false в случае ошибки.
 *
 * verify_source - для таблиц с целочисленными значениями,
 * verify_sourcef - для таблиц с дробными значениями.
 */
bool verify_source(dimensns_t *table_size, FILE *fptr);
bool verify_sourcef(dimensns_t *table_size, FILE *fptr);

/*
 * Считывает ячейки таблицы из файла в массив, преобразовывая в численный формат.
 * Возвращает false в случае ошибки.
 *
 * load_array - для таблиц с целочисленными значениями,
 * load_arrayf - для таблиц с дробными значениями.
 */
bool load_array(const dimensns_t table_size, int table[table_size.y][table_size.x], FILE *fptr);
bool load_arrayf(const dimensns_t table_size, float table[table_size.y][table_size.x], FILE *fptr);

/*
 * Выводит массив на экран (при необходимости следует изменить format string в коде).
 *
 * print_table - для таблиц с целочисленными значениями,
 * print_tablef - для таблиц с дробными значениями.
 */
void print_table(const dimensns_t table_size, const int table[table_size.y][table_size.x]);
void print_tablef(const dimensns_t table_size, const float table[table_size.y][table_size.x]);

#endif // CSVIMPORT_H_INCLUDED
