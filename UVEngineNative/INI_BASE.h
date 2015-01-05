#pragma once 
#include <string>
#include <list>
using namespace std;

 
namespace DCG__INI__FILE 
{ 
 
	struct LINE_IN_ITEM_RANGE //����һ��ITEM�Ľṹ 
	{ 
	public: 
		std::string item;  //���� 
		std::string trimedItem; //�Ѿ�trim�������ƣ��� trim(item) �Ľ�� 
		std::string value; //ֵ 
		bool isItem;  //�Ƿ���ITEM����ITEM��Χ�ڣ�Ҳ�п�����һЩ�޹ص����֣���Щ���ֱ������� value �������Щ���ֵ� isItem Ϊ false �� 
	}; 
	typedef std::list<LINE_IN_ITEM_RANGE> LineInSection; //ÿһ�� Section �ڲ����� 
 
 
	static const basic_string<CHAR>::size_type npos = -1; 
 
 
 
	class CINI_BASE 
	{ 
	public: 
		CINI_BASE(void); 
		~CINI_BASE(void); 
		void trim(string & str); 
		// �ж�ĳ���Ƿ���section��ʽ���� [ ... ] ��ʽ 
		bool isSection(const char * Line); 
		// �ж�ĳ���Ƿ���Item��ʽ�����еȺţ����ҵȺ�����зǿ��ַ� 
		bool isItem(const char * Line); 
		// �Ƿ���� ���� ���š�Section �� Item д��ʱ����Ҫ�жϣ����ܰ������ں� 
		bool isContainEqu(const char * Line); 
		// �Ƿ���� ���� ���š�Section �� Item д��ʱ����Ҫ�жϣ����ܰ������ں� 
		bool isContainEqu(const string & Line); 
		// �Ƿ���������� 
		bool isContainBracket(const string & str); 
		// �ж��Ƿ������� 
		bool isDigital(const string & str); 
		// ��ȡ��һ���ո���ַ���ָ�� 
		char * getNextBlank(char * Line); 
		// ��ȡ��һ���ո���ַ���ָ�� 
		size_t getNextBlank(const string & Line); 
		// ��ȡ��һ���ǿ��ַ���ָ�� 
		char * getNextNotBlank(char * Line); 
		// ��ȡ��һ���ǿ��ַ���ָ�� 
		size_t getNextNotBlank(const string & Line); 
		// �ж��Ƿ���bool�ַ���, 
		// TRUE = 0 
		// FALSE = 1 
		// error = -1 
		int isBool( const string & str ); 
 
		bool CompareStringsIgnoreCase( const string & str1, const string & str2 ); 
	}; 
}; 
