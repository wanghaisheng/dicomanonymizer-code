// Anonymizer.cpp: implementation of the CAnonymizer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DicomAnonym.h"
#include "Anonymizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnonymizer::CAnonymizer()
{
	m_iPrimaryTag = 0x00100010;  // Patient's Name
}

CAnonymizer::~CAnonymizer()
{

}

//------------------------------------------------------------------------------
int CAnonymizer::ScanFolder(LPCTSTR sFolder, int& nProgress)
{
	m_sFolder = sFolder;
	m_sFolder.TrimRight(_T("/"));
	if (m_sFolder.Right(1) != _T("\\"))
		m_sFolder += _T("\\");

	m_FileList.clear();
	ScanFolderForFiles(m_sFolder, _T("*.*"), nProgress);
	CreateDictionary();

	return m_FileList.size();
}

//------------------------------------------------------------------------------
void CAnonymizer::SetDictionary(const NamePair& entry)
{
	Dictionary::iterator it;
	for (it = m_Dictionary.begin(); it != m_Dictionary.end(); it++)
	{
		if (entry.m_sName == it->m_sName)
		{
			if (entry.m_sAnonymized != it->m_sAnonymized)
			{
				*it = entry;
			}
			return;
		}
	}
	m_Dictionary.push_back(entry);
}

//------------------------------------------------------------------------------
void CAnonymizer::SortDictionary(bool bByName)  // bByName: false to sort by Anonymized.
{
	int (__cdecl *Compare )(const void *elem1, const void *elem2 ) = bByName ? CompareByName : CompareByAnonymized;

	for (int iMain = 0; iMain < m_Dictionary.size(); iMain++)
	{
		// Select the smallest one from the rest:
		int iSelect = iMain;
		for (int iSub = iMain+1; iSub < m_Dictionary.size(); iSub++)
		{
			if (Compare(&m_Dictionary[iSub], &m_Dictionary[iSelect]) < 0)
			{
				iSelect = iSub;
			}
		}
		// Swap it to main:
		if (iSelect != iMain)
		{
			NamePair swap = m_Dictionary[iSelect];
			m_Dictionary[iSelect] = m_Dictionary[iMain];
			m_Dictionary[iMain] = swap;
		}
	}
}

//------------------------------------------------------------------------------
int CAnonymizer::CompareByName(const void* pair1, const void* pair2)
{
	if (((const NamePair*)pair1)->m_sName < ((const NamePair*)pair2)->m_sName)
	{
		return -1;
	}
	return (((const NamePair*)pair1)->m_sName > ((const NamePair*)pair2)->m_sName);
}

//------------------------------------------------------------------------------
int CAnonymizer::CompareByAnonymized(const void* pair1, const void* pair2)
{
	if (((const NamePair*)pair1)->m_sAnonymized < ((const NamePair*)pair2)->m_sAnonymized)
	{
		return -1;
	}
	return (((const NamePair*)pair1)->m_sAnonymized > ((const NamePair*)pair2)->m_sAnonymized);
}


//------------------------------------------------------------------------------
void CAnonymizer::AssignNewNames(const CString& sPrefix, int nStart, bool bUseNumber)
{
	int n = nStart;

	std::vector<NamePair>::iterator it;
	for (it = m_Dictionary.begin(); it != m_Dictionary.end(); it++)
	{
		NamePair& pair = *it;
		int nLength = pair.m_sName.GetLength();

		// Generate new name of the same length:
		CString sAnonym;
		if (bUseNumber)
		{
			sAnonym.Format(sPrefix + "%03d", n);
		}
		else
		{
			sAnonym = sPrefix;
		}
		int nAnonym = sAnonym.GetLength();
		if (nAnonym < nLength)
		{
			for (int i=0; i < (nLength-nAnonym); i++)
			{
				sAnonym += " ";
			}
		}
		else
		{
			sAnonym = sAnonym.Right(nLength);
		}

		pair.m_sAnonymized = sAnonym;
		n++;
	}
}

//------------------------------------------------------------------------------
bool CAnonymizer::Anonymize(int index, bool bEraseInstitution)
{
	int nFiles = GetFileList().size();
	if ((index < 0) || (index >= nFiles))
	{
		m_sErrMsg.Format("Index %d is out of range %d.", index, nFiles);
		return false;
	}

	const FileProperty& prop = GetFileList().at(index);
	CString sFilePath = m_sFolder + prop.m_sFileName;
	CString sAnonymized;
	if (!LookupDictionary(prop.m_sPatientName, sAnonymized))
	{
		m_sErrMsg.Format("Can not find patient name '%d' in dictionary.", prop.m_sPatientName);
		return false;
	}

	bool bOK = AnonymizeDicomFile(sFilePath, sAnonymized, bEraseInstitution);
	if (!bOK)
	{
		m_sErrMsg = "Can not anonymize file\n";
		m_sErrMsg += sFilePath;
		m_sErrMsg += "\n\nPossible reasons:\n";
		m_sErrMsg += "1. It is not a DICOM file, or\n";
		m_sErrMsg += "2. It is read-only.\n";
		return false;
	}
	return true;
}

//=========================================================================
void CAnonymizer::ScanFolderForFiles(LPCTSTR sFolder, LPCTSTR sFileName, int& nProgress)
{
	CString sCast = sFolder;
	ASSERT(sCast.Right(1) == _T("\\"));
	sCast += sFileName;

	enum FolderType
	{
		kUncertain,
		kDicomFolder,
		kNonDicomFolder
	};
	FolderType eFolderType = kUncertain;

	CFileFind FF;
	BOOL bFoundSomething = FF.FindFile(sCast);
	
	FileProperty F;

	while (bFoundSomething)
	{
		bFoundSomething = FF.FindNextFile();
		if (FF.IsDots())
		{
			continue;
		}
		else if (FF.IsDirectory())
		{
			CString sNewFolder = CString(sFolder) + FF.GetFileName() + _T("\\");
			ScanFolderForFiles(sNewFolder, sFileName, nProgress);
			continue;
		}

		// Found a file. Is it a DICOM file?
		if (eFolderType == kUncertain)
		{
			if (GetDicomFileProperties(FF.GetFilePath(), F))
			{
				eFolderType = kDicomFolder;
			}
			else
			{
				eFolderType = kNonDicomFolder;
				continue;
			}
		}

		// Add the DICOM file into list:
		if (eFolderType == kDicomFolder)
		{
			F.m_sFileName = FF.GetFilePath().Mid(m_sFolder.GetLength());
			m_FileList.push_back(F);
			nProgress++;
		}
	}

	nProgress = m_FileList.size();
}


//=========================================================================
bool CAnonymizer::GetDicomFileProperties(const CString& sFullPath, FileProperty& F)
{
	F.m_sFileName = sFullPath.Mid(m_sFolder.GetLength());

	if (!IsLikeDicomFile(sFullPath))
	{
		return false;
	}

	CFile file;
	if (!OpenDicomFile(sFullPath, file))
	{
		return false;
	}
	if (!ReadDicomTag(file, m_iPrimaryTag, F.m_sPatientName))
	{
		return false;
	}
	file.Close();

	return true;
}


//=========================================================================
void CAnonymizer::CreateDictionary()
{
	m_Dictionary.resize(0);

	FileList::const_iterator it;
	for (it = m_FileList.begin(); it != m_FileList.end(); it++)
	{
		NamePair entry;
		entry.m_sName = it->m_sPatientName;
		SetDictionary(entry);
	}

}

//=========================================================================
bool CAnonymizer::LookupDictionary(const CString& sName, CString& sAnonymized)
{
	Dictionary::const_iterator it;
	for (it = m_Dictionary.begin(); it != m_Dictionary.end(); it++)
	{
		if (sName == it->m_sName)
		{
			sAnonymized = it->m_sAnonymized;
			return true;
		}
	}
	return false;
}

//=========================================================================
bool CAnonymizer::IsLikeDicomFile(const CString& sFullPath)
{
	// Check file length:
	CFileFind FF;
	if (!FF.FindFile(sFullPath))
	{
		return false;
	}
	FF.FindNextFile();

	DWORD iLength = FF.GetLength();
	if (iLength < 512)  // File is too small.
	{
		return false;
	}

	return true;
}

//=========================================================================
bool CAnonymizer::OpenDicomFile(const CString& sFullPath, CFile& file, bool bWrite)
{
	if (!file.Open(sFullPath, bWrite ? CFile::modeReadWrite : CFile::modeRead))
	{
		return false;
	}
	const UINT kBufferSize = 0x84;
	const UINT nDwords = kBufferSize / 4;
	DWORD buffer[nDwords];
	UINT nRead = file.Read(buffer, kBufferSize);
	if (nRead == kBufferSize)
	{
#if 0
		// Check leading zeros:
		for (int i=0; i< nDwords-1; i++)
		{
			if (buffer[i] != 0)
			{
				file.Close();
				return false;
			}
		}
#endif
		// "DICM":
		if (buffer[nDwords-1] == 0x4D434944)
		{
			return true;
		}
	}

	file.Close();
	return false;
}

//=========================================================================
bool CAnonymizer::ReadDicomTag(CFile& file, DWORD tag, CString& oValue)
{
	const UINT kBufferSize = 512;
	DWORD nLength;
	if (SeekDicomTag(file, tag, nLength) && (nLength < kBufferSize))
	{
		// Read the string:
		char buffer[kBufferSize];
		file.Read(buffer, nLength);
		oValue = CString((char*)buffer, nLength);
		return true;
	}
	return false;
}

//=========================================================================
bool CAnonymizer::SeekDicomTag(CFile& file, DWORD tag, DWORD& oLength)
{
	const UINT kBlockSize = 1024;
	WORD buffer[kBlockSize / sizeof(WORD)];
	WORD* pTag = (WORD*)&tag;

	// Find the tag:
	bool bFound = false;
	UINT nRead = kBlockSize;
	while (!bFound && (nRead == kBlockSize))
	{
		nRead = file.Read(buffer, kBlockSize);
		UINT nWords = nRead / sizeof(WORD);
		for (int i=0; i < nWords-2; i++)
		{
			if ((buffer[i] == pTag[0]) && (buffer[i+1] == pTag[1]))
			{
				file.Seek(i * sizeof(WORD) + 4 - nRead, CFile::current);
				bFound = true;
				break;
			}

			// Removed because this can happen before patient name in some dose report file.
			//if ((buffer[i] == 0x7FE0) && (buffer[i+1] == 0x0010))
			//{
			//	break;  // Reached pixel data. Terminate.
			//}
		}
		if (bFound)
		{
			break;
		}
		file.Seek(-4, CFile::current);
	}
	if (!bFound)
	{
		return false;
	}

	// Read the length:
	file.Read(&oLength, sizeof(oLength));
	const unsigned long kMaxLength = 127;
	if (oLength > kMaxLength)
	{
		oLength = oLength >> 16;
		if (oLength > kMaxLength)
		{
			ASSERT(0);  // bad length
			return false;
		}
	}

	return true;
}

//=========================================================================
bool CAnonymizer::ReplaceStringInFile(const CString& sFullPath, const CString& sOld, const CString& sNew)
{
	const UINT nWordSize = sOld.GetLength();
	if (nWordSize == 0)
	{
		return true;
	}
	char firstChar = sOld[0];

	CFile file;
	if (!file.Open(sFullPath, CFile::modeReadWrite))
	{
		return false;
	}

	const UINT kBlockSize = 1024;
	char buffer[kBlockSize];

	while (1)
	{
		UINT nRead = file.Read(buffer, kBlockSize);
		int nScan = nRead - nWordSize;
		for (int i=0; i<nScan; i++)
		{
			if (buffer[i] == firstChar)
			{
				CString sFound(buffer+i, nWordSize);
				if (sFound == sOld)
				{
					file.Seek(i-kBlockSize, CFile::current);
					file.Write((LPCTSTR)sNew, nWordSize);
					return true;
				}
			}
		}
		if (nRead < kBlockSize)
		{
			break;
		}
		file.Seek(-nWordSize, CFile::current);
	}

	ASSERT(0);
	return false;
}

//=========================================================================
bool CAnonymizer::AnonymizeDicomFile(const CString& sFullPath, const CString& sNewPatientName, bool bEraseInstitution)
{
	CFile file;
	if (!OpenDicomFile(sFullPath, file, true))
	{
		return false;
	}

	if (bEraseInstitution)
	{
		EraseInstitution(file);
	}

	// Replace patient name:
	DWORD nLength;
	if (SeekDicomTag(file, m_iPrimaryTag, nLength))
	{
		ASSERT(sNewPatientName.GetLength() == nLength);
		file.Write((LPCTSTR)sNewPatientName, nLength);
	}

	file.Close();
	return true;
}

//=========================================================================
void CAnonymizer::EraseInstitution(CFile& file)
{
	DWORD tags[] =
	{
		0x00800008,  // Institution Name
		0x00900008,  // Referring Physician's Name
		0x10100008,  // Station Name
		0x10700008  // Operators' Name
	};
	const nTags = sizeof(tags) / sizeof(tags[0]);

	LONG pos = file.Seek(0, CFile::current);
	for (int iTag=0; iTag < nTags; iTag++)
	{
		DWORD nLength;
		if (SeekDicomTag(file, tags[iTag], nLength))
		{
			if ((0 < nLength) && (nLength < 256))
			{
				CString str(' ', nLength);
				file.Write((LPCTSTR)str, nLength);
			}
		}
		else
		{
			file.Seek(pos, CFile::begin);
		}
	}
}