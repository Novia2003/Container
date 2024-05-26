
// TextDoc.cpp: реализация класса CTextDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "Container.h"
#endif

#include "TextDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTextDoc

IMPLEMENT_DYNCREATE(CTextDoc, CDocument)

BEGIN_MESSAGE_MAP(CTextDoc, CDocument)
	ON_COMMAND(ID_32771, &CTextDoc::OnSelectWordsThatAreLessThan5LettersLong)
	ON_COMMAND(ID_32772, &CTextDoc::OnSelectPalindromeWords)
END_MESSAGE_MAP()


// Создание или уничтожение CTextDoc

CTextDoc::CTextDoc() noexcept
{
	// TODO: добавьте код для одноразового вызова конструктора

}

CString CTextDoc::GetText()
{
	return _fileText;
}

void CTextDoc::SetText(CString text)
{
	_fileText = text;
}

CTextDoc::~CTextDoc()
{
}

BOOL CTextDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	return TRUE;
}




// Сериализация CTextDoc

void CTextDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar.WriteString(_fileText);
	}
	else
	{
		CString buf;

		while (ar.ReadString(buf))
		{
			_fileText.Append(buf);
			_fileText.Append("\n");
		}

		_logText.Format("Received file with size %d", _fileText.GetLength());
		logInfo(_logText, "INFO");
	}

}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CTextDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CTextDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CTextDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Диагностика CTextDoc

#ifdef _DEBUG
void CTextDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTextDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CTextDoc


void CTextDoc::OnSelectWordsThatAreLessThan5LettersLong()
{
	if (_fileText.GetLength() > 0)
	{
		_resultText.Empty();
		CString word;
		int wordCount = 0;

		for (int i = 0; i < _fileText.GetLength(); ++i) {
			if (isalpha(_fileText[i])) {
				word += _fileText[i];
			}
			else {
				if (word.GetLength() > 0 && word.GetLength() < 5) {
					_resultText += word + _T("\n");
					wordCount++;
				}

				word.Empty();
			}
		}

		if (word.GetLength() > 0 && word.GetLength() < 5) {
			_resultText += word + _T("\n");
			wordCount++;
		}

		_logText.Format("Found words with a length of less than 5 letters: %d", wordCount);
		logInfo(_logText, "INFO");

		auto pos = theApp.GetFirstDocTemplatePosition();
		auto pDocTemplate = theApp.GetNextDocTemplate(pos);
		if (pDocTemplate)
		{
			auto doc = dynamic_cast<CTextDoc*>(pDocTemplate->CreateNewDocument());
			doc->SetText(_resultText);
			auto chld = pDocTemplate->CreateNewFrame(
				doc,
				dynamic_cast<CFrameWnd*>(theApp.GetMainWnd())
			);
			chld->ShowWindow(true);
		}
	}
	else
	{
		MessageBox(NULL, _T("No text in doc. Open file with text and retry..."), _T("Message"), MB_OK);
	}
}

bool CTextDoc::IsPalindrome(const CString& word) {
	int length = word.GetLength();
	for (int i = 0; i < length / 2; ++i) {
		if (word[i] != word[length - 1 - i]) {
			return false;
		}
	}
	return true;
}



void CTextDoc::OnSelectPalindromeWords()
{
	if (_fileText.GetLength() > 0)
	{
		_resultText.Empty();
		CString word;
		int wordCount = 0;

		for (int i = 0; i < _fileText.GetLength(); ++i) {
			if (isalpha(_fileText[i])) {
				word += _fileText[i];
			}
			else {
				if (!word.IsEmpty() && word.GetLength() >= 3 && IsPalindrome(word)) {
					_resultText += word + _T("\n");
					wordCount++;
				}

				word.Empty();
			}
		}

		if (!word.IsEmpty() && word.GetLength() >= 3 && IsPalindrome(word)) {
			_resultText += word + _T("\n");
			wordCount++;
		}

		_logText.Format("Palindrome words found: %d", wordCount);
		logInfo(_logText, "INFO");

		auto pos = theApp.GetFirstDocTemplatePosition();
		auto pDocTemplate = theApp.GetNextDocTemplate(pos);

		if (pDocTemplate)
		{
			auto doc = dynamic_cast<CTextDoc*>(pDocTemplate->CreateNewDocument());
			doc->SetText(_resultText);
			auto chld = pDocTemplate->CreateNewFrame(
				doc,
				dynamic_cast<CFrameWnd*>(theApp.GetMainWnd())
			);
			chld->ShowWindow(true);
		}
	}
	else
	{
		MessageBox(NULL, _T("No text in doc. Open file with text and retry..."), _T("Message"), MB_OK);
	}
}

void CTextDoc::logInfo(CString message, CString type)
{
	CString logFilePath = _T("log.txt");
	CString fullMessageWithTypeAndTime;

	CStdioFile logFile;

	CTime currentTime = CTime::GetCurrentTime();

	int year = currentTime.GetYear();
	int month = currentTime.GetMonth();
	int day = currentTime.GetDay();
	int hour = currentTime.GetHour();
	int minute = currentTime.GetMinute();
	int second = currentTime.GetSecond();

	CString timeString;
	timeString.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), year, month, day, hour, minute, second);

	fullMessageWithTypeAndTime.Append(timeString);
	fullMessageWithTypeAndTime.Append("\t");
	fullMessageWithTypeAndTime.Append(type);
	fullMessageWithTypeAndTime.Append("\t");
	fullMessageWithTypeAndTime.Append(message);
	fullMessageWithTypeAndTime.Append("\n");

	if (logFile.Open(logFilePath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)) {
		logFile.SeekToEnd();

		logFile.WriteString(fullMessageWithTypeAndTime);

		logFile.Close();
	}
}
