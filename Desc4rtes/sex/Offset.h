#pragma once
#include <Windows.h>
#include <iostream>

struct offset
{
	static uint64_t AngleHook;
	static uint64_t BorderLine;
	static uint64_t FovHook;
	static uint64_t CompoenetHook;
	static uint64_t skill;
};

uint64_t offset::AngleHook = 0x6A5F6C;         // �ޱ� ��ġ �ϴ°�
uint64_t offset::BorderLine = 0xB23BDE;       // �׿� ������ ��ġ �ϴ°�
uint64_t offset::FovHook = 0x6A3B44;         // ���� (��ƼƼ) ��ġ �ϴ°�
uint64_t offset::CompoenetHook = 0x16C1629; // ������Ʈ ��ġ �ϴ°� // ���� ���� �� �����ٵ� ���� ȫ���̰� ������ä��� �ָ���� ������ �����긧�� ���� ���� 
uint64_t offset::skill = 0x1ae9100;        //�� ��ų ��ġ �ϴ°�

#define Compo__SKILL 0x2F           //�� ��ų ���� id ��ġ x
#define Compo__HEALTH 0x33         //�� ü�� ���� id ��ġ x
#define OFFSET__SKillBase 0xD0    //�� ��ų ���̽� ��ġ x
