#include "pch.h"
#include "INI.h"  
#include <sys/stat.h>
   
CINI::CINI()
{

}

CINI::CINI( const char * FileName )   
{   
    if( FileName == NULL )   
    {   
        throw( "config file can't be found." );   
    }   
   
    if( strlen( FileName ) > MAXFILENAME )   
    {   
        throw( "config file name is too long." );   
    }   
   
    lineEnd.resize( DEFAULT_LINE );   
    //headLine.reserve( DEFAULT_LINE );    
    _fileName = FileName;   
    LoadFileIntoMen( FileName );   
    bModified_ = false;   
}   
   
CINI::~CINI(void)   
{   
    writeList();   
}   
   
long CINI::GetFileLength( const char * FileName )   
{   
    FILE * f;
	fopen_s( &f,FileName, "r" );

    if( f == NULL )   
    { //��ȡ�ļ�ʧ��    
        return -1;   
    }   
   
    long fl = -1; // Length of file    
    struct stat  buf;   
    int fileno;   
    fileno = _fileno( f );   
    if ( fstat( fileno, & buf ) != 0 )   
    {   
        fclose( f );   
        return -1;   
    }   
    fclose( f );   
    fl = (long)buf.st_size;   
   
    return fl;   
}   
   
   
// ���ļ������ڴ�    
int CINI::LoadFileIntoMen(const char * FileName)   
{   
    long FileLength = GetFileLength( FileName );   
    if( FileLength <= 0 )   
    {   
        return 0; //����ļ����ȵ���0�����ļ����������ڣ�ֱ�ӷ��سɹ����ļ������ڣ�����ᴴ���ļ���    
    }   
   
    FILE * f;
	fopen_s( &f, FileName, "r" );   
    if( f == NULL )   
    { //��ȡ�ļ�ʧ��    
        return -1;   
    }   
   
    char * _buf;   
    try   
    {   
        _buf = new char [ FileLength + 1 ];   
        if( _buf == NULL )   
        {   
            return -2; //�ڴ治��    
        }   
    }catch(...){ return -2 ; } //�ڴ治��    
    auto_ptr<CHAR> FileBuf( _buf );   
    _buf [ FileLength  ] = '\0';   
    size_t s_read = fread( _buf, sizeof(char), FileLength , f );   
    _buf[ s_read ] = '\0';   
   
    fclose ( f );   
    //�ؽ����µĻ��з�    
    RebuildLineBreak( _buf, "\n", s_read );   
   
   
    //�����¶����ڴ� section     
    size_t i;   
    bool bHeadLine = true;  //һ��ʼ��ȥʶ�� headLine    
    string strTemp, strLine;   
    CItemLine * item = NULL;   
    for( i = 0; i < lineEnd.size(); i++ )   
    {   
        const char * line = ReadLine( i, _buf );   
        if( line == NULL )   
        {   
            continue;   
        }   
        strLine = line;   
        if( bHeadLine )   
        {   
            if( this->isSection( line ) )   
            {   
                item = this->section.add_line( strLine );   
                if( item == NULL ) //����һ�гɹ���˵����һ�п�������ͨ�� head line    
                {   
                    this->headLine.push_back( strLine );   
                    continue;   
                }   
                else   
                { //���� section �ɹ��� ��ʽ����section ��Χ    
                    bHeadLine = false;   
                }   
            }   
            else   
            {   
                this->headLine.push_back( strLine );   
                continue;   
            }   
        }   
        else   
        {              
            if( this->isSection( line ) )   
            {   
                CItemLine * item2 = NULL;   
                item2 = this->section.add_line( line );   
                if( item2 == NULL )   
                {   
                    if( item == NULL )   
                    {   
                        continue;   
                    }   
                    item->add_line( strLine );   
                }   
                else   
                {   
                    item = item2;   
                }   
            }   
            else   
            {   
                item->add_line( strLine );   
            }   
        }   
    } //�����ڴ����    
    return 0;   
}   
   
// ���������ݹ������з�, lineBreak �ǻ��з�    
int CINI::RebuildLineBreak(char * strContent, const char * lineBreak, size_t filesize )   
{   
    char * p = NULL;   
    lineEnd.clear();   
    size_t lenOfLineBreak = strlen( lineBreak );   
   
    lineEnd.push_back( 0 );   
   
    p = strContent;   
    while( p != NULL )   
    {   
        p = strstr( p, lineBreak );   
        if( p == NULL )   
        {   
            break;   
        }   
        size_t i;   
        for( i = 0; i < lenOfLineBreak; i ++ )   
        {   
            *p = '\0';   
            ++p;   
        }          
        size_t  offset = (size_t)( p - strContent );   
        if( offset >= filesize )   
        {   
            break;   
        }   
        lineEnd.push_back( (long) offset );        
    }   
    return 0;   
}   
   
// ��ȡĳһ�У�lineNumber���к�,��ȡ�ɹ���������һ�е���ʼ�ַ��ĵ�ַ    
const char * CINI::ReadLine( size_t lineNumber, char * buf )   
{   
    if( lineNumber > lineEnd.size()  || lineNumber < 0 )   
    {   
        return NULL;   
    }   
   
    if( lineNumber == 0 )   
    {   
        return buf;   
    }   
   
    size_t LineStartPos = (size_t)( lineEnd[ lineNumber ] );   
    char *p = buf + LineStartPos;   
    return p;   
}   
   
//д�������ռ����������һ�������� string ���͵�value��    
void CINI::WriteValueCommon( const string & Section, const string & Item, const string & value )   
{   
    CItemLine * item = NULL;   
    item = section.GetItemPtr( Section );   
    if( item == NULL )   
    { //û�����section         
        item = section.add_line( Section, false );   
    }   
    item->add_line( Item, value );    
    bModified_ = true;   
}   
//���������ռ����������һ�������� string & ���͵�value�����ֵ�Ƕ�����ֵ��bool ���͵ĺ������ر�־��ʾ��������û�ж�����    
bool CINI::ReadValueCommon( const string & Section, const string & Item, string & value )   
{   
    CItemLine * item = section.GetItemPtr( Section );   
    if( item == NULL )   
    {   
        return false;   
    }   
   
    if( ! item->getValue( Item, value ) )   
    {   
        return false;   
    }   
    trim( value );   
    return true;   
}   
   
int CINI::ReadInt( const string & Section, const string & Item, const int DefaultValue )   
{   
    int i;   
    string str;   
    if( ReadValueCommon( Section, Item, str ) )   
    {   
        trim( str );   
        if( isDigital( str ) )   
        {   
            i = atoi( str.c_str() );   
        }   
        else   
        {   
            i = DefaultValue;   
        }   
    }   
    else   
    {   
        i = DefaultValue;   
    }   
    return i;   
}   
   
long CINI::ReadLong( const string & Section, const string & Item, const long & DefaultValue )   
{   
    long i;   
    string str;   
    if( ReadValueCommon( Section, Item, str ) )   
    {   
        trim( str );   
        if( isDigital( str ) )   
        {   
            i = atol( str.c_str() );   
        }   
        else   
        {   
            i = DefaultValue;   
        }   
    }   
    else   
    {   
        i = DefaultValue;   
    }   
    return i;   
}   
   
double CINI::ReadFloat( const string & Section, const string & Item, const double & DefaultValue )   
{   
    double i;   
    string str;   
    if( ReadValueCommon( Section, Item, str ) )   
    {   
        trim( str );   
        if( isDigital( str ) )   
        {   
            i = atof( str.c_str() );   
        }   
        else   
        {   
            i = DefaultValue;   
        }   
    }   
    else   
    {   
        i = DefaultValue;   
    }   
    return i;   
}   
   
string  CINI::ReadString( const string & Section, const string & Item, const string & DefaultValue )   
{   
    string i;   
    string str;   
    if( ReadValueCommon( Section, Item, str ) )   
    {   
        trim( str );   
        if( (int)str.length() == 0 )   
        {   
            i = DefaultValue;   
        }   
        else   
        {   
            i = str;   
        }   
    }   
    else   
    {   
        i = DefaultValue;   
    }   
    return i;   
}   
   
   
bool CINI::ReadBool( const string & Section, const string & Item, const bool DefaultValue )   
{   
    bool i;   
    string str;   
    if( ReadValueCommon( Section, Item, str ) )   
    {   
        trim( str );   
        int boolValue;   
        boolValue = isBool( str );   
        if( boolValue == 0 )   
        {   
            i = true;   
        }   
        else if ( boolValue == 1 )   
        {   
            i = false;   
        }   
        else   
        {   
            i = DefaultValue;   
        }   
    }   
    else   
    {   
        i = DefaultValue;   
    }   
    return i;   
}   
   
   
void CINI::WriteInt( const string & Section, const string & Item, const int i )   
{   
    string strTemp;   
    char cTemp[ 256 ];   
    cTemp[255] = '\0';   
    sprintf_s( cTemp, "%d", i );   
    strTemp = cTemp;   
    WriteValueCommon( Section, Item, strTemp );   
    return;   
}   
   
void CINI::WriteLong( const string & Section, const string & Item, const long & i )   
{   
    string strTemp;   
    char cTemp[ 256 ];   
    cTemp[255] = '\0';   
    sprintf_s( cTemp, "%ld", i );   
    strTemp = cTemp;   
    WriteValueCommon( Section, Item, strTemp );   
    return;   
}   
   
void CINI::WriteFloat( const string & Section, const string & Item, const double & i )   
{   
    string strTemp;   
    char cTemp[ 256 ];   
    cTemp[255] = '\0';   
    sprintf_s( cTemp, "%lf", i );   
    strTemp = cTemp;   
    WriteValueCommon( Section, Item, strTemp );   
    return;   
}   
   
   
void CINI::WriteString( const string & Section, const string & Item, const string & i )   
{   
    WriteValueCommon( Section, Item, i );   
    return;   
}   
   
   
void CINI::WriteBool( const string & Section, const string & Item, const bool i )   
{   
    string strTemp;   
    if( i )   
    {   
        strTemp = "True";   
    }   
    else   
    {   
        strTemp = "False";   
    }   
    WriteValueCommon( Section, Item, strTemp );   
    return;   
}   
// ��ini��Ϣд���ļ�    
void CINI::writeList(void)   
{   
    if( ! bModified_ )   
    { //ini�ļ�û���κ�д���������д���ļ�    
        return;   
    }   
    FILE *p;   
    fopen_s( &p, this->_fileName.c_str(), "w" );   
    if( !p )   
    {   
        p = NULL;   
        throw( "couldn't write file" );   
    }   
    int i;   
    string sTemp;   
   
    int lenOfheadLine = (int)headLine.size();   
    int lenOfSection  = section.getSectionCount();   
   
    //��д headline    
    if( lenOfSection == 0 )   
    {   
        for( i = 0; i < lenOfheadLine; i ++ )   
        {   
            sTemp = headLine[i];   
            if( i == (lenOfheadLine - 1) )   
            {                  
                fprintf( p, "%s", sTemp.c_str() );   
            }   
            else   
            {   
                fprintf( p, "%s\n", sTemp.c_str() );   
            }   
        }   
    }   
    else   
    {   
        for( i = 0; i < lenOfheadLine; i ++ )   
        {   
            sTemp = headLine[i];   
            fprintf( p, "%s\n", sTemp.c_str() );   
        }   
    }   
   
   
    //��д section    
    for( i = 0; i < lenOfSection; i++ )   
    {   
        if( ! section.getSectionName( i, sTemp ) )   
        {   
            continue;   
        }   
        CItemLine * item = section.GetItemPtr( i );   
        if( item == NULL )   
        {   
            continue;   
        }   
        if( i == (lenOfSection-1)  &&  item == NULL )   
        {   
            fprintf( p, "[%s]", sTemp.c_str() );   
        }   
        else   
        {   
            fprintf( p, "[%s]\n", sTemp.c_str() );   
            int j;   
            for( j = 0; j < item->getAllLineCount(); j ++ )   
            {   
                sTemp = item->getLine( j );   
                if( i == (lenOfSection-1)  &&  j == (item->getAllLineCount() - 1) )   
                {   
                    fprintf( p, "%s", sTemp.c_str() );   
                }   
                else   
                {   
                    fprintf( p, "%s\n", sTemp.c_str() );   
                }   
            }   
        }   
    }   
    fclose( p );   
    bModified_ = false;   
}   
   
void CINI::WriteFileNow()   
{   
    writeList();   
}   
   
// ��ȡһ���ж��ٸ� section    
int CINI::GetSectionCount(void)   
{   
    return this->section.getSectionCount();     
}   
   
// ��ȡ��index��section�����֡�index��0��ʼ���㡣�����ȡ�ɹ�������true��ͬʱ section_name ������ֵΪȡ��������    
bool CINI::GetSectionName(int index, string & section_name)   
{   
    return this->section.getSectionName( index, section_name );   
}   
   
// ��ȡ�� index  �� section ������ж���item,ȡ��������-1    
int CINI::GetItemCount(int index_of_Section)   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( index_of_Section );   
    if( item == NULL )   
    {   
        return -1;   
    }   
    return item->getItemCount();    
}   
   
// ��ȡ section_name ������ж���item,ȡ��������-1    
int CINI::GetItemCount( const string & section_name )   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( section_name );   
    if( item == NULL )   
    {   
        return -1;   
    }   
    return item->getItemCount();   
}   
   
// �ӵ� index_of_Section �� section ���ڶ��item��ȡ���� index_of_Item �� Item ������    
bool CINI::GetItemName(int index_of_Section, int index_of_Item,  string & item_value )   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( index_of_Section );   
    if( item == NULL )   
    {   
        return false;   
    }   
    return item->getItemName( index_of_Item, item_value );      
}   
// ��ȡ section_name ��� index_of_Item �� Item ������    
bool CINI::GetItemName( const string & section_name, int index_of_Item,  string & item_value )   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( section_name );   
    if( item == NULL )   
    {   
        return false;   
    }   
    return item->getItemName( index_of_Item, item_value );      
}   
   
// ��ȡ item ��ֵ    
bool CINI::GetItemValue(int index_of_Section, int index_of_Item, string & value)   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( index_of_Section );   
    if( item == NULL )   
    {   
        return false;   
    }   
    return item->getValue( index_of_Item, value );   
}   
   
bool CINI::GetItemValue(const string & section_name, int index_of_Item, string & value)   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( section_name );   
    if( item == NULL )   
    {   
        return false;   
    }   
    return item->getValue( index_of_Item, value );   
}   
bool CINI::GetItemValue(int index_of_Section, const string & item_name, string & value)   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( index_of_Section );   
    if( item == NULL )   
    {   
        return false;   
    }   
    return item->getValue( item_name, value );   
}   
bool CINI::GetItemValue(const string & section_name, const string & item_name, string & value)   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( section_name );   
    if( item == NULL )   
    {   
        return false;   
    }   
    return item->getValue( item_name, value );   
}   
// ɾ���� index ��section    
void CINI::DelSection(int index)   
{   
    if( this->section.del_section( index ) )   
    {   
        bModified_ = true;   
    }   
}   
// ɾ����Ϊ section_name ��section    
void CINI::DelSection( const string & section_name )   
{   
    if( this->section.del_section( section_name ) )   
    {   
        bModified_ = true;   
    }   
}   
   
// ɾ��ȫ��section    
void CINI::clearSection(void)   
{   
    if( this->section.getSectionCount() == 0 )   
    {   
        return ;   
    }   
    this->section.clearAllSection();   
    bModified_ = true;   
}   
   
// ɾ��item    
void CINI::DelItem(int index_of_section, int index_of_item)   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( index_of_section );   
    if( item == NULL )   
    {   
        return;   
    }   
    if( item->del_Item( index_of_item ) )   
        bModified_ = true;   
}   
   
void CINI::DelItem( const string & section_name, int index_of_item)   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( section_name );   
    if( item == NULL )   
    {   
        return;   
    }   
    if( item->del_Item( index_of_item ) )   
        bModified_ = true;   
}   
void CINI::DelItem(int index_of_section, const string & item_name )   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( index_of_section );   
    if( item == NULL )   
    {   
        return;   
    }   
    if( item->del_Item( item_name ) )   
        bModified_ = true;   
}   
void CINI::DelItem( const string & section_name, const string & item_name )   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( section_name );   
    if( item == NULL )   
    {   
        return;   
    }   
    if( item->del_Item( item_name ) )   
        bModified_ = true;   
}   
   
// ɾ��ĳ��section�����е�item    
void CINI::clearItem(int index_of_section, bool bIncludeOtherLine )   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( index_of_section );   
    if( item == NULL )   
    {   
        return;   
    }   
    item->clearAllItem( bIncludeOtherLine );   
    bModified_ = true;   
}   
   
// ɾ��ĳ��section�����е�item    
//������ bIncludeOtherLine Ϊtrueʱ����Section�����е��У�����item�ĺ��������ֵģ�����ɾ����    
//������ bIncludeOtherLine Ϊ false ʱ��ֻɾ�� item �ġ��������ֱ���    
void CINI::clearItem( const string & section_name, bool bIncludeOtherLine )   
{   
    CItemLine * item;   
    item = this->section.GetItemPtr( section_name );   
    if( item == NULL )   
    {   
        return;   
    }   
    item->clearAllItem( bIncludeOtherLine );   
    bModified_ = true;   
}  