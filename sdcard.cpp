// sdcard.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "windows.h"
#include "string.h"

#include "sddef.h"

#include "sffdisk.h"


int main(int argc, char* argv[])
{
 //   std::cout << "Enter SD card Driver letter <Enter>:";

	char szTotalPath[MAX_PATH];
//	char szInpChar[10] = { 0 };

//	std::cin.get(szInpChar[0]);

	strcpy(szTotalPath, "\\\\.\\");
//	strcat(szTotalPath, szInpChar);
	strcat(szTotalPath, argv[1]);
	strcat(szTotalPath, ":");

	UCHAR ucCmd = atoi(argv[2]);

	DWORD dwBytesReturned = 0;

	HANDLE hVol;

	hVol = CreateFile(szTotalPath,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hVol == INVALID_HANDLE_VALUE) {
		int err = GetLastError();
		printf("Can not open handle for path %s \r\n", szTotalPath);
	}
	/*
	//get device number of the drive
	STORAGE_DEVICE_NUMBER vol_disk;
	bool result = DeviceIoControl(hVol, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0,
		(LPVOID)&vol_disk, (DWORD)sizeof(vol_disk),
		(LPDWORD)&dwBytesReturned, (LPOVERLAPPED)NULL);

	char dev_path[MAX_PATH] = { 0 };
	char dev_num[MAX_PATH] = { 0 };
	strcpy(dev_path, "\\\\.\\PHYSICALDRIVE");
	sprintf(dev_num, "%d", vol_disk.DeviceNumber);
	strcat(dev_path, dev_num);

	HANDLE dev_hdl = CreateFile(dev_path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	int err01 = GetLastError();
	printf("CreateFile %s err %d\r\n", dev_path, err01);

	if (dev_hdl == INVALID_HANDLE_VALUE)
	{
		printf("Search Windows Error Codes for code %d\r\n", err01);
		return 1;
	}
	*/
	int nSizeOfCmd = sizeof(SFFDISK_DEVICE_COMMAND_DATA) + sizeof(SDCMD_DESCRIPTOR) + 16;
	SFFDISK_DEVICE_COMMAND_DATA* pCmd = (SFFDISK_DEVICE_COMMAND_DATA*) new BYTE[nSizeOfCmd];
	memset(pCmd, 0, nSizeOfCmd);
	pCmd->HeaderSize = sizeof(SFFDISK_DEVICE_COMMAND_DATA);
	pCmd->Flags = 0;
	pCmd->Command = SFFDISK_DC_DEVICE_COMMAND;
	pCmd->ProtocolArgumentSize = sizeof(SDCMD_DESCRIPTOR);
	pCmd->DeviceDataBufferSize = 16;
	ULONG_PTR info = 0;
	pCmd->Information = info;

	///Command protocol
	SDCMD_DESCRIPTOR sdCmdDescriptor = { 0 };
	sdCmdDescriptor.Cmd = ucCmd; // csd 9
	sdCmdDescriptor.CmdClass = SDCC_STANDARD;
	sdCmdDescriptor.TransferDirection = SDTD_READ;
	sdCmdDescriptor.TransferType = SDTT_CMD_ONLY; 
	sdCmdDescriptor.ResponseType = SDRT_2;
	memcpy((BYTE*)(&(pCmd->Data[0])), &sdCmdDescriptor, sizeof(SDCMD_DESCRIPTOR));

	int nSizeOfRet = nSizeOfCmd;// *10;


	uint8_t* pRet = (uint8_t*) new BYTE[nSizeOfCmd];
	memset(pRet, 0, nSizeOfCmd);

//	printf("BytesSent %d\r\n", nSizeOfCmd);

//	int ret = MyVoid();

	dwBytesReturned = 0;

	int iResult = DeviceIoControl(hVol, IOCTL_SFFDISK_DEVICE_COMMAND, pCmd, nSizeOfCmd, pCmd, nSizeOfRet, &dwBytesReturned, NULL);
//	int iResult = DeviceIoControl(dev_hdl, IOCTL_SFFDISK_DEVICE_COMMAND, pCmd, nSizeOfCmd, pCmd, nSizeOfRet, &dwBytesReturned, NULL);

	int err = GetLastError();

//	printf("BytesRet %d\r\n", dwBytesReturned);

//	printf("bResult %d\r\n", iResult);

//	printf("err %d\r\n", err);
	if (err != 0) printf("Search Windows Error Codes for code %d\r\n", err);

	int i16RetOffset = sizeof(SFFDISK_DEVICE_COMMAND_DATA) + sizeof(SDCMD_DESCRIPTOR);

	uint8_t* pBytes = (BYTE*)pCmd;

/*	for (int i = 0; i < 16; i++)
	{

		uint8_t uiRetByte = pBytes[i16RetOffset + i];
		printf("pCmd_(l16) %02X\r\n", uiRetByte);

	}*/
	printf("\r\n");
	for (int i = 0; i < 16; i++)
	{
		uint8_t uiRetByte = pBytes[i16RetOffset + i];
		printf("%02X ", uiRetByte);
	}
	dwBytesReturned = 0;
	printf("\r\n");

	CloseHandle(hVol);
}

