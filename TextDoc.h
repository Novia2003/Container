
// TextDoc.h: интерфейс класса CTextDoc 
//


#pragma once


class CTextDoc : public CDocument
{
protected: // создать только из сериализации
	CTextDoc() noexcept;
	DECLARE_DYNCREATE(CTextDoc)

	CString _fileText;
	CString _resultText;
	CString _logText;

// Атрибуты
public:

// Операции
public:

// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

	bool IsPalindrome(const CString& word);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	virtual ~CTextDoc();

	virtual CString GetText();
	virtual void SetText(CString text);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void logInfo(CString message, CString type);

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnSelectWordsThatAreLessThan5LettersLong();
	afx_msg void OnSelectPalindromeWords();
};
