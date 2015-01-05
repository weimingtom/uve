#pragma once 
#include "INI_BASE.h" 
 
namespace DCG__INI__FILE 
{ 
	class CItemLine : public CINI_BASE 
	{ 
	public: 
		CItemLine(void); 
		~CItemLine(void); 
	public: 
 	// �����������صڼ���Item�� item ����(������ value )��index ��0��ʼ�������Ҳ������ر��� NOT_FOUND_ITEM 
		bool getValue(int index, string & RetValue); 
		bool getValue(const string & ItemName, string & RetValue); 
		void add_line(const string & Line); 
		void add_line(const string & ItemName, const string & value ); 
		bool get_item_and_value(const string & Line , string & item , string & value); 
		bool del_Item(const string & ItemName); 
		bool del_Item(int index); 
 
		bool getItemName( int index, string & item ); 
		bool getItemName( const string & ItemName, string & item ); 
 
		//��ȡĳһ�е����ݣ�����һЩ���������� 
		string getLine( int index ); 
		//��ȡ�����е����� 
		int getAllLineCount(); 
		//��ȡitem������ 
		int getItemCount(void); 
		// ������е� item �������� bIncludeOtherLine Ϊtrueʱ����Section�����е��У�����item�ĺ��������ֵģ�����ɾ���� 
		//������ bIncludeOtherLine Ϊ false ʱ��ֻɾ�� item �ġ��������ֱ��� 
		void clearAllItem(bool bIncludeOtherLine = false ); 
 
	protected: 
		LineInSection line_of_item; 
		int ITEM_COUNT; 
	}; 
};