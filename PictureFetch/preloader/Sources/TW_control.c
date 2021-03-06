#include "TW_control.h"

const unsigned char	Tw8835_Innolux_Pannal_Init_Data[]=
{
	0xFF, 0x00,//; Page 0
	0x06, 0x00,0x07, 0x02,0x08, 0x89,0x1F, 0x00,0x40, 0x10,0x41, 0x0C,0x42, 0x02,0x43, 0x01,0x44, 0xEB,0x45, 0x76,
	0x46, 0xD9,0x47, 0x80,0x48, 0x00,0x50, 0x00,0x51, 0x00,0x52, 0x00,0x53, 0x03,0x54, 0x00,0x56, 0x00,0x57, 0x00,
	0x5F, 0x00,0x80, 0x00,0x81, 0x00,0x82, 0x00,0x83, 0x00,0x84, 0x0C,0x85, 0x00,0x86, 0x00,0x88, 0x00,0x89, 0x00,
	0x8A, 0x00,0x8B, 0x00,0x8C, 0x0C,0x8D, 0x00,0x8E, 0x00,0x90, 0x00,0x91, 0x00,0x92, 0x00,0x93, 0x00,0x94, 0x0C,
	0x95, 0x00,0x96, 0x00,0x98, 0x60,0x99, 0x00,0x9A, 0x00,0x9B, 0x00,0x9C, 0x0C,0x9D, 0x00,0x9E, 0x00,0xD4, 0x00,
	0xE0, 0x71,0xE1, 0x77,0xE2, 0x04,0xE3, 0x40,0xE4, 0x84,0xE5, 0x80,0xE6, 0x20,0xE8, 0x71,0xE9, 0x0A,0xEA, 0x0F,
	0xEB, 0x40,0xEC, 0x20,0xED, 0x40,0xEE, 0x20,0xF6, 0x00,0xF7, 0x16,0xF8, 0x01,0xF9, 0x86,0xFA, 0xA0,0xFB, 0x40,
	0xFC, 0x23,0xFD, 0x34,
	
	0xFF, 0x01,  //; Page 1
	0x01, 0x83,0x02, 0x4C,0x04, 0x00,0x05, 0x29,0x06, 0x03,0x07, 0x02,0x08, 0x14,0x09, 0xF0,0x0A, 0x0F,0x0B, 0xD9,
	0x0C, 0xCC,0x0D, 0x00,0x10, 0xA0,0x11, 0x80,0x12, 0x11,0x13, 0x80,0x14, 0x80,0x15, 0x00,0x17, 0x30,0x18, 0x44,
	0x1C, 0x2F,0x1D, 0x7F,0x1E, 0x00,0x20, 0x50,0x21, 0x22,0x22, 0xF0,0x23, 0xD8,0x24, 0xBC,0x25, 0xB8,0x26, 0x44,
	0x27, 0x38,0x28, 0x00,0x29, 0xE0,0x2A, 0x78,0x2B, 0x44,0x2C, 0x30,0x2D, 0x14,0x2E, 0xA5,0x2F, 0xE4,0x33, 0x85, //0X29 00改E0
	0x34, 0x1A,0x35, 0x00,0xC0, 0x01,0xC2, 0x01,0xC3, 0x03,0xC4, 0x5A,0xC5, 0x00,0xC6, 0x20,0xC7, 0x04,0xC8, 0x00,
	0xC9, 0x06,0xCA, 0x06,0xCB, 0x10,0xCC, 0x00,0xCD, 0x54,0xD0, 0x00,0xD1, 0xF0,0xD2, 0xF0,0xD3, 0xF0,0xD4, 0x00,
	0xD5, 0x00,0xD6, 0x10,0xD7, 0x70,0xD8, 0x00,0xD9, 0x04,0xDA, 0x80,0xDB, 0x80,0xDC, 0x20,0xE0, 0x00,0xE1, 0x05,
	0xE2, 0xD9,0xE3, 0x07,0xE4, 0x33,0xE5, 0x31,0xE6, 0x00,0xE7, 0x2A,
	
	0xFF, 0x02,  //; Page 2
	0x01, 0x00,0x02, 0x14,0x03, 0x80,0x04, 0x1B,0x05, 0xA5,0x06, 0x0F,0x07, 0x80,0x08, 0x12,0x09, 0x06,0x0A, 0x14,
	0x0B, 0x12,0x0C, 0xC7,0x0D, 0x91,0x0E, 0x20,0x0F, 0x53,0x10, 0x14,0x11, 0x22,0x12, 0x23,0x13, 0x0D,0x14, 0x09,
	0x15, 0x28,0x16, 0xF0,0x17, 0x01,0x18, 0x10,0x19, 0x22,0x1A, 0x05,0x1B, 0x02,0x1C, 0x50,0x1D, 0x27,0x1E, 0x03,
	0x40, 0x10,0x41, 0x00,0x42, 0x05,0x43, 0x01,0x44, 0x64,0x45, 0xF4,0x46, 0x00,0x47, 0x0A,0x48, 0x36,0x49, 0x10,
	0x4A, 0x00,0x4B, 0x00,0x4C, 0x00,0x4D, 0x44,0x4E, 0x04,0xE0, 0x00,0xE4, 0x21,0xF8, 0x00,0xF9, 0x80,
	
	0xFF, 0x03,  //; Page 3
	0x00, 0x00,0x01, 0x06,0x02, 0x06,0x03, 0x00,0x04, 0x8C,0x05, 0x00,0x06, 0x00,0x07, 0x00,0x08, 0x0F,0x09, 0x00,
	0x0A, 0x00,0x0B, 0x60,0x0C, 0x40,0x0D, 0x00,0x0E, 0x00,0x10, 0x40,0x11, 0x00,0x12, 0x00,0x13, 0x96,0x14, 0x00,
	0x15, 0x01,0x16, 0x28,0x17, 0x00,0x18, 0x00,0x19, 0x00,0x1A, 0x00,0x1B, 0x00,0x1C, 0x00,0x1D, 0x00,0x1E, 0x00,
	0x1F, 0x00,0x20, 0x00,0x21, 0x00,0x22, 0x00,0x23, 0x96,0x24, 0x64,0x25, 0x01,0x26, 0x28,0x27, 0x01,0x28, 0x00,
	0x29, 0x00,0x2A, 0x00,0x2B, 0x00,0x2C, 0x00,0x2D, 0x00,0x2E, 0x00,0x2F, 0x28,0x30, 0x00,0x31, 0x00,0x32, 0x00,
	0x33, 0x96,0x34, 0xC8,0x35, 0x01,0x36, 0x28,0x37, 0x00,0x38, 0x00,0x39, 0x00,0x3A, 0x00,0x3B, 0x00,0x3C, 0x00,
	0x3D, 0x00,0x3E, 0x00,0x3F, 0x50,0x40, 0x00,0x41, 0x00,0x42, 0x01,0x43, 0x96,0x44, 0x2C,0x45, 0x01,0x46, 0x28,
	0x47, 0x01,0x48, 0x00,0x49, 0x00,0x4A, 0x00,0x4B, 0x00,0x4C, 0x00,0x4D, 0x00,0x4E, 0x00,0x4F, 0x78,0x50, 0x0D,
	0x51, 0x34,0x52, 0x01,0x53, 0x6C,0x54, 0x90,0xff, 0x04,0x0e, 0x00,0x0f, 0x34,

	//0xFF, 0x04, //; Page 4
	//0x00, 0x04
};

const BYTE	FontOsdWinBase[4] = { 0x10, 0x20, 0x30, 0x40};


void	Tw_Pannal_Parameter_Init(void)
{
	unsigned int i;

	for(i=0;i<sizeof(Tw8835_Innolux_Pannal_Init_Data)/2;i++)
	{
		WriteTW88(Tw8835_Innolux_Pannal_Init_Data[i*2],Tw8835_Innolux_Pannal_Init_Data[i*2+1]);
		asm nop;
		asm nop;
	}
}

void	BackLightCmd(BYTE Cmd)
{
	WriteTW88Page(0x00);	
	if(Cmd==ON)
	{
		WriteTW88(0x84,0x1c);
		WriteTW88(0x8c,0x1c);

		WriteTW88(0x94,0x1c);
	}
	else if(Cmd==OFF)
	{
		WriteTW88(0x84,0x0c);
		WriteTW88(0x8c,0x0c);

		WriteTW88(0x94,0x0c);
	}
}


