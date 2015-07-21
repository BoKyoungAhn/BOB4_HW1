#pragma warning(disable:4996)
#include <Windows.h>
#include <crtdbg.h>
#include<FileAPI.h>
#include <memory>
#include <WinBase.h>
#include <conio.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include<iostream>
#include <locale.h>
using namespace std;

void print(_In_ const char* fmt, _In_ ...)
{
	char log_buffer[2048];
	va_list args;

	va_start(args, fmt);
	HRESULT hRes = StringCbVPrintfA(log_buffer, sizeof(log_buffer), fmt, args);
	if (S_OK != hRes)
	{
		fprintf(
			stderr,
			"%s, StringCbVPrintfA() failed. res = 0x%08x",
			__FUNCTION__,
			hRes
			);
		return;
	}

	OutputDebugStringA(log_buffer);
	fprintf(stdout, "%s \n", log_buffer);
}

bool is_file_existsW(_In_ const wchar_t* file_path)
{
	_ASSERTE(NULL != file_path);
	_ASSERTE(TRUE != IsBadStringPtrW(file_path, MAX_PATH));
	if ((NULL == file_path) || (TRUE == IsBadStringPtrW(file_path, MAX_PATH))) return false;

	WIN32_FILE_ATTRIBUTE_DATA info = { 0 };

	if (GetFileAttributesExW(file_path, GetFileExInfoStandard, &info) == 0)
		return false;
	else
		return true;
}
bool create_bob_txt()
{
	//char *locale = setlocale(LC_ALL, "en_US.UTF-8");
	// current directory 를 구한다.
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR)* buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	// current dir \\ bob.txt 파일명 생성
	wchar_t file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}
	free(buf); buf = NULL;

	if (true == is_file_existsW(file_name))
	{
		::DeleteFileW(file_name);
	}

	// 파일 생성
	HANDLE file_handle = CreateFileW(
		file_name,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		print("err, CreateFile(path=%ws), gle=0x%08x", file_name, GetLastError());
		return false;
	}


	CloseHandle(file_handle);
	TCHAR * sfile = TEXT("E:\\BOB_노용환 멘토님\\과제\\homework\\homework\\bob.txt");
	DWORD dwBytesWritten;
	SetErrorMode(SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS);
	HANDLE hFile = CreateFile(sfile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	TCHAR * swork = TEXT("hahahahahehehehe 안녕하세요 까꿍 Hello world\n");
	size_t llen = _tcslen(swork);

	unsigned char smarker[3];
	smarker[0] = 0xEF;
	smarker[1] = 0xBB;
	smarker[2] = 0xBF;
	WriteFile(hFile, smarker, 3, &dwBytesWritten, NULL);

	char *utf8 = (char *)malloc(1000);
	int lBytesWritten = WideCharToMultiByte(CP_UTF8, 0, swork, -1, utf8, 1000, NULL, NULL);
	int err = GetLastError();
	WriteFile(hFile, utf8, lBytesWritten, &dwBytesWritten, NULL);
	free(utf8);
	SetEndOfFile(hFile);
	CloseHandle(hFile);

	return true;
}
bool delete_bob_txt()
{
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}
	buf = (PWSTR)malloc(sizeof(WCHAR)* buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());

		free(buf);
		return false;
	}
	wchar_t file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}
	free(buf); buf = NULL;
	DeleteFileW(file_name);
	return true;

}
int copy_bob_txt() {
	FILE *in, *out;
	char* buf;
	size_t len;
	const char* src = "bob.txt";
	const char* dst = "bob2.txt";

	if ((in = fopen(src, "rb")) == NULL) return 1;
	if ((out = fopen(dst, "wb")) == NULL) { fclose(in); return 2; }

	if ((buf = (char *)malloc(1024)) == NULL) { fclose(in); fclose(out); return 3; }

	while ((len = fread(buf, sizeof(char), sizeof(buf), in)) != NULL)
	if (fwrite(buf, sizeof(char), len, out) == 0) {
		fclose(in); fclose(out);
		free(buf);
		_unlink(dst); // 에러난 파일 지우고 종료
		return 4;
	}
	fclose(in); fclose(out);
	free(buf); // 메모리 할당 해제

	return 0;

}

void readFilewithReadFile() {
	
	TCHAR * sfile = TEXT("E:\\BOB_노용환 멘토님\\과제\\homework\\homework\\bob2.txt");
	char answer[1024] = "";
	char *buf_real;
	
	HANDLE file_handle2 = CreateFile(sfile, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD fileSize = GetFileSize(file_handle2, NULL);
	wchar_t buffer[1024];
	
	
	HANDLE file_hhhhhandle = CreateFileMapping(file_handle2, NULL, PAGE_READONLY, 0, 0, NULL);
	LPVOID lpvoidFile = MapViewOfFile(file_hhhhhandle, FILE_MAP_READ, 0, 0, 0);
	
	memcpy(answer, lpvoidFile, fileSize);
	answer[fileSize] = '\0';
	UnmapViewOfFile(lpvoidFile);
	CloseHandle(file_hhhhhandle);



	int size = MultiByteToWideChar(CP_UTF8, 0, answer, fileSize, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, answer, fileSize, buffer, size);
	buffer[size] = L'\0';
	size = WideCharToMultiByte(CP_ACP, 0, buffer, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, buffer, -1, answer, size, NULL, NULL);
	
	buf_real = &answer[3]; // 앞에 버퍼 떼고 
	printf("%s", buf_real);

	CloseHandle(file_handle2);
}

int main(int argc, _TCHAR* argv[])
{
	create_bob_txt();
	int code = copy_bob_txt();
	readFilewithReadFile();
	delete_bob_txt();
	return 0;
}
