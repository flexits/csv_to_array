/**
 Загрузка данных в формате CSV из файла в массив

 См. комментарии в csvimport.h

 MIT License
 Copyright (c) 2023 Александр Коростелин https://github.com/flexits
**/

#include "csvimport.h"

bool verify_sourcef(dimensns_t *table_size, FILE *fptr)
{
    if (fptr == NULL)
    {
        printf("Не удалось открыть файл таблицы!\n");
        return false;
    }
    table_size->x = 0; // счётчик столбцов
    table_size->y = 0; // счётчик строк
    char c;
    // допустимая структура файла:
    // digit - separator - ... - newline
    // digit - separator - ... - separator - newline
    // digit - separator - ... - EOF
    // digit - separator - ... - separator - EOF
    // посимвольно считывая файл, проверяем соответствие структуры
    // одновременно инкрементируем счётчики
    bool is_first_line = true;
    bool prev_char_was_digit = false;
    bool prev_char_was_newline = false;
    bool contains_dec_separator = false;
    for (;;)
    {
        c = fgetc(fptr);
        switch (c)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            prev_char_was_digit = true;
            prev_char_was_newline = false;
            break;
        case ',':
        case '.':
            // десятичный разделитель не может быть в начале ячейки и не может встречаться несколько раз
            if (!prev_char_was_digit)
            {
                printf("Неверное содержимое файла!\n(десятичный разделитель перед значением)\n");
                return false;
            }
            if (contains_dec_separator)
            {
                printf("Неверное содержимое файла!\n(несколько десятичных разделителей в ячейке)\n");
                return false;
            }
            contains_dec_separator = true;
            prev_char_was_newline = false;
            break;
        case ';':
            // разделитель колонок
            if (prev_char_was_digit)
            {
                prev_char_was_digit = false;
            }
            else
            {
                printf("Неверное содержимое файла!\n(пустая ячейка)\n");
                return false;
            }
            if (is_first_line)
            {
                (table_size->x)++; // каждый разделитель в первой строке = столбец таблицы
            }
            contains_dec_separator = false;
            prev_char_was_newline = false;
            break;
        case '\n':
            if (!prev_char_was_newline)
            { // пропуская многократные переводы строки, увеличиваем счётчик
                (table_size->y)++;
            }
            if (is_first_line && prev_char_was_digit)
            {
                (table_size->x)++; // последний столбец в первой строке не был закрыт разделителем
            }
            prev_char_was_newline = true;
            prev_char_was_digit = false;
            contains_dec_separator = false;
            is_first_line = false;
            break;
        case EOF:
            if (!prev_char_was_newline)
            { // пропуская многократные переводы строки, увеличиваем счётчик
                (table_size->y)++;
            }
            if (is_first_line && prev_char_was_digit)
            {
                (table_size->x)++; // последний столбец в первой строке не был закрыт разделителем
            }
            return true;
            break;
        case '\r':
            break;
        default:
            printf("Неверное содержимое файла!\n(недопустимые символы)\n");
            return false;
        }
    }
    // проверяем соответствие размеров массива установленным ограничениям
    if (table_size->x < X_SIZE_MIN || table_size->x > X_SIZE_MAX || table_size->y < Y_SIZE_MIN || table_size->y > Y_SIZE_MAX)
    {
        printf("Размер таблицы за пределами допустимого диапазона!\n");
        return false;
    }
    return true;
}

bool load_arrayf(const dimensns_t table_size, float table[table_size.y][table_size.x], FILE *fptr)
{
    if (fptr == NULL)
    {
        printf("Ошибка открытия файла!\n");
        return false;
    }
    rewind(fptr);

    // посимвольно считываем файл, каждую цифру преобразовываем в float и записываем в очередной элемент массива
    float *element = &table[0][0];                        // указатель на очередной элемент массива
    int limiter = table_size.x * table_size.y;            // общая длина массива
    char *strtemp = calloc(MAX_DIGITS + 1, sizeof(char)); // строковое представление ячейки таблицы
    int strtemp_index = 0;                                // индекс текущего символа в строковом предствлении
    char c = EOF;                                         // текущий считанный символ
    char dpoint = *(localeconv()->decimal_point);         // разделитель, соответствующий локали
    do
    {
        switch (c = fgetc(fptr))
        {
        case ',':
        case '.':
            c = dpoint; // заменим разделитель на соответствующий локали
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            // добавим текущий символ в строковое представление
            if (strtemp_index < MAX_DIGITS)
            {
                strtemp[strtemp_index++] = c;
            }
            break;
        case ';':
        case '\n':
        case EOF:
            // ячейка закончилась, преобразовываем строку в число
            if (strtemp_index > 0)
            {
                *element = atof(strtemp);
                element++;
                limiter--;
                // обнуляем строку
                while (strtemp_index > 0)
                {
                    strtemp[strtemp_index--] = '\0';
                }
            }
            break;
        }
    } while (c != EOF && limiter > 0);
    free(strtemp);

    if (limiter > 0)
    {
        printf("Ошибка преобразования значений ячеек!\n"); // файл закончился раньше, чем были считаны все данные = ошибка содержимого
        return false;
    }

    return true;
}

void print_tablef(const dimensns_t table_size, const float table[table_size.y][table_size.x])
{
    printf("\n");
    for (int i = 0; i < table_size.y; i++)
    {
        for (int j = 0; j < table_size.x; j++)
        {
            printf("%2.2f ", table[i][j]);
        }
        printf("\n");
    }
}

bool verify_source(dimensns_t *table_size, FILE *fptr)
{
    if (fptr == NULL)
    {
        printf("Не удалось открыть файл таблицы!\n");
        return false;
    }
    table_size->x = 0; // счётчик столбцов
    table_size->y = 0; // счётчик строк
    char c;
    // допустимая структура файла:
    // digit - separator - ... - newline
    // digit - separator - ... - separator - newline
    // digit - separator - ... - EOF
    // digit - separator - ... - separator - EOF
    // посимвольно считывая файл, проверяем соответствие структуры
    // одновременно инкрементируем счётчики
    bool is_first_line = true;
    bool prev_char_was_digit = false;
    bool prev_char_was_newline = false;
    for (;;)
    {
        c = fgetc(fptr);
        switch (c)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            prev_char_was_digit = true;
            prev_char_was_newline = false;
            break;
        case ',':
        case ';':
            // разделитель колонок
            if (prev_char_was_digit)
            {
                prev_char_was_digit = false;
            }
            else
            {
                printf("Неверное содержимое файла!\n(пустая ячейка)\n");
                return false;
            }
            if (is_first_line)
            {
                (table_size->x)++; // каждый разделитель в первой строке = столбец таблицы
            }
            prev_char_was_newline = false;
            break;
        case '\n':
            if (!prev_char_was_newline)
            { // пропуская многократные переводы строки, увеличиваем счётчик
                (table_size->y)++;
            }
            if (is_first_line && prev_char_was_digit)
            {
                (table_size->x)++; // последний столбец в первой строке не был закрыт разделителем
            }
            prev_char_was_newline = true;
            prev_char_was_digit = false;
            is_first_line = false;
            break;
        case EOF:
            if (!prev_char_was_newline)
            { // пропуская многократные переводы строки, увеличиваем счётчик
                (table_size->y)++;
            }
            if (is_first_line && prev_char_was_digit)
            {
                (table_size->x)++; // последний столбец в первой строке не был закрыт разделителем
            }
            return true;
            break;
        case '\r':
            break;
        default:
            printf("Неверное содержимое файла!\n(недопустимые символы)\n");
            return false;
        }
    }
    // проверяем соответствие размеров массива установленным ограничениям
    if (table_size->x < X_SIZE_MIN || table_size->x > X_SIZE_MAX || table_size->y < Y_SIZE_MIN || table_size->y > Y_SIZE_MAX)
    {
        printf("Размер таблицы за пределами допустимого диапазона!\n");
        return false;
    }
    return true;
}

bool load_array(const dimensns_t table_size, int table[table_size.y][table_size.x], FILE *fptr)
{
    if (fptr == NULL)
    {
        printf("Ошибка открытия файла!\n");
        return false;
    }
    rewind(fptr);

    // посимвольно считываем файл, каждую цифру преобразовываем в float и записываем в очередной элемент массива
    int *element = &table[0][0];                          // указатель на очередной элемент массива
    int limiter = table_size.x * table_size.y;            // общая длина массива
    char *strtemp = calloc(MAX_DIGITS + 1, sizeof(char)); // строковое представление ячейки таблицы
    int strtemp_index = 0;                                // индекс текущего символа в строковом предствлении
    char c = EOF;                                         // текущий считанный символ
    do
    {
        switch (c = fgetc(fptr))
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            // добавим текущий символ в строковое представление
            if (strtemp_index < MAX_DIGITS)
            {
                strtemp[strtemp_index++] = c;
            }
            break;
        case ';':
        case ',':
        case '\n':
        case EOF:
            // ячейка закончилась, преобразовываем строку в число
            if (strtemp_index > 0)
            {
                *element = atoi(strtemp);
                element++;
                limiter--;
                // обнуляем строку
                while (strtemp_index > 0)
                {
                    strtemp[strtemp_index--] = '\0';
                }
            }
            break;
        }
    } while (c != EOF && limiter > 0);
    free(strtemp);

    if (limiter > 0)
    {
        printf("Ошибка преобразования значений ячеек!\n"); // файл закончился раньше, чем были считаны все данные = ошибка содержимого
        return false;
    }

    return true;
}

void print_table(const dimensns_t table_size, const int table[table_size.y][table_size.x])
{
    printf("\n");
    for (int i = 0; i < table_size.y; i++)
    {
        for (int j = 0; j < table_size.x; j++)
        {
            printf("%3d ", table[i][j]);
        }
        printf("\n");
    }
}
