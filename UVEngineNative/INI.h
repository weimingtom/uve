#pragma once 
#include "SECTION.h"
#include <vector>
#include <string>
using namespace std; 
using namespace DCG__INI__FILE; 
#define MAXFILENAME  4096 
#define DEFAULT_LINE 50 //����һ��ini�ļ�������50�У�����Ҳ�������У�û�����⣩ 
 
typedef std::vector<long> LineEnd; // ���з���λ�� 
 
class CINI :	public CINI_BASE 
{ 
public: 
	CINI();
	CINI( const char * FileName ); 
	~CINI(void); 
protected: 
	string _fileName; 
	string _iniContent; //INI�ļ����� 
	long GetFileLength( const char * FileName ); 
	// ���ļ������ڴ� 
	int LoadFileIntoMen(const char * FileName); 
	LineEnd lineEnd;  //���µĻ��з� 
	// ���������ݹ������з�, lineBreak �ǻ��з� 
	int RebuildLineBreak(char *  strContent, const char * lineBreak, size_t filesize ); 
	// ��ȡĳһ�У�lineNumber���к�,��ȡ�ɹ���������һ�е���ʼ�ַ��ĵ�ַ 
	const char * ReadLine( size_t lineNumber, char * buf );	 
 
	vector<std::string> headLine; //��һ��ini�ļ�ͷ����������һЩ���ֲ����κ�section���棬��Щ�ļ�ͳһ����headLine���� 
 
 
	CSECTION section; 
	//д�������ռ����������һ�������� string ���͵�value�� 
	void WriteValueCommon( const string & Section, const string & Item, const string & value ); 
	//���������ռ����������һ�������� string & ���͵�value�����ֵ�Ƕ�����ֵ��bool ���͵ĺ������ر�־��ʾ��������û�ж����� 
	bool ReadValueCommon( const string & Section, const string & Item, string & value ); 
	// ��ini��Ϣд���ļ� 
	void writeList(void); 
 
	//�Ƿ��޸Ĺ��ı�־ 
	//����д���������ɾ������ʱ��ֵ����ֵ�� true 
	//ÿ�α��棬��ֵ������Ϊ false 
	//�����ļ�ʱ�������ֵΪ false �Ͳ����� 
	bool bModified_; 
 
public: 
	int    ReadInt( const string & Section, const string & Item, const int DefaultValue ); 
	long   ReadLong( const string & Section, const string & Item, const long & DefaultValue ); 
	double ReadFloat( const string & Section, const string & Item, const double & DefaultValue ); 
	string  ReadString( const string & Section, const string & Item, const string & DefaultValue ); 
	bool   ReadBool( const string & Section, const string & Item, const bool DefaultValue ); 
 
	void WriteInt( const string & Section, const string & Item, const int i ); 
	void WriteLong( const string & Section, const string & Item, const long & i ); 
	void WriteFloat( const string & Section, const string & Item, const double & i ); 
	void WriteString( const string & Section, const string & Item, const string & i ); 
	void WriteBool( const string & Section, const string & Item, const bool i ); 
 
	void WriteFileNow(); 
 
	// ��ȡһ���ж��ٸ� section 
	int GetSectionCount(void); 
	// ��ȡ��index��section�����֡�index��0��ʼ���㡣�����ȡ�ɹ�������true��ͬʱ section_name ������ֵΪȡ�������� 
	bool GetSectionName(int index, string & section_name); 
	// ��ȡ�� index  �� section ������ж���item,ȡ��������-1 
	int GetItemCount(int index_of_Section); 
	// ��ȡ section_name ������ж���item,ȡ��������-1 
	int GetItemCount( const string & section_name ); 
 
	// �ӵ� index_of_Section �� section ���ڶ��item��ȡ���� index_of_Item �� Item ������ 
	bool GetItemName(int index_of_Section, int index_of_Item,  string & item_value ); 
	// ��ȡ section_name ��� index_of_Item �� Item ������ 
	bool GetItemName( const string & section_name, int index_of_Item,  string & item_value ); 
	// ��ȡ item ��ֵ 
	bool GetItemValue(int index_of_Section, int index_of_Item, string & value); 
	bool GetItemValue(const string & section_name, int index_of_Item, string & value); 
	bool GetItemValue(int index_of_Section, const string & item_name, string & value); 
	bool GetItemValue(const string & section_name, const string & item_name, string & value); 
	// ɾ���� index ��section 
	void DelSection(int index); 
	// ɾ����Ϊ section_name ��section 
	void DelSection( const string & section_name ); 
	// ɾ��ȫ��section 
	void clearSection(void); 
	// ɾ��item 
	void DelItem(int index_of_section, int index_of_item); 
	void DelItem( const string & section_name, int index_of_item); 
	void DelItem(int index_of_section, const string & item_name ); 
	void DelItem( const string & section_name, const string & item_name ); 
 
	// ɾ��ĳ��section�����е�item 
	//������ bIncludeOtherLine Ϊtrueʱ����Section�����е��У�����item�ĺ��������ֵģ�����ɾ���� 
	//������ bIncludeOtherLine Ϊ false ʱ��ֻɾ�� item �ġ��������ֱ��� 
	void clearItem(int index_of_section, bool bIncludeOtherLine = false ); 
 
	// ɾ��ĳ��section�����е�item 
	//������ bIncludeOtherLine Ϊtrueʱ����Section�����е��У�����item�ĺ��������ֵģ�����ɾ���� 
	//������ bIncludeOtherLine Ϊ false ʱ��ֻɾ�� item �ġ��������ֱ��� 
	void clearItem( const string & section_name, bool bIncludeOtherLine = false ); 
}; 
