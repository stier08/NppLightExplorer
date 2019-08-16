// Utils.h

#include <tchar.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#pragma warning ( disable : 4996 )

extern "C" {
int UTL_strcmp    (const TCHAR *string1, const TCHAR *string2);
int UTL_stricmp   (const TCHAR *string1, const TCHAR *string2);
int UTL_strncmp   (const TCHAR *string1, const TCHAR *string2, size_t count );
}

// --- Defines --------------------------------------------------------------------------------------------------------

#define _CRVERIFY(exp)      ((exp) ? TRUE : FALSE)
#define _CRCHECK(exp)

#define INI_SECTION_TARGETS "Targets"


inline void    *UTL_alloc(size_t num, size_t size )                                 { return calloc(num, size); }
inline void     UTL_free(void *memblock )                                           { free(memblock); }
inline TCHAR   *UTL_strcpy(TCHAR *string1, const TCHAR *string2 )                   { return _tcscpy(string1, string2); };
inline TCHAR   *UTL_strncpy(TCHAR *string1, const TCHAR *string2, size_t count )    { return _tcsncpy(string1, string2, count); };
inline int      UTL_vsnprintf( TCHAR *buf,size_t num,const TCHAR *fmt,va_list args) { return _vsntprintf(buf,num,fmt,args); }
inline TCHAR   *UTL_strcat( TCHAR *string1, const TCHAR *string2 )                  { return _tcscat(string1, string2 ); };
inline wchar_t *UTL_memmove( wchar_t *dest, const wchar_t *src, size_t count )      { return wmemmove(dest, src, count); }
inline void    *UTL_memcpy( wchar_t *dest, const wchar_t *src, size_t count )             { return wmemcpy(dest, src, count);}
inline int      UTL_strlen(const TCHAR *string )                                    { return (!string) ? 0 : (int)_tcslen(string); }
inline TCHAR   *UTL_strchr( TCHAR *string, int c )                                  { return _tcschr(string, c); };
inline TCHAR   *UTL_strstr( TCHAR *string1, const TCHAR *string2 )                  { return _tcsstr(string1,string2); };
inline TCHAR   *UTL_strrchr( TCHAR *string, int c )                                 { return _tcsrchr(string, c); };
inline size_t   UTL_strcspn( const TCHAR *string1, const TCHAR *string2 )           { return _tcscspn(string1, string2); };
inline long     UTL_atol( const TCHAR *string )                                     { return _tstol(string); };
inline int      UTL_atoi( const TCHAR *string )                                     { return _tstoi(string); };
inline TCHAR   *UTL_itoa( int value, TCHAR *string, int radix)                      { return _itot(value,string,radix); };
inline TCHAR   *UTL_ltoa( long value, TCHAR *string, int radix )                    { return _i64tot(value,string,radix); };
inline LPTSTR   UTL_strupr(LPTSTR lpsz)                                             { return CharUpper(lpsz); };
inline LPTSTR   UTL_strlwr(LPTSTR lpsz)                                             { return CharLower(lpsz); };
inline unsigned long UTL_strtoul(const TCHAR *nptr, TCHAR **endptr, int base )      { return _tcstoul(nptr,endptr,base); };
inline TCHAR   *UTL_strrev( TCHAR *string )                                         { return _tcsrev(string); };

inline LPCTSTR          UTL_Null(LPCTSTR expr, LPCTSTR defaultValue = _T("")) { return (LPCTSTR)((expr) ? expr : defaultValue);};
inline LPTSTR           UTL_Null(LPTSTR expr,  LPTSTR defaultValue = _T("")) { return (LPTSTR)((expr) ? expr : defaultValue);};

/****************************************************************************************************************************/
/*** Buffer que se auto libera si es construido en la pila. (No reserva espacio en la pila) */
class CUTL_BUFFER {   
public:
   CUTL_BUFFER();
   CUTL_BUFFER(LPCTSTR str);
   CUTL_BUFFER(UINT size);
   CUTL_BUFFER(const CUTL_BUFFER &other);
   virtual ~CUTL_BUFFER();
public:
   // Funciones.
   BOOL         Realloc(UINT size);
   BOOL         Accept(CUTL_BUFFER &other);
   LPTSTR        Detach();
   UINT         Len() const;
   LPTSTR        Copy(LPCTSTR string2);
   LPTSTR        NCopy(LPCTSTR string2, size_t count);
   LPTSTR        Cat(LPCTSTR string2);
   LPTSTR        InsStr(LPCTSTR str, UINT numChars, UINT pos, BOOL forceUseNumChars = FALSE);
   LPTSTR        InsCar(TCHAR ch, UINT pos);
   UINT         RepCar(TCHAR replaceCh, TCHAR withCh, UINT start=0);
   void         RemoveCar(UINT pos);
   CUTL_BUFFER &Sf(LPCTSTR fmt, ... );
   CUTL_BUFFER &SfVa(LPCTSTR fmt, va_list args);
   // conversión numérica/alfanumérica
   long         AToL();
   double       AToF();
   int          AToI();
   LPCTSTR       IToA(int value, int radix =10);
   LPCTSTR       LToA(long value, int radix =10);
   LPCTSTR       ULToA(unsigned long value, int radix = 10);
   // Otros métodos
   CUTL_BUFFER &Trim();
   BOOL         Find(LPCTSTR string, UINT &found, UINT start = 0) const;
   BOOL         Find(const TCHAR ch, UINT &found, UINT start = 0) const;
   BOOL         ReverseFind(const TCHAR ch, UINT &found) const;
   CUTL_BUFFER &Reverse();
   BOOL         FindOneOf(LPCTSTR set, UINT &found) const;
   LPCTSTR       GetSafe() const;

   CUTL_BUFFER &CUTL_BUFFER::Upper()        { if (data) UTL_strupr(data); return *this; }
   CUTL_BUFFER &CUTL_BUFFER::Lower()        { if (data) UTL_strlwr(data); return *this; }

public: // Operadores.  
   TCHAR        &operator[](UINT index) const;
   TCHAR        &operator[](int index) const;
   operator     LPTSTR() const;
   operator     LPCTSTR() const;   
   CUTL_BUFFER &operator=(const CUTL_BUFFER &buf);
   CUTL_BUFFER &operator=(LPCTSTR str);
   CUTL_BUFFER &operator+=(LPCTSTR str);
   
   BOOL         operator!() const;
   BOOL         operator<(LPCTSTR str) const;
   BOOL         operator<=(LPCTSTR str) const;
   BOOL         operator==(LPCTSTR str) const;
   BOOL         operator==(LPTSTR str) const;
   BOOL         operator!=(LPCTSTR str) const;
   BOOL         operator>=(LPCTSTR str) const;
   BOOL         operator>(LPCTSTR str) const;
public:
   LPTSTR data;

   //--- AREA X --------------------------------------------------------
private:
   UINT length;                               
};

inline UINT CUTL_BUFFER::Len() const            { return UTL_strlen(data);}
inline LPCTSTR      CUTL_BUFFER::GetSafe() const{ return (data) ? data : _T(""); }
inline CUTL_BUFFER &CUTL_BUFFER::Reverse()      { if (data) UTL_strrev(data); return *this; }
inline CUTL_BUFFER::operator LPTSTR() const      { return data; }
inline CUTL_BUFFER::operator LPCTSTR() const     { return data; }

inline BOOL CUTL_BUFFER::operator!() const            {return data ? FALSE : TRUE; }
inline BOOL CUTL_BUFFER::operator<(LPCTSTR str) const  {return (UTL_strcmp(data, str) < 0); }
inline BOOL CUTL_BUFFER::operator<=(LPCTSTR str) const {return (UTL_strcmp(data, str) <= 0);}
inline BOOL CUTL_BUFFER::operator==(LPCTSTR str) const {return (UTL_strcmp(data, str) == 0);}
inline BOOL CUTL_BUFFER::operator==(LPTSTR str)  const {return (UTL_strcmp(data, str) == 0);}
inline BOOL CUTL_BUFFER::operator!=(LPCTSTR str) const {return (UTL_strcmp(data, str) != 0);}
inline BOOL CUTL_BUFFER::operator>=(LPCTSTR str) const {return (UTL_strcmp(data, str) >= 0);}
inline BOOL CUTL_BUFFER::operator>(LPCTSTR str) const  {return (UTL_strcmp(data, str) > 0); }


class CUT2_INI : public CUTL_BUFFER {
public: //constructores
   CUT2_INI(LPCTSTR filename);
   virtual ~CUT2_INI();
public: //Funciones
   int    LoadInt(LPCTSTR section, LPCTSTR entry, int def=0);
   LPCTSTR LoadStr(LPCTSTR section, LPCTSTR entry, LPCTSTR def = NULL);
   long   LoadLong(LPCTSTR section, LPCTSTR entry, long def =0);
   BOOL   Write(LPCTSTR section, LPCTSTR entry, LPCTSTR str) const;
   BOOL   Write(LPCTSTR section, LPCTSTR entry, int value) const;
   BOOL   Write(LPCTSTR section, LPCTSTR entry, long value) const;
   BOOL   Delete(LPCTSTR section, LPCTSTR entry=NULL) const;
   BOOL   Flush() const;
   LPCTSTR LoadEntries(LPCTSTR section);
   LPCTSTR LoadSections();
   LPCTSTR GetEntry(UINT pos) const;
public: // Operadores
   operator int();
   operator DWORD();

   /*--- Area X ------------------------------------*/
private: // miembros
   CUTL_BUFFER m_file;
   int   m_int;   
   long m_long;
};

/***********************************************************************************************/
/*** Clase para manipular fechas */
class CCR3_DATE {
public:
   CCR3_DATE();
   CCR3_DATE(LPCTSTR date);
   CCR3_DATE(time_t timeExpr);
   CCR3_DATE(const CCR3_DATE &other);
   virtual ~CCR3_DATE();
   
   // Para operar la información directamente (Usar con cautela)
   operator    LPTSTR();
   operator    LPCTSTR();   
   operator    time_t();
   // Asignaciones
   CCR3_DATE &operator=(const CCR3_DATE &date);
   CCR3_DATE &operator=(LPCTSTR date);   
   CCR3_DATE &operator=(time_t timeExpr);
   void        SysDate();
   BOOL        SetDate(int yy, int mm, int dd);
   BOOL        SetTime(int hh, int mi, int ss, int fs = 0);
   void        ResetTime();
   void        ResetDate();
   void        Clear();
   // Lecturas
   int m_yy, m_mm, m_dd, m_hh, m_mi, m_ss, m_fs;
   TCHAR m_info[18];
   // Operaciones
   BOOL        operator<(const CCR3_DATE &date);
   BOOL        operator<=(const CCR3_DATE &date);
   BOOL        operator>(const CCR3_DATE &date);
   BOOL        operator>=(const CCR3_DATE &date);   
   BOOL        operator==(const CCR3_DATE &date);   

   static UINT SizeOf();

public: // Función para permitir que CCR3_DATE sea clave de tablas CUTL_KEYMAP

   // Formateo
   enum formatStyle {DDMMYY=0, DDHHMI=1, DDMMYYHHMISS=2, DDMMYYHHMISSML=3, HHMISS=4, HHMI=5};
   void Format(formatStyle style, CUTL_BUFFER* target);
private:
   BOOL        ChkDate(int yy, int mm, int dd);
   BOOL        ChkTime(int hh, int mi, int ss, int ml);
};

/****************************************************************************************************************************/
/*** Clase para manipular rutas de ficheros (paths) */
class CUTL_PATH {
public: // Constantes
   enum SPECIALDIR {DIR_CURRENT, DIR_WINDOWS, DIR_SYSTEM, DIR_MODULE, DIR_TEMP};
   enum MAX_VALS {
      MAX_FILEPATHLENGTH = _MAX_PATH,
      MAX_FILENAMELENGTH = 8,
      MAX_FILEEXTLENGTH  = 3
   };
public: // Constructores
	CUTL_PATH();
	CUTL_PATH(const CUTL_PATH &other);
	CUTL_PATH(LPCTSTR path);
	CUTL_PATH(SPECIALDIR specialDir);
	virtual ~CUTL_PATH();

public: //métodos de carga y/o configuración
	void Set(LPCTSTR path);
	void SetDrive(TCHAR driveLeter);
	void SetDriveDirectory(LPCTSTR pDriveDirectory);
	void SetDirectory(LPCTSTR pDirectory, BOOL ensureAbsolute = FALSE);
	void SetName(LPCTSTR pName);
	void SetNameExtension(LPCTSTR pNameExtension);
	void SetExtension(LPCTSTR pExtension);
	void AppendDirectory(LPCTSTR pSubDirectory);
	void UpDirectory(CUTL_BUFFER* pLastDirectory = NULL);
	void SetComponents (LPCTSTR drive, LPCTSTR directory, LPCTSTR name, LPCTSTR extension);

public: //métodos automáticos de crecaión de rutas
	void CurrentDirectory();
	void WindowsDirectory();
	void SystemDirectory();
	void TempDirectory();

	void PrivateProfile();
	void LocalProfile(LPCTSTR iniName, LPCTSTR extension = NULL);

	void MakeRoot ();

	void Clear();

public: //métodos de consulta de datos
	void GetDrive(CUTL_BUFFER &drive) const;
	void GetDriveDirectory(CUTL_BUFFER &driveDirectory) const;
	void GetDirectory(CUTL_BUFFER &directory) const;
	void GetName(CUTL_BUFFER &name) const;
	void GetNameExtension(CUTL_BUFFER &nameExtension) const;
	void GetExtension(CUTL_BUFFER &extension) const;
	void GetComponents (CUTL_BUFFER *pDrive = NULL, CUTL_BUFFER *pDir = NULL, CUTL_BUFFER *pName = NULL, CUTL_BUFFER *pExt = NULL) const;
	BOOL GetFullyQualified(CUTL_BUFFER& fullyQualified) const;    
	BOOL GetTime(CCR3_DATE &date) const;

public: //métodos de actuación sobre la unidad
	BOOL IsRemovableDrive() const;
	BOOL IsCDRomDrive() const;
	BOOL IsNetworkDrive() const;
	BOOL IsRAMDrive() const;

	LONG DriveTotalSpaceBytes() const;
	LONG DriveFreeSpaceBytes() const;
	LONG GetDriveClusterSize() const;


public: //métodos de actuación sobre el directorio
	BOOL DirectoryExists() const;
	BOOL IsDirectoryEmpty() const;

	BOOL CreateDirectory(BOOL createIntermediates =TRUE);
	BOOL RemoveDirectory();
	BOOL RemoveDirectoryContent();
	BOOL ChangeDirectory();

public: //métodos de actuación sobre el fichero
   BOOL Exists() const;
   LONG GetSize() const; 

   BOOL Delete(BOOL evenIfReadOnly =TRUE);
   BOOL Rename(LPCTSTR newPath);	

   BOOL FindFirst(DWORD attributes = _A_NORMAL);
   BOOL FindNext();

public: //operadores
	BOOL       IsEmpty()  const;
   BOOL       IsValid () const;
   BOOL       IsWild ()  const;
   
   CUTL_PATH& operator= (const CUTL_PATH &other);
   CUTL_PATH& operator= (LPCTSTR path);
              operator LPCTSTR() const;
   BOOL       operator == (const CUTL_PATH &other) const;
    

   /*--- Area X------------------------------------------------*/

protected: //otros métodos
   void        xInit();
   void        xClose();

   BOOL        xGetDiskFreeSpace(LPDWORD pSectorsPerCluster, LPDWORD pBytesPerSector, LPDWORD pFreeClusters, LPDWORD pClusters) const;


private: //miembros
   CUTL_BUFFER m_path;
   DWORD			m_findFileAttributes;
	HANDLE      m_hFindFile;
};


/**** Clase para traducir strings con sentencias de escape */
class CUTL_PARSE {
public:
   CUTL_PARSE();
   CUTL_PARSE(LPCTSTR srcStream, LPCTSTR charSet = NULL, TCHAR escape = NULL);
   virtual ~CUTL_PARSE();
public:
   enum token {T_STRING, T_ESCAPE, T_CHARSET, T_EOF, T_ERROR, T_NOTOKEN};
   //////////////////////////////////////////////////////////
   // Formato de los tokens
   // type      | NumArgs | Args
   //--------------------------------------------------------
   // T_STRING  |    1    | (Str) cadena
   // T_CHARSET |    1    | (chr) caracter
   // T_ESCAPE  | depende | depende
   // T_EOF     |    0    |
   // T_ERROR   |    0    | El mensaje de error queda registrado en CORE
   // T_NOTOKEN |    0    | 
   //////////////////////////////////////////////////////////
   
   void   SetCharSet(LPCTSTR charSet, TCHAR escape = NULL);
   void   SetStream(LPCTSTR srcStream);
   token  NextToken();
   token  LookAhead();
   void   Restart();
   // Para obtener la información del último token leido
   token  Type()                 { return m_tokenType; };
   UINT   NumArgs()              { return m_tokenNumArgs; };
   // lectura de argumentos con conversión
   LPTSTR StrArg(UINT pos =1);
   DWORD  LongHexArg(UINT pos =1);
   long   LongArg(UINT pos =1);
   int    IntArg(UINT pos =1);
   TCHAR   ChrArg(UINT pos =1);

   //--- AREA X --------------------------------------------------------
private: 
   token xSetError();

private: // Miembros
   CUTL_BUFFER m_charSet;
   TCHAR        m_escape;
   // stream
   CUTL_BUFFER m_stream;
   // posición en el stream
   UINT        m_offset;
   // información del último token leido
   token       m_tokenType;
   UINT        m_tokenNumArgs;
   CUTL_BUFFER m_tokenArgs;  
};


//#ifdef UTILS

/*--- Constantes ------------------------------------------------------------*/

const TCHAR    PATH_DRIVE_DELIMITER  = _T(':');
const TCHAR    PATH_DIR_DELIMITER    = _T('\\');
const TCHAR    PATH_EXT_DELIMITER    = _T('.');

LPCTSTR const  PATH_INI_EXT          = _T("ini");
LPCTSTR const  PATH_WILD_NAME_EXT    = _T("*.*");
LPCTSTR const  PATH_WILD_SET         = _T("?*");


void        StripLeadingChar(CUTL_BUFFER& text, TCHAR leadingCh);
void        StripLeadingBackslash (CUTL_BUFFER& directory);
void        StripTrailingChar(CUTL_BUFFER& text, TCHAR traillingCh);
void        StripTrailingBackslash (CUTL_BUFFER& directory);
void        EnsureTrailingBackslash (CUTL_BUFFER& directory);
void        EnsureLeadingBackslash(CUTL_BUFFER& directory);
BOOL        AttributesMatch(DWORD targetAttributes, DWORD fileAttributes);

//#endif