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

uint64_t offset::AngleHook = 0x6A5F6C;         // 앵글 패치 하는곳
uint64_t offset::BorderLine = 0xB23BDE;       // 테월 색변경 패치 하는곳
uint64_t offset::FovHook = 0x6A3B44;         // 포브 (엔티티) 패치 하는곳
uint64_t offset::CompoenetHook = 0x16C1629; // 컴포넌트 패치 하는곳 // 병신 여기 니 못쓸텐데 ㅉㅉ 홍승이가 컴포줘ㅓㅆ음 주면머함 ㅇㅇ모 ㅅ맜쯔름테 ㄴ데 ㅋㅋ 
uint64_t offset::skill = 0x1ae9100;        //적 스킬 패치 하는곳

#define Compo__SKILL 0x2F           //적 스킬 컴포 id 패치 x
#define Compo__HEALTH 0x33         //적 체력 컴포 id 패치 x
#define OFFSET__SKillBase 0xD0    //적 스킬 베이스 패치 x
