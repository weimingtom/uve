#pragma once 
#include "ItemLine.h" 
 
namespace DCG__INI__FILE 
{ 
	//���� section �Ľṹ 
	struct ONE_SECTION 
	{ 
		string section; 
		string trimedSection;	 
		CItemLine items; 
	}; 
 
	typedef list<ONE_SECTION> SECTION_LIST; 
 
 
	class CSECTION :	public CINI_BASE 
	{ 
	public: 
		CSECTION(void); 
		~CSECTION(void); 
 
	public: 
		bool get_SectionName (const string & Line, string & section ); 
		//����һ�У���һ�е������� Line ���� 
		//���� bIsSectionFormat Ϊ false ʱ������� Line ������ [ Section ] �ĸ�ʽ�Żᱻ�����������ĸ�ʽ���ԣ��᷵��NULL 
		//���� bIsSectionFormat Ϊ true  ʱ������� Line ���벻���� [ ] ��Ȼ��ᱻ����һ�� Section ��������������� [ ]���᷵�� NULL 
		CItemLine * add_line(const string & Line, bool bIsSectionFormat = true ); 
 
		//����������ɾ�� section������������Ҳ���������false������һ����ɾ���ɹ�������true 
		bool del_section(const string & ItemName); 
		//����������ɾ�� section����������ŷ�Χ�д��󣬷���false������һ����ɾ���ɹ�������true 
		bool del_section(int index); 
 
		void clearAllSection(void); 
		// ��ȡĳ��Section�µ����е�item������Section���֣��Ҳ���item������NULL 
		CItemLine * GetItemPtr(const string & SectionName); 
		// ��ȡĳ��Section�µ����е�item������Section��������ţ�0��ʼ���㣩���Ҳ���item������NULL 
		CItemLine * GetItemPtr(int index); 
		//��ȡĳһ�е����ݣ�����һЩ����������,��ȡ�ɹ�������true��ʧ�ܷ���false 
		bool getSectionName( int index, string & GOT_SectionName ); 
		//��ȡ�����е����� 
		int getSectionCount(void); 
 
	protected: 
		SECTION_LIST sections_;	 
	}; 
}; 
