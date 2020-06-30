/// <summary>
/// バイナリファイルを扱うクラス
/// </summary>
#pragma once


#include <memory>


class BinaryFile
{
public:
	// ファイル名を指定してロード
	static BinaryFile LoadFile(const wchar_t* fileName);

	// コンストラクタ
	BinaryFile();
	// ムーブコンストラクタ
	BinaryFile(BinaryFile&& in);

	// アクセッサ
	char* GetData()
	{
		return m_data.get();
	}
	size_t GetSize()
	{
		return m_size;
	}

protected:
	// データ
	std::unique_ptr<char[]> m_data;
	// サイズ
	size_t m_size;
};
