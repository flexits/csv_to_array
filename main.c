/**
 ����㧪� ������ � �ଠ� CSV �� 䠩�� � ���ᨢ - �ਬ�� �ᯮ�짮�����.

 ��. �������ਨ � csvimport.h

 MIT License
 Copyright (c) 2023 ����ᠭ�� ����⥫�� https://github.com/flexits
**/

#include <stdio.h>
#include <stdlib.h>

#include "csvimport.h"

#define SRCF_FILE_NAME "durn.csv"   // 䠩� ⠡���� � �஡�묨 ���祭�ﬨ
#define SRCI_FILE_NAME "conn.csv"   // 䠩� ⠡���� � 楫묨 ���祭�ﬨ

int main()
{
    // ࠧ��� ⠡���
    dimensns_t dataf_table_size = {0, 0};
    dimensns_t datai_table_size = {0, 0};

    // �������� ᮤ�ন���� 䠩��� � ���᫥��� ࠧ��஢ ⠡���
    FILE *fptrf = fopen(SRCF_FILE_NAME, "r");
    FILE *fptri = fopen(SRCI_FILE_NAME, "r");
    if (!verify_sourcef(&dataf_table_size, fptrf) || !verify_source(&datai_table_size, fptri))
    {
        if (fptrf) fclose(fptrf);
        if (fptri) fclose(fptri);
        return -1;
    }

    // ᮧ��� ���ᨢ� ᮮ⢥�����饣� ࠧ��� � ���뢠�� ᮤ�ন��� 䠩���
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

    // �뢥��� �� �࠭ ���ᨢ� � ��⠭�묨 ����묨
    print_tablef(dataf_table_size, dataf_table);
    print_table(datai_table_size, datai_table);

    // ...

    free(dataf_table);
    free(datai_table);
    return 0;
}
