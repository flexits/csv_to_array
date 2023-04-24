 /**
 ����㧪� �室��� ������ � �ଠ� CSV �� 䠩�� � ���ᨢ.

 ������ ��ப� 䠩�� �।�⠢��� ᮡ�� ��ப� ⠡����.
 ���� �� ������ ᮤ�ঠ�� ��ப� ���������.
 ���� ����� ᮤ�ঠ�� ����� ��ப�, �஬� ��ࢮ�.

 �����⨬� ࠧ����⥫� �����: ������ � �窠 � ����⮩ ���
 ⠡��� � 楫��᫥��묨 ���祭�ﬨ; ⮫쪮 �窠 � ����⮩
 ��� ⠡��� � �஡�묨 �᫠��.
 �����⨬� ������� ࠧ����⥫�: ������ � �窠 (⮫쪮 ���
 ⠡��� � �஡�묨 ���祭�ﬨ).
 � ����� ⠡��� ����᪠���� ᬥ襭�� ࠧ����⥫��.

 ������ ��ப� ������ ᮤ�ঠ�� ���������� ������⢮ �����;
 ����� ���� �� ����᪠����.

 ����᪠���� ࠧ��筮� ������⢮ ��ப � �⮫�殢.

 �ਬ�� ᮤ�ন���� 䠩�� (� �������� ᪮���� ������� �㦥��� ᨬ����):
 2;0;0;1;[��ॢ�� ��ப�]
 0;0;0;1;[��ॢ�� ��ப�]
 1;1;0;1;[��ॢ�� ��ப�]
 3;0;0;1;

 ����᪠���� ������⢨� ࠧ����⥫� ��᫥ ��᫥����� ���� � ��ப�.

 ����窨, ��� � �����⨬� � 䠩��� CSV � ��饬 ��砥,
 ������������ �ணࠬ��� ��� �������⨬�� ᮤ�ন���.

 ���஡���� � �ଠ� CSV ����� ������� � �⠭���� RFC 4180.
 https://www.rfc-editor.org/rfc/rfc4180.txt

 MIT License
 Copyright (c) 2023 ����ᠭ�� ����⥫�� https://github.com/flexits
 **/

#ifndef CSVIMPORT_H_INCLUDED
#define CSVIMPORT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

/*
 * ��࠭�祭�� �室��� 䠩���
 */
#define MAX_DIGITS 5  // ���ᨬ��쭮� �᫮ ᨬ����� � �祩��
#define X_SIZE_MIN 3  // �������쭮� �᫮ �⮫�殢
#define X_SIZE_MAX 12 // ���ᨬ��쭮� �᫮ �⮫�殢
#define Y_SIZE_MIN 3  // �������쭮� �᫮ ��ப
#define Y_SIZE_MAX 12 // ���ᨬ��쭮� �᫮ ��ப

/*
 * ������ ⠡����: x - �᫮ �⮫�殢; y - �᫮ ��ப
 */
typedef struct Dimensions
{
    int x;
    int y;
} dimensns_t;

/*
 * �஢���� ���४⭮��� CSV-ᮤ�ন���� 䠩�� � ������ ࠧ���� ⠡����.
 * �����頥� false � ��砥 �訡��.
 *
 * verify_source - ��� ⠡��� � 楫��᫥��묨 ���祭�ﬨ,
 * verify_sourcef - ��� ⠡��� � �஡�묨 ���祭�ﬨ.
 */
bool verify_source(dimensns_t *table_size, FILE *fptr);
bool verify_sourcef(dimensns_t *table_size, FILE *fptr);

/*
 * ���뢠�� �祩�� ⠡���� �� 䠩�� � ���ᨢ, �८�ࠧ��뢠� � �᫥��� �ଠ�.
 * �����頥� false � ��砥 �訡��.
 *
 * load_array - ��� ⠡��� � 楫��᫥��묨 ���祭�ﬨ,
 * load_arrayf - ��� ⠡��� � �஡�묨 ���祭�ﬨ.
 */
bool load_array(const dimensns_t table_size, int table[table_size.y][table_size.x], FILE *fptr);
bool load_arrayf(const dimensns_t table_size, float table[table_size.y][table_size.x], FILE *fptr);

/*
 * �뢮��� ���ᨢ �� �࠭ (�� ����室����� ᫥��� �������� format string � ����).
 *
 * print_table - ��� ⠡��� � 楫��᫥��묨 ���祭�ﬨ,
 * print_tablef - ��� ⠡��� � �஡�묨 ���祭�ﬨ.
 */
void print_table(const dimensns_t table_size, const int table[table_size.y][table_size.x]);
void print_tablef(const dimensns_t table_size, const float table[table_size.y][table_size.x]);

#endif // CSVIMPORT_H_INCLUDED
