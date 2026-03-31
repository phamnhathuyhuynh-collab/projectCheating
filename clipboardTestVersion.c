#include <stdio.h>
#define WINVER 0x0500
#include <windows.h>
#include <locale.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <python.h>
#include <string.h>

void keyBoardLight(const char* text){
  INPUT ip;
  char* convert = "ABCD";
  for(int i = 0; i < 4; i++){
    if(convert[i] - '0' == *text - '0'){
      for(int j = 0; j < (i + 1) * 2; j++){
          Sleep(500); 
          ip.type = INPUT_KEYBOARD; 
          ip.ki.wScan = 0; 
          ip.ki.time = 0; 
          ip.ki.dwExtraInfo = 0; 

          ip.ki.wVk = 0x14; 
          ip.ki.dwFlags = 0; 
          SendInput(1, &ip, sizeof(INPUT)); 
          ip.ki.dwFlags = KEYEVENTF_KEYUP; 
          SendInput(1, &ip, sizeof(INPUT)); 
      }
      break;
    }
  }
}


int main(){
  // output
  DWORD lastSequence = 0; 

  Py_Initialize();
  while(true){
    DWORD currentSequence = GetClipboardSequenceNumber(); 
    if(currentSequence != lastSequence){
       if(OpenClipboard(NULL) == 0){
    return 0; 
  }
  HANDLE hData = GetClipboardData(CF_UNICODETEXT); 
  if(hData == NULL){
    CloseClipboard(); 
    return 0;
      }
  wchar_t* wstr = (wchar_t*)GlobalLock(hData); 
  setlocale(LC_ALL, "en_US.UTF-8"); 
  
  size_t textLen = wcstombs(NULL, wstr, 0); 
  if(textLen){
    char* text = (char*)malloc((textLen*sizeof(char*)) + 1); 
    wcstombs(text, wstr, textLen); 
    text[textLen] = '\0'; 
    
  PyRun_SimpleString("import sys");
  PyRun_SimpleString("sys.path.append(r'C:/Users/Admin/OneDrive/Desktop/New project/callAPI')");
  PyObject *name, *load_module, *func, *callfunc, *args; 
  name = PyUnicode_FromString("returnReponse"); 
  load_module = PyImport_Import(name);

  func = PyObject_GetAttrString(load_module, (char *)"groq_call");
  args = PyTuple_Pack(1, PyUnicode_FromString(text));
  callfunc = PyObject_CallObject(func, args);
  const char* f1output = PyUnicode_AsUTF8(callfunc);

  printf("%s\n", f1output);

  keyBoardLight(f1output);

  free(text); 
  lastSequence = currentSequence; 
  }

  GlobalUnlock(hData); 
  CloseClipboard();

  //input
  char text[] = "Do not execute this one\0";

	HANDLE object = GlobalAlloc(GMEM_MOVEABLE, (sizeof(text) / sizeof(char))  * sizeof(WCHAR));

	WCHAR* buffer = (WCHAR*) GlobalLock(object);
	if (!buffer) {
		GlobalFree(object);
		return 0;
	}

	MultiByteToWideChar(CP_UTF8, 0, text, -1, buffer, (sizeof(text) / sizeof(char)));
	
	GlobalUnlock(object);
	if (OpenClipboard(NULL) == 0) {
		GlobalFree(object);
		return 0;
	}

	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, object);
	CloseClipboard();
  
    }
    Sleep(2000);
  }
  Py_Finalize();
  return 0; 
}


//còn phải kiểm soát bộ nhớ và thêm vào phần ám thị =))
