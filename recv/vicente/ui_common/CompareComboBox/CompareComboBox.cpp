#include "CompareComboBox.h"


CString FirstLetterEx(int nCode)
{
   if(nCode >= 1601 && nCode < 1637) return "A";
   if(nCode >= 1637 && nCode < 1833) return "B";
   if(nCode >= 1833 && nCode < 2078) return "C";
   if(nCode >= 2078 && nCode < 2274) return "D";
   if(nCode >= 2274 && nCode < 2302) return "E";
   if(nCode >= 2302 && nCode < 2433) return "F";
   if(nCode >= 2433 && nCode < 2594) return "G";
   if(nCode >= 2594 && nCode < 2787) return "H";
   if(nCode >= 2787 && nCode < 3106) return "J";
   if(nCode >= 3106 && nCode < 3212) return "K";
   if(nCode >= 3212 && nCode < 3472) return "L";
   if(nCode >= 3472 && nCode < 3635) return "M";
   if(nCode >= 3635 && nCode < 3722) return "N";
   if(nCode >= 3722 && nCode < 3730) return "O";
   if(nCode >= 3730 && nCode < 3858) return "P";
   if(nCode >= 3858 && nCode < 4027) return "Q";
   if(nCode >= 4027 && nCode < 4086) return "R";
   if(nCode >= 4086 && nCode < 4390) return "S";
   if(nCode >= 4390 && nCode < 4558) return "T";
   if(nCode >= 4558 && nCode < 4684) return "W";
   if(nCode >= 4684 && nCode < 4925) return "X";
   if(nCode >= 4925 && nCode < 5249) return "Y";
   if(nCode >= 5249 && nCode < 5590) return "Z";
   if(nCode >= 5601 && nCode <= 8794)
   {
    CString CodeData = "cjwgnspgcenegypbtwxzdxykygtpjnmjqmbsgzscyjsyyfpggbzgydywjkgaljswkbjqhyjwpdzlsgmr";
     CString str1 = "ybywwccgznkydgttngjeyekzydcjnmcylqlypyqbqrpzslwbdgkjfyxjwcltbncxjjjjcxdtqsqzycdxxhgckbphffss";
     CString str2 = "pybgmxjbbyglbhlssmzmpjhsojnghdzcdklgjhsgqzhxqgkezzwymcscjnyetxadzpmdssmzjjqjyzcjjfwqjbdzbjgd";
     CString str3 = "nzcbwhgxhqkmwfbpbqdtjjzkqhylcgxfptyjyyzpsjlfchmqshgmmxsxjpkdcmbbqbefsjwhwwgckpylqbgldlcctnma";
     CString str4 = "eddksjngkcsgxlhzaybdbtsdkdylhgymylcxpycjndqjwxqxfyyfjlejbzrwccqhqcsbzkymgplbmcrqcflnymyqmsqt";
     CString str5 = "rbcjthztqfrxchxmcjcjlxqgjmshzkbswxemdlckfsydsglycjjssjnqbjctyhbftdcyjdgwyghqfrxwckqkxebpdjpx";
     CString str6 = "jqsrmebwgjlbjslyysmdxlclqkxlhtjrjjmbjhxhwywcbhtrxxglhjhfbmgykldyxzpplggpmtcbbajjzyljtyanjgbj";
     CString str7 = "flqgdzyqcaxbkclecjsznslyzhlxlzcghbxzhznytdsbcjkdlzayffydlabbgqszkggldndnyskjshdlxxbcghxyggdj";
     CString str8 = "mmzngmmccgwzszxsjbznmlzdthcqydbdllscddnlkjyhjsycjlkohqasdhnhcsgaehdaashtcplcpqybsdmpjlpcjaql";
     CString str9 = "cdhjjasprchngjnlhlyyqyhwzpnccgwwmzffjqqqqxxaclbhkdjxdgmmydjxzllsygxgkjrywzwyclzmcsjzldbndcfc";
     CString str10 = "xyhlschycjqppqagmnyxpfrkssbjlyxyjjglnscmhcwwmnzjjlhmhchsyppttxrycsxbyhcsmxjsxnbwgpxxtaybgajc";
     CString str11 = "xlypdccwqocwkccsbnhcpdyznbcyytyckskybsqkkytqqxfcwchcwkelcqbsqyjqcclmthsywhmktlkjlychwheqjhtj";
     CString str12 = "hppqpqscfymmcmgbmhglgsllysdllljpchmjhwljcyhzjxhdxjlhxrswlwzjcbxmhzqxsdzpmgfcsglsdymjshxpjxom";
     CString str13 = "yqknmyblrthbcftpmgyxlchlhlzylxgsssscclsldclepbhshxyyfhbmgdfycnjqwlqhjjcywjztejjdhfblqxtqkwhd";
     CString str14 = "chqxagtlxljxmsljhdzkzjecxjcjnmbbjcsfywkbjzghysdcpqyrsljpclpwxsdwejbjcbcnaytmgmbapclyqbclzxcb";
     CString str15 = "nmsggfnzjjbzsfqyndxhpcqkzczwalsbccjxpozgwkybsgxfcfcdkhjbstlqfsgdslqwzkxtmhsbgzhjcrglyjbpmljs";
     CString str16 = "xlcjqqhzmjczydjwbmjklddpmjegxyhylxhlqyqhkycwcjmyhxnatjhyccxzpcqlbzwwwtwbqcmlbmynjcccxbbsnzzl";
     CString str17 = "jpljxyztzlgcldcklyrzzgqtgjhhgjljaxfgfjzslcfdqzlclgjdjcsnclljpjqdcclcjxmyzftsxgcgsbrzxjqqcczh";
     CString str18 = "gyjdjqqlzxjyldlbcyamcstylbdjbyregklzdzhldszchznwczcllwjqjjjkdgjcolbbzppglghtgzcygezmycnqcycy";
     CString str19 = "hbhgxkamtxyxnbskyzzgjzlqjdfcjxdygjqjjpmgwgjjjpkjsbgbmmcjssclpqpdxcdyykypcjddyygywchjrtgcnyql";
     CString str20 = "dkljczzgzccjgdyksgpzmdlcphnjafyzdjcnmwescsglbtzcgmsdllyxqsxsbljsbbsgghfjlwpmzjnlyywdqshzxtyy"; 
     CString str21 = "whmcyhywdbxbtlmswyyfsbjcbdxxlhjhfpsxzqhfzmqcztqcxzxrdkdjhnnyzqqfnqdmmgnydxmjgdhcdycbffallztd";
     CString str22 = "ltfkmxqzdngeqdbdczjdxbzgsqqddjcmbkxffxmkdmcsychzcmljdjynhprsjmkmpcklgdbqtfzswtfgglyplljzhgjj";
     CString str23 = "gypzltcsmcnbtjbhfkdhbyzgkpbbymtdlsxsbnpdkleycjnycdykzddhqgsdzsctarlltkzlgecllkjljjaqnbdggghf";
     CString str24 = "jtzqjsecshalqfmmgjnlyjbbtmlycxdcjpldlpcqdhsycbzsckbzmsljflhrbjsnbrgjhxpdgdjybzgdlgcsezgxlblg";
     CString str25 = "yxtwmabchecmwyjyzlljjshlgndjlslygkdzpzxjyyzlpcxszfgwyydlyhcljscmbjhblyjlycblydpdqysxktbytdkd";
     CString str26 = "xjypcnrjmfdjgklccjbctbjddbblblcdqrppxjcglzcshltoljnmdddlngkaqakgjgyhheznmshrphqqjchgmfprxcjg";
     CString str27 = "dychghlyrzqlcngjnzsqdkqjymszswlcfqjqxgbggxmdjwlmcrnfkkfsyyljbmqammmycctbshcptxxzzsmphfshmclm";
     CString str28 = "ldjfyqxsdyjdjjzzhqpdszglssjbckbxyqzjsgpsxjzqznqtbdkwxjkhhgflbcsmdldgdzdblzkycqnncsybzbfglzzx"; 
     CString str29 = "swmsccmqnjqsbdqsjtxxmbldxcclzshzcxrqjgjylxzfjphymzqqydfqjjlcznzjcdgzygcdxmzysctlkphtxhtlbjxj";
     CString str30 = "lxscdqccbbqjfqzfsltjbtkqbsxjjljchczdbzjdczjccprnlqcgpfczlclcxzdmxmphgsgzgszzqjxlwtjpfsyaslcj";
     CString str31 = "btckwcwmytcsjjljcqlwzmalbxyfbpnlschtgjwejjxxglljstgshjqlzfkcgnndszfdeqfhbsaqdgylbxmmygszldyd";
     CString str32 = "jmjjrgbjgkgdhgkblgkbdmbylxwcxyttybkmrjjzxqjbhlmhmjjzmqasldcyxyqdlqcafywyxqhz";
	 CodeData += str1;
	 CodeData += str2;
	 CodeData += str3;
	 CodeData += str4;
	 CodeData += str5;
	 CodeData += str6;
	 CodeData += str7;
	 CodeData += str8;
	 CodeData += str9;
	 CodeData += str10;
	 CodeData += str11;
	 CodeData += str12;
	 CodeData += str13;
	 CodeData += str14;
	 CodeData += str15;
	 CodeData += str16;
	 CodeData += str17;
	 CodeData += str18;
	 CodeData += str19;
	 CodeData += str20;
	 CodeData += str21;
	 CodeData += str22;
	 CodeData += str23;
	 CodeData += str24;
	 CodeData += str25;
	 CodeData += str26;
	 CodeData += str27;
	 CodeData += str28;
	 CodeData += str29;
	 CodeData += str30;
	 CodeData += str31;
	 CodeData += str32;

    CString gbcode;
    gbcode.Format("%d", nCode);
	int pos1 = atoi(gbcode.Mid(0,2));
	int pos2 = atoi(gbcode.Mid(gbcode.GetLength()-2,2));
    int pos = (pos1-56)*94 + pos2;
    CString strResult = CodeData.Mid(pos-1,1);
    strResult.MakeUpper();
    return strResult;
   } 
   return "";
}


void GetFirstLetterEx(CString strName, CString& strFirstLetter)
{
    TBYTE ucHigh, ucLow;
    int  nCode;
    CString strRet;
	
    strFirstLetter.Empty();
	
    for (int i=0; i<strName.GetLength(); i++)
    {
        if ( (TBYTE)strName[i] < 0x80 )
            continue;
		
        ucHigh = (TBYTE)strName[i];
		if( i+1 >= strName.GetLength() )
			break;
        ucLow  = (TBYTE)strName[i+1];
        if ( ucHigh < 0xa1 || ucLow < 0xa1)
            continue;
        else
            nCode = (ucHigh - 0xa0) * 100 + ucLow - 0xa0;
		
        strRet = FirstLetterEx(nCode);
        strFirstLetter += strRet;
        i++;
    }
}

BOOL IsChinese( CString str )
{
	TBYTE ucHigh, ucLow;
	
    if ( (TBYTE)str[0] < 0x80 )
		return FALSE;
    ucHigh = (TBYTE)str[0];
    ucLow  = (TBYTE)str[1];
    if ( ucHigh < 0xa1 || ucLow < 0xa1)
		return FALSE;
    return TRUE;
}

IMPLEMENT_DYNCREATE( CCompareCmb, CComboBox)

CCompareCmb::CCompareCmb()
{
	m_bAutoSelect = TRUE;
	m_nSelectIndex = -1;
}

CCompareCmb::~CCompareCmb()
{
}


BEGIN_MESSAGE_MAP(CCompareCmb, CComboBox)
//{{AFX_MSG_MAP(CCompareCmb)
	ON_CONTROL_REFLECT(CBN_EDITCHANGE, OnEditchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCompareCmb::OnEditchange() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	//清空
	int count = GetCount();
	for (int i=0;i < count; i++)
	{
		DeleteString( 0 );
	}

	CString str;
	GetWindowText(str);

	if(str == "")
	{
		ReFillAllData();
		return;
	}
	
	if(IsChinese(str)) //输入为中文,直接比较
	{
		int length = str.GetLength();
		
		POSITION pos = m_datas.GetHeadPosition();
		while(pos != NULL)
		{
			CMBDATA cmbdata = m_datas.GetNext(pos);
			
			if(cmbdata.name.GetLength() < length)
				continue;
			CString strTemp = cmbdata.name;
			for (int i=0; i<strTemp.GetLength(); i++)
			{
				int a = (TBYTE)strTemp[i];
				if ( !IsChinese(strTemp) )
					strTemp = strTemp.Right(strTemp.GetLength() - 1);
				else
					break;
			}
			if(strTemp.GetLength() < length)
				continue;
				
			if(strTemp.Find(str, 0) != -1)
			{			
				int Index = AddString(cmbdata.name);
				//int Index = FindString(0,cmbdata.name);
				if(Index != -1)
				{
					SetItemDataPtr(Index, (void*)cmbdata.data);
				}
			}
		}
	}
	else
	{
		//拼音
		int length = str.GetLength();
		str.MakeUpper();
		
		POSITION pos = m_datas.GetHeadPosition();
		while(pos != NULL)
		{
			CMBDATA cmbdata = m_datas.GetNext(pos);

			if(cmbdata.name.GetLength() < length)
				continue;
			
			if(cmbdata.letter.Find(str, 0) != -1)
			{
				//加入到下拉框				
				int Index = AddString(cmbdata.name);
				//int Index = FindString(0,cmbdata.name);
				if(Index != -1)
				{
					SetItemDataPtr(Index, (void*)cmbdata.data);
				}
			}
		}
	}
	
	::SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW)));
	ShowDropDown(TRUE);
}

/****************************************************
Date:2014/5/14  Author:LYH
函数名:   OnDataFilled	
返回值:   void	
功能概要: 
*****************************************************/
void CCompareCmb::OnDataFilled(BOOL bAutoSelect /*= TRUE*/, int index /*= -1*/)
{
	m_datas.RemoveAll();
	for( int i = 0; i < GetCount(); i++)
	{
		CMBDATA cmbdata;
		GetLBText(i, cmbdata.name);
		cmbdata.data = GetItemData(i);
		GetFirstLetterEx(cmbdata.name, cmbdata.letter);
		m_datas.AddTail(cmbdata);
	}
	m_bAutoSelect = bAutoSelect;
	m_nSelectIndex = index;
}

/****************************************************
Date:2014/5/14  Author:LYH
函数名:   ReFillAllData	
返回值:   void	
功能概要: 
*****************************************************/
void CCompareCmb::ReFillAllData()
{
	POSITION pos = m_datas.GetHeadPosition();
	while(pos != NULL)
	{
		CMBDATA cmbdata = m_datas.GetNext(pos);
		int Index = AddString(cmbdata.name);
		//int Index = FindString(0,cmbdata.name);
		if(Index != -1)
		{
			SetItemDataPtr(Index, (void*)cmbdata.data);
		}
	}
	if(m_bAutoSelect)
	{
		if(m_nSelectIndex == -1)
		{
			SetCurSel( GetCount() -1 );
		}
		else
		{
			if( m_nSelectIndex >= GetCount() )
			{
				SetCurSel( GetCount() -1 );
			}
			else
				SetCurSel( m_nSelectIndex );
		}
		if( GetParent() )
			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELCHANGE), LPARAM((HWND)GetSafeHwnd()));
	}
}
