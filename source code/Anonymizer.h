// Anonymizer.h: interface for the CAnonymizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANONYMIZER_H__43D8742D_8D3B_4F62_8C0E_A138D4460A55__INCLUDED_)
#define AFX_ANONYMIZER_H__43D8742D_8D3B_4F62_8C0E_A138D4460A55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include <vector>

class CAnonymizer  
{
public:
	CAnonymizer();
	virtual ~CAnonymizer();

	void SetPrimaryTag(DWORD tag) { m_iPrimaryTag = tag; }

	// Scan for a list of DICOM files:
	int ScanFolder(LPCTSTR sFolder, int& nProgress);

	struct FileProperty
	{
		CString m_sFileName;  // File name to be appended to m_sFolder.
		CString m_sPatientName;
	};
	typedef std::vector<FileProperty> FileList;
	const FileList& GetFileList() { return m_FileList; }

	// Dictionary to anonymize them:
	struct NamePair
	{
		CString m_sName;
		CString m_sAnonymized;
	};
	typedef std::vector<NamePair> Dictionary;
	const Dictionary& GetDictionary() { return m_Dictionary; }
	void SetDictionary(const NamePair& entry);
	void SortDictionary(bool bByName);  // bByName: false to sort by Anonymized.

	void AssignNewNames(const CString& sPrefix, int nStart, bool bUseNumber);

	bool Anonymize(int index, bool bEraseInstitution);

	CString GetLastErrMsg() { return m_sErrMsg; }

private:

	DWORD m_iPrimaryTag;
	CString m_sFolder;  // with a "\" in the end.
	CString m_sErrMsg;

	FileList m_FileList;
	Dictionary m_Dictionary;

	void ScanFolderForFiles(LPCTSTR sFolder, LPCTSTR sFileName, int& nProgress);
		bool GetDicomFileProperties(const CString& sFullPath, FileProperty& F);
		void CreateDictionary();
	bool LookupDictionary(const CString& sName, CString& sAnonymized);

	static bool IsLikeDicomFile(const CString& sFullPath);  // Returns false if file is too short.
	static bool OpenDicomFile(const CString& sFullPath, CFile& file, bool bWrite=false);  // Returns false if is not DICOM.
	static bool ReadDicomTag(CFile& file, DWORD tag, CString& oValue);
	static bool SeekDicomTag(CFile& file, DWORD tag, DWORD& oLength);  // Points to the value.
	static bool ReplaceStringInFile(const CString& sFullPath, const CString& sOld, const CString& sNew);

	bool AnonymizeDicomFile(const CString& sFullPath, const CString& sNewPatientName, bool bEraseInstitution);
	static void EraseInstitution(CFile& file);

	static int CompareByName(const void* pair1, const void* pair2);
	static int CompareByAnonymized(const void* pair1, const void* pair2);
};

#endif // !defined(AFX_ANONYMIZER_H__43D8742D_8D3B_4F62_8C0E_A138D4460A55__INCLUDED_)
