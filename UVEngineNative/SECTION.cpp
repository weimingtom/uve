#include "pch.h"
#include "SECTION.h"     
   
using namespace DCG__INI__FILE;   
   
   
CSECTION::CSECTION(void)   
{   
    sections_.clear();     
}   
   
CSECTION::~CSECTION(void)   
{   
}   
   
bool DCG__INI__FILE::CSECTION::get_SectionName(const  string & Line, string & section )   
{   
    section = "";   
    basic_string <CHAR>::size_type i1, i2;   
    i1 = Line.find( "[" );   
    if( i1 == -1 )   
    {   
        return false;   
    }   
    i2 = Line.find( "]" );   
    if( i2 == -1 )   
    {   
        return false;   
    }   
    if( i1 >= i2 )   
    {   
        return false;   
    }   
    section.append( Line, i1 + 1, i2 - i1 - 1 );   
    return true;   
}   
   
//����һ�У���һ�е������� Line ����    
//���� bIsSectionFormat Ϊ false ʱ������� Line ������ [ Section ] �ĸ�ʽ�Żᱻ�����������ĸ�ʽ���ԣ��᷵��NULL    
//���� bIsSectionFormat Ϊ true  ʱ������� Line ���벻���� [ ] ��Ȼ��ᱻ����һ�� Section ��������������� [ ]���᷵�� NULL    
CItemLine * DCG__INI__FILE::CSECTION::add_line(const string & Line, bool bIsSectionFormat)   
{   
    if( ! bIsSectionFormat )   
    {   
        if( this->isContainBracket( Line ) )   
        {   
            return NULL;   
        }   
    }   
    else   
    {   
        if( ! this->isSection( Line.c_str() ) )   
        {   
            return NULL;   
        }   
    }   
    string section;   
    if( bIsSectionFormat )   
    {   
        if( ! this->get_SectionName( Line, section ) )   
        {   
            return NULL;   
        }   
    }   
    else   
    {   
        section = Line;   
    }   
   
       
    ONE_SECTION a_section;   
    a_section.section = section;   
    trim( section );   
    a_section.trimedSection = section;   
    sections_.push_back( a_section );   
    SECTION_LIST::const_reference cr = sections_.back();   
	SECTION_LIST::reference f = const_cast<SECTION_LIST::reference> (cr);   
    return &(f.items);   
}   
   
//����������ɾ�� section������������Ҳ���������false������һ����ɾ���ɹ�������true    
bool DCG__INI__FILE::CSECTION::del_section(const string & ItemName)   
{   
    string str = ItemName;   
    trim( str );   
    SECTION_LIST::iterator it;   
    for( it = sections_.begin(); it != sections_.end(); it++ )   
    {   
        if( (*it).trimedSection == str )   
        {   
            sections_.erase( it );   
            return true;   
        }   
    }   
    return false;   
}   
   
   
//����������ɾ�� section����������ŷ�Χ�д��󣬷���false������һ����ɾ���ɹ�������true    
bool DCG__INI__FILE::CSECTION::del_section(int index)   
{   
    if( index < 0 || index >= (int)(sections_.size()) )   
    {   
        return false;   
    }   
    int i = 0;   
    SECTION_LIST::iterator it;   
    for( it = sections_.begin(); it != sections_.end(); it++, i++ )   
    {   
        if( i == index )               
        {   
            sections_.erase( it );   
            return true;   
        }              
    }   
    return false;   
}   
   
   
int DCG__INI__FILE::CSECTION::getSectionCount(void)   
{   
    return (int)(sections_.size());   
}   
   
void DCG__INI__FILE::CSECTION::clearAllSection(void)   
{   
    sections_.clear();   
}   
   
// ��ȡĳ��Section�µ����е�item������Section���֣��Ҳ���item������NULL    
CItemLine * DCG__INI__FILE::CSECTION::GetItemPtr(const string & SectionName)   
{   
    string str = SectionName;   
    trim( str );   
    SECTION_LIST::iterator it;   
    for( it = sections_.begin(); it != sections_.end(); it++ )   
    {   
        if(  (*it).trimedSection == str  )   
        {   
            return &((*it).items);   
        }   
    }   
    return NULL;   
}   
   
// ��ȡĳ��Section�µ����е�item������Section��������ţ�0��ʼ���㣩���Ҳ���item������NULL    
CItemLine * DCG__INI__FILE::CSECTION::GetItemPtr(int index)   
{   
    if( index < 0 || index >= (int)(sections_.size()) )   
    {   
        return NULL;   
    }   
    int i = 0;   
    SECTION_LIST::iterator it;   
    for( it = sections_.begin(); it != sections_.end(); it++, i++ )   
    {   
        if(  i == index  )   
        {   
            return &((*it).items);   
        }   
    }   
    return NULL;   
}   
   
//��ȡĳһ�е����ݣ�����һЩ����������,��ȡ�ɹ�������true��ʧ�ܷ���false    
bool DCG__INI__FILE::CSECTION::getSectionName( int index, string & GOT_SectionName )   
{   
    GOT_SectionName = "";   
    if( index < 0 || index >= (int)sections_.size() )   
    {   
        return false;   
    }   
    int i = 0;   
    SECTION_LIST::iterator it;   
    for( it = sections_.begin(); it != sections_.end(); it ++,  i++ )   
    {   
        if( i == index )   
        {   
            GOT_SectionName = (*it).section;   
            return true;   
        }   
    }   
    return false;   
}