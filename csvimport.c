/**
 ����㧪� ������ � �ଠ� CSV �� 䠩�� � ���ᨢ

 ��. �������ਨ � csvimport.h

 MIT License
 Copyright (c) 2023 ����ᠭ�� ����⥫�� https://github.com/flexits
**/

#include "csvimport.h"

bool verify_sourcef(dimensns_t *table_size, FILE *fptr)
{
    if (fptr == NULL)
    {
        printf("�� 㤠���� ������ 䠩� ⠡����!\n");
        return false;
    }
    table_size->x = 0; // ����稪 �⮫�殢
    table_size->y = 0; // ����稪 ��ப
    char c;
    // �����⨬�� ������� 䠩��:
    // digit - separator - ... - newline
    // digit - separator - ... - separator - newline
    // digit - separator - ... - EOF
    // digit - separator - ... - separator - EOF
    // ��ᨬ���쭮 ���뢠� 䠩�, �஢��塞 ᮮ⢥��⢨� ��������
    // �����६���� ���६����㥬 ����稪�
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
            // ������� ࠧ����⥫� �� ����� ���� � ��砫� �祩�� � �� ����� ��������� ��᪮�쪮 ࠧ
            if (!prev_char_was_digit)
            {
                printf("����୮� ᮤ�ন��� 䠩��!\n(������� ࠧ����⥫� ��। ���祭���)\n");
                return false;
            }
            if (contains_dec_separator)
            {
                printf("����୮� ᮤ�ন��� 䠩��!\n(��᪮�쪮 �������� ࠧ����⥫�� � �祩��)\n");
                return false;
            }
            contains_dec_separator = true;
            prev_char_was_newline = false;
            break;
        case ';':
            // ࠧ����⥫� �������
            if (prev_char_was_digit)
            {
                prev_char_was_digit = false;
            }
            else
            {
                printf("����୮� ᮤ�ন��� 䠩��!\n(����� �祩��)\n");
                return false;
            }
            if (is_first_line)
            {
                (table_size->x)++; // ����� ࠧ����⥫� � ��ࢮ� ��ப� = �⮫��� ⠡����
            }
            contains_dec_separator = false;
            prev_char_was_newline = false;
            break;
        case '\n':
            if (!prev_char_was_newline)
            { // �ய�᪠� ��������� ��ॢ��� ��ப�, 㢥��稢��� ����稪
                (table_size->y)++;
            }
            if (is_first_line && prev_char_was_digit)
            {
                (table_size->x)++; // ��᫥���� �⮫��� � ��ࢮ� ��ப� �� �� ������ ࠧ����⥫��
            }
            prev_char_was_newline = true;
            prev_char_was_digit = false;
            contains_dec_separator = false;
            is_first_line = false;
            break;
        case EOF:
            if (!prev_char_was_newline)
            { // �ய�᪠� ��������� ��ॢ��� ��ப�, 㢥��稢��� ����稪
                (table_size->y)++;
            }
            if (is_first_line && prev_char_was_digit)
            {
                (table_size->x)++; // ��᫥���� �⮫��� � ��ࢮ� ��ப� �� �� ������ ࠧ����⥫��
            }
            return true;
            break;
        case '\r':
            break;
        default:
            printf("����୮� ᮤ�ন��� 䠩��!\n(�������⨬� ᨬ����)\n");
            return false;
        }
    }
    // �஢��塞 ᮮ⢥��⢨� ࠧ��஢ ���ᨢ� ��⠭������� ��࠭�祭��
    if (table_size->x < X_SIZE_MIN || table_size->x > X_SIZE_MAX || table_size->y < Y_SIZE_MIN || table_size->y > Y_SIZE_MAX)
    {
        printf("������ ⠡���� �� �।����� �����⨬��� ���������!\n");
        return false;
    }
    return true;
}

bool load_arrayf(const dimensns_t table_size, float table[table_size.y][table_size.x], FILE *fptr)
{
    if (fptr == NULL)
    {
        printf("�訡�� ������ 䠩��!\n");
        return false;
    }
    rewind(fptr);

    // ��ᨬ���쭮 ���뢠�� 䠩�, ������ ���� �८�ࠧ��뢠�� � float � �����뢠�� � ��।��� ����� ���ᨢ�
    float *element = &table[0][0];                        // 㪠��⥫� �� ��।��� ����� ���ᨢ�
    int limiter = table_size.x * table_size.y;            // ���� ����� ���ᨢ�
    char *strtemp = calloc(MAX_DIGITS + 1, sizeof(char)); // ��ப���� �।�⠢����� �祩�� ⠡����
    int strtemp_index = 0;                                // ������ ⥪�饣� ᨬ���� � ��ப���� �।�⢫����
    char c = EOF;                                         // ⥪�騩 ��⠭�� ᨬ���
    char dpoint = *(localeconv()->decimal_point);         // ࠧ����⥫�, ᮮ⢥�����騩 ������
    do
    {
        switch (c = fgetc(fptr))
        {
        case ',':
        case '.':
            c = dpoint; // ������� ࠧ����⥫� �� ᮮ⢥�����騩 ������
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
            // ������� ⥪�騩 ᨬ��� � ��ப���� �।�⠢�����
            if (strtemp_index < MAX_DIGITS)
            {
                strtemp[strtemp_index++] = c;
            }
            break;
        case ';':
        case '\n':
        case EOF:
            // �祩�� �����稫���, �८�ࠧ��뢠�� ��ப� � �᫮
            if (strtemp_index > 0)
            {
                *element = atof(strtemp);
                element++;
                limiter--;
                // ����塞 ��ப�
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
        printf("�訡�� �८�ࠧ������ ���祭�� �祥�!\n"); // 䠩� �����稫�� ࠭��, 祬 �뫨 ��⠭� �� ����� = �訡�� ᮤ�ন����
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
        printf("�� 㤠���� ������ 䠩� ⠡����!\n");
        return false;
    }
    table_size->x = 0; // ����稪 �⮫�殢
    table_size->y = 0; // ����稪 ��ப
    char c;
    // �����⨬�� ������� 䠩��:
    // digit - separator - ... - newline
    // digit - separator - ... - separator - newline
    // digit - separator - ... - EOF
    // digit - separator - ... - separator - EOF
    // ��ᨬ���쭮 ���뢠� 䠩�, �஢��塞 ᮮ⢥��⢨� ��������
    // �����६���� ���६����㥬 ����稪�
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
            // ࠧ����⥫� �������
            if (prev_char_was_digit)
            {
                prev_char_was_digit = false;
            }
            else
            {
                printf("����୮� ᮤ�ন��� 䠩��!\n(����� �祩��)\n");
                return false;
            }
            if (is_first_line)
            {
                (table_size->x)++; // ����� ࠧ����⥫� � ��ࢮ� ��ப� = �⮫��� ⠡����
            }
            prev_char_was_newline = false;
            break;
        case '\n':
            if (!prev_char_was_newline)
            { // �ய�᪠� ��������� ��ॢ��� ��ப�, 㢥��稢��� ����稪
                (table_size->y)++;
            }
            if (is_first_line && prev_char_was_digit)
            {
                (table_size->x)++; // ��᫥���� �⮫��� � ��ࢮ� ��ப� �� �� ������ ࠧ����⥫��
            }
            prev_char_was_newline = true;
            prev_char_was_digit = false;
            is_first_line = false;
            break;
        case EOF:
            if (!prev_char_was_newline)
            { // �ய�᪠� ��������� ��ॢ��� ��ப�, 㢥��稢��� ����稪
                (table_size->y)++;
            }
            if (is_first_line && prev_char_was_digit)
            {
                (table_size->x)++; // ��᫥���� �⮫��� � ��ࢮ� ��ப� �� �� ������ ࠧ����⥫��
            }
            return true;
            break;
        case '\r':
            break;
        default:
            printf("����୮� ᮤ�ন��� 䠩��!\n(�������⨬� ᨬ����)\n");
            return false;
        }
    }
    // �஢��塞 ᮮ⢥��⢨� ࠧ��஢ ���ᨢ� ��⠭������� ��࠭�祭��
    if (table_size->x < X_SIZE_MIN || table_size->x > X_SIZE_MAX || table_size->y < Y_SIZE_MIN || table_size->y > Y_SIZE_MAX)
    {
        printf("������ ⠡���� �� �।����� �����⨬��� ���������!\n");
        return false;
    }
    return true;
}

bool load_array(const dimensns_t table_size, int table[table_size.y][table_size.x], FILE *fptr)
{
    if (fptr == NULL)
    {
        printf("�訡�� ������ 䠩��!\n");
        return false;
    }
    rewind(fptr);

    // ��ᨬ���쭮 ���뢠�� 䠩�, ������ ���� �८�ࠧ��뢠�� � float � �����뢠�� � ��।��� ����� ���ᨢ�
    int *element = &table[0][0];                          // 㪠��⥫� �� ��।��� ����� ���ᨢ�
    int limiter = table_size.x * table_size.y;            // ���� ����� ���ᨢ�
    char *strtemp = calloc(MAX_DIGITS + 1, sizeof(char)); // ��ப���� �।�⠢����� �祩�� ⠡����
    int strtemp_index = 0;                                // ������ ⥪�饣� ᨬ���� � ��ப���� �।�⢫����
    char c = EOF;                                         // ⥪�騩 ��⠭�� ᨬ���
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
            // ������� ⥪�騩 ᨬ��� � ��ப���� �।�⠢�����
            if (strtemp_index < MAX_DIGITS)
            {
                strtemp[strtemp_index++] = c;
            }
            break;
        case ';':
        case ',':
        case '\n':
        case EOF:
            // �祩�� �����稫���, �८�ࠧ��뢠�� ��ப� � �᫮
            if (strtemp_index > 0)
            {
                *element = atoi(strtemp);
                element++;
                limiter--;
                // ����塞 ��ப�
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
        printf("�訡�� �८�ࠧ������ ���祭�� �祥�!\n"); // 䠩� �����稫�� ࠭��, 祬 �뫨 ��⠭� �� ����� = �訡�� ᮤ�ন����
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
