//WindowsXP.h written by david paull.
//#define OUTPUT_TO_DEBUGGER//output text to VisualC++ Output Window
#define OUTPUT_TO_FILE//output text to errors.txt

void InitDebugMsg();
void OutputMatrix(float* pMat);
void AppendDebugMsg(char *pChar);

void ReadFromFile(HANDLE fH,char* pData,long Size);
void WriteToFile(HANDLE fH,char* pData,long Size);
void AddPathToFileName(char *pFileName);
void RemovePathFromFileName(char *pFileName);
void RemoveFilenameAndExt(char *pFileName);
void RemoveExtensionFromFileName(char *pFileName);
bool IsValidFilename(char* pName);
void GetPath(char *pFileName);

bool LoadBMP(char*  pFileName,
			 unsigned char*& pBits,
			 long&  Width,
			 long&  Height);
bool SaveBMP(char* pFileName,
			 unsigned char* pBits,
			 long  Width,
			 long  Height); 

unsigned int ReadNextInteger(char *ch);
float ReadNextFloat(char *ch);
enum_FILE_TYPE GetFileType(char* pFileName);

