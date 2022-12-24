// sdcard.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "windows.h"
#include "string.h"

#include "sddef.h"

#include "sffdisk.h"


int main(int argc, char* argv[])
{
	char szTotalPath[MAX_PATH];

	strcpy(szTotalPath, "\\\\.\\");
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

	dwBytesReturned = 0;

	int iResult = DeviceIoControl(hVol, IOCTL_SFFDISK_DEVICE_COMMAND, pCmd, nSizeOfCmd, pCmd, nSizeOfRet, &dwBytesReturned, NULL);

	int err = GetLastError();

	if (err != 0) printf("Search Windows Error Codes for code %d\r\n", err);

	int i16RetOffset = sizeof(SFFDISK_DEVICE_COMMAND_DATA) + sizeof(SDCMD_DESCRIPTOR);

	uint8_t* pBytes = (BYTE*)pCmd;

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

