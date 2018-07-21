#ifndef _CONSTANT_H_
#define _CONSTANT_H_

/////////////////////////////�������Ͷ���//////////////////////////////////
/** @brief           �ַ���*/
const int DataType_String	= 0;
/** @brief           ö��*/
const int DataType_Enum		= 1;
/** @brief           ��ɫ*/
const int DataType_Color	= 2;
/** @brief           ����*/
const int DataType_HRef		= 3;
///////////////////////////�������Ͷ���END/////////////////////////////////

//////////////////////////////Ԫ�����Ͷ���/////////////////////////////////
/** @brief           δ֧��*/
const int ElementType_Unsupported		= 0;
/** @brief           SvgԪ�أ���Ԫ��*/
const int ElementType_Svg				= 1;
/** @brief           Բ��*/
const int ElementType_Circle			= 2;
/** @brief           ��Բ*/
const int ElementType_Ellipse			= 3;
/** @brief           ֱ��*/
const int ElementType_Line				= 4;
/** @brief           ����*/
const int ElementType_Polyline			= 5;
/** @brief           �����*/
const int ElementType_Polygon			= 6;
/** @brief           Բ��*/
const int ElementType_Path				= 7;
/** @brief           ����*/
const int ElementType_Rect				= 8;
/** @brief           �ı�*/
const int ElementType_Text				= 9;
/** @brief           ����*/
const int ElementType_Desc				= 10;
/** @brief           ���*/
const int ElementType_Group				= 11;
/** @brief           ͼƬ*/
const int ElementType_Image				= 12;
/** @brief           Ԥ��������*/
const int ElementType_Defs				= 13;
/** @brief           Ԥ����ʵ��*/
const int ElementType_Symbol			= 14;
/** @brief           ����*/
const int ElementType_Use				= 15;
/** @brief           Ԫ����*/
const int ElementType_Metadata			= 16;
/** @brief           ���*/
const int ElementType_Style				= 17;
/** @brief           �豸����*/
const int ElementType_PsrRef			= 18;
/** @brief           ������*/
const int ElementType_Link				= 19;
/** @brief           �豸��ʶ*/
const int ElementType_ObjRef			= 20;
/** @brief           �豸����*/
const int ElementType_CimClass			= 21;
////////////////////////////Ԫ�����Ͷ���END////////////////////////////////

//////////////////////////////Ԫ�����ƶ���/////////////////////////////////
/** @brief           δ֧��*/
#define	 ElementName_Unsupported		"Unsupported"
/** @brief           SvgԪ�أ���Ԫ��*/
#define  ElementName_Svg			"svg"
/** @brief           Բ��*/
#define  ElementName_Circle			"circle"
/** @brief           ��Բ*/
#define  ElementName_Ellipse		"ellipse"
/** @brief           ֱ��*/
#define  ElementName_Line			"line"
/** @brief           ����*/
#define  ElementName_Polyline		"polyline"
/** @brief           �����*/
#define  ElementName_Polygon		"polygon"
/** @brief           Բ��*/
#define  ElementName_Path			"path"
/** @brief           ����*/
#define  ElementName_Rect			"rect"
/** @brief           �ı�*/
#define  ElementName_Text			"text"
/** @brief           ����*/
#define  ElementName_Desc			"desc"
/** @brief           ���*/
#define  ElementName_Group			"g"
/** @brief           ͼƬ*/
#define  ElementName_Image			"image"
/** @brief           Ԥ��������*/
#define	 ElementName_Defs			"defs"
/** @brief           Ԥ����ʵ��*/
#define  ElementName_Symbol			"symbol"
/** @brief           ����*/
#define  ElementName_Use			"use"
/** @brief           Ԫ����*/
#define  ElementName_Metadata		"metadata"
/** @brief           ���*/
#define  ElementName_Style			"style"
/** @brief           �豸����*/
#define  ElementName_PsrRef			"cge:PSR_Ref"
/** @brief           ������*/
#define  ElementName_Link			"a"
/** @brief           �豸��ʶ*/
#define  ElementName_ObjRef			"PSR:ObjRef"
/** @brief           �豸����*/
#define  ElementName_CimClass		"PSR:CimClass"
////////////////////////////Ԫ�����ƶ���END////////////////////////////////

///////////////////////////����ֵ��Դ����//////////////////////////////////
/** @brief           ԭʼֵ�����ļ��ж�ȡ���û�����*/
const int ValueSrc_Original		= 0;
/** @brief           �ɸ��ڵ�̳ж���*/
const int ValueSrc_Inherit		= 1;
/** @brief           �����ýڵ��ֵ����*/
const int ValueSrc_Cover		= 2;
/** @brief           ��CSS��ʽȡ��*/
const int ValueSrc_CSS			= 3;

//////////////////////////����ֵ��Դ����END////////////////////////////////

/////////////////////////////�����鶨��////////////////////////////////////
/** @brief           �����飺δ֪*/
const int AttrGroup_UnKnown		= 0;
/** @brief           �����飺����*/
const int AttrGroup_Core		= 1;
/** @brief           �����飺Ԫ��ָ��*/
const int AttrGroup_Specific	= 2;
/** @brief           �����飺����*/
const int AttrGroup_Paint		= 3;
/** @brief           �����飺ͼ��*/
const int AttrGroup_Graphics	= 4;
/** @brief           �����飺���*/
const int AttrGroup_Style		= 5;
/** @brief           �����飺����*/
const int AttrGroup_Font		= 6;
/** @brief           �����飺����*/
const int AttrGroup_Link		= 7;
/** @brief           �����飺���ݹ���*/
const int AttrGroup_Psr			= 8;
///////////////////////////�����鶨��END///////////////////////////////////

//////////////////////////////�������Ͷ���/////////////////////////////////////
const int attr_Unknown				= 0;
const int attr_DevType				= 999; //�豸���ͣ����ⶨ��
const int attrEx_IsFill				= 998; //�Ƿ����

const int attrCore_Id				= 1;
const int attrCore_xmlNs			= 2;
const int attrCore_xmlNsXlink		= 3;
const int attrCore_xmlNsPsr			= 4;
const int attrCore_xmlNsCge			= 5;
const int attrCore_xmlBase			= 6;
const int attrCore_xmlLang			= 7;
const int attrCore_xmlSpace			= 8;
const int attrCore_Version			= 9;
const int attrCore_uri				= 10;

const int attrSpecific_X			= 20;
const int attrSpecific_Y			= 21;
const int attrSpecific_CX			= 22;
const int attrSpecific_CY			= 23;
const int attrSpecific_Width		= 24;
const int attrSpecific_Height		= 25;
const int attrSpecific_R			= 26;
const int attrSpecific_RX			= 27;
const int attrSpecific_RY			= 28;
const int attrSpecific_DX			= 29;
const int attrSpecific_DY			= 30;
const int attrSpecific_X1			= 31;
const int attrSpecific_Y1			= 32;
const int attrSpecific_X2			= 33;
const int attrSpecific_Y2			= 34;
const int attrSpecific_Points		= 35;
const int attrSpecific_PathData		= 36;
const int attrSpecific_PathLength	= 37;
const int attrSpecific_TextLength	= 38;
const int attrSpecific_LengthAdjust	= 39;

const int attrPaint_Color			= 70;
const int attrPaint_Fill			= 71;
const int attrPaint_FillRule		= 72;
const int attrPaint_FillOpacity		= 73;
const int attrPaint_Stroke			= 74;
const int attrPaint_StrokeWidth		= 75;
const int attrPaint_StrokeLineCap	= 76;
const int attrPaint_StrokeLineJoin	= 77;
const int attrPaint_StrokeMiterLimit= 78;
const int attrPaint_StrokeDashArray	= 79;
const int attrPaint_StrokeDashOffset= 80;
const int attrPaint_StrokeOpacity	= 81;

const int attrGraphics_Display		= 100;
const int attrGraphics_Visibility	= 101;
const int attrGraphics_Opacity		= 102;
const int attrGraphics_Transform	= 103;
const int attrGraphics_ViewBox		= 104;
const int attrGraphics_preserveAspectRatio	= 105;
const int attrGraphics_ImageRendering	= 106;
const int attrGraphics_ShapeRendering	= 107;
const int attrGraphics_TextRendering	= 108;
const int attrGraphics_PointerEvents	= 109;

const int attrStyle_Style		= 120;
const int attrStyle_Class		= 121;
const int attrStyle_Type		= 122;

const int attrFont_Family		= 130;
const int attrFont_Style		= 131;
const int attrFont_Variant		= 132;
const int attrFont_Weight		= 133;
const int attrFont_Stretch		= 134;
const int attrFont_Size			= 135;
const int attrFont_SizeAdjust	= 136;
const int attrFont_WriteMode	= 137;

const int attrXLink_Href		= 150;
const int attrXLink_Type		= 151;
const int attrXLink_Role		= 152;
const int attrXLink_Arcrole		= 153;
const int attrXLink_Title		= 154;
const int attrXLink_Show		= 155;
const int attrXLink_Actuate		= 156;
const int attrXLink_Target		= 157;

const int attrPsr_ObjectID		= 170;
const int attrPsr_ObjectName	= 171;
const int attrPsr_ClassID		= 172;
const int attrPsr_ClassName		= 173;
const int attrPsr_CimType		= 174;

const int attrHotPoint_Points	= 180;

////////////////////////////�������Ͷ���END////////////////////////////////////

//�ĵ��༭״̬����
/** @brief           �༭*/
const int Status_Edit	= 0;
/** @brief           ����*/
const int Status_Run	= 1;
//ͼ������
#define CLASS_BK		"backGroundClass"		//����
#define CLASS_IMAGE		"ImageClass"			//ͼƬ��
#define CLASS_BUS		"BusbarSectionClass"	//ĸ�߲�
#define CLASS_LINE		"ACLineSegmentClass"	//��·��
#define CLASS_TRANS		"PowerTransformerClass"	//��ѹ����
#define CLASS_GEN		"SynchronousMachineClass" //�������
#define CLASS_CAP		"CapacitorClass"		//��������
#define CLASS_BREAK		"BreakerClass"			//��·����
#define CLASS_SWITCH	"DisconnectorClass"		//��բ��
#define CLASS_GROUND	"GroundClass"			//�ӵز�
#define CLASS_REACTOR	"ReactorClass"			//�翹����
#define CLASS_PROTECT	"RelayClass"			//������
#define CLASS_WAVEREC	"WaveRecClass"			//¼������
#define CLASS_TEXT		"TextClass"				//�ı���
#define CLASS_OTHER		"OtherClass"			//������
#define CLASS_DATAPOINT "DataPointClass"		//���ݵ��

//�豸����
#define		STR_BLACK			"����"
#define		STR_SUBSTATION		"��վ"
#define		STR_BUS				"ĸ��"
#define		STR_LINE			"��·"
#define		STR_BREAKER			"��·��"
#define		STR_SWITCH			"��բ"
#define		STR_GEN				"�����"
#define		STR_TRANSFORMER		"��ѹ��"
#define		STR_CAPACITOR		"������"
#define		STR_PROTECTION		"����װ��"
#define		STR_WAVERECORDE		"¼����"
#define		STR_GROUND			"�ӵ�"
#define		STR_TEXT			"����"
#define		STR_LINK			"������"
#define		STR_OTHER 			"����"
#define		STR_REACTOR			"�翹��"
#define		STR_NET				"����"
#define		STR_SEC				"�����豸"	//�����豸

#define		STR_DATAPOINT		"���ݵ�"

#define		STR_NONE			"δָ��"

//��ѹ�ȼ�����
#define VOLTAGE_3KV			"kV3"	//3kv
#define VOLTAGE_6KV			"kV6"	//6kv
#define VOLTAGE_10KV		"kV10"	//10kv
#define VOLTAGE_13KV		"kV13"	//13kv
#define VOLTAGE_15KV		"kV15"	//10kv
#define VOLTAGE_20KV		"kV20"	//10kv
#define VOLTAGE_35KV		"kV35"	//10kv
#define VOLTAGE_110KV		"kV110"	//110kv
#define VOLTAGE_220KV		"kV220"	//220kv
#define VOLTAGE_330KV		"kV330"	//330kv
#define VOLTAGE_500KV		"kV500"	//500kv
#define VOLTAGE_800KV		"kV800" //800kv

/////////////////////////////�����������Ͷ���//////////////////////////////
/** @brief           �޲���*/
const int OPERATE_NULL			= 0;
/** @brief           ���Ը���*/
const int OPERATE_CHANGEATTR	= 1;
/** @brief           ����Ԫ��*/
const int OPERATE_ADDELEMENT	= 2;
/** @brief           ɾ��Ԫ��*/
const int OPERATE_DLEELEMENT	= 3;
/** @brief           ���Ԫ��*/
const int OPERATE_GROUPELEMENT	= 4;
/** @brief           ������*/
const int OPERATE_DEGROUPELEMENT= 5;

//////////////////////////////����ͼ��������//////////////////////////////
/** @brief           ����ֱ��*/
const int MOUSEDRAW_LINE		= 0;
/** @brief           ���ƾ���*/
const int MOUSEDRAW_RECT		= 1;
/** @brief           ������Բ*/
const int MOUSEDRAW_ELLIPSE		= 2;
/** @brief           ��������*/
const int MOUSEDRAW_POLYLINE	= 3;
/** @brief           ���ƶ����*/
const int MOUSEDRAW_POLYGON		= 4;
/** @brief           ��������*/
const int MOUSEDRAW_TEXT		= 5;
/** @brief           ��������*/
const int MOUSEDRAW_PATH		= 6;

//////////////////////////////�����λ�ö���/////////////////////////////
/** @brief           δ�����ͼ����*/
const int POSITION_NULL			= 0;
/** @brief           �����ͼ�η�Χ���룬Ϊ�����Ų���*/
const int POSITION_CENTER		= 1;
/** @brief           �����ͼ�η�Χ���ϵ㣬Ϊ�����ϵ��˫�����Ų���*/
const int POSITION_TOPLEFT		= 2;
/** @brief           �����ͼ�η�Χ���Ϸ���Ϊ���Ϸ��ĵ������Ų���*/
const int POSITION_TOP			= 3;
/** @brief           �����ͼ�η�Χ�����ϵ㣬Ϊ�����Ϸ���˫�����Ų���*/
const int POSITION_TOPRIGHT		= 4;
/** @brief           �����ͼ�η�Χ���󷽣�Ϊ���󷽵ĵ������Ų���*/
const int POSITION_LEFT			= 5;
/** @brief           �����ͼ�η�Χ���ҷ���Ϊ���ҷ��ĵ������Ų���*/
const int POSITION_RIGHT		= 6;
/** @brief           �����ͼ�η�Χ�����·���Ϊ�����·���˫�����Ų���*/
const int POSITION_BOTTOMLEFT	= 7;
/** @brief           �����ͼ�η�Χ���·���Ϊ���·��ĵ������Ų���*/
const int POSITION_BOTTOM		= 8;
/** @brief           �����ͼ�η�Χ�����·���Ϊ�����·���˫�����Ų���*/
const int POSITION_BOTTOMRIGHT	= 9;

//////////////////////////////���뷽ʽ////////////////////////////////////////////
/** @brief           �����*/
const int ALIGNMENT_LEFT = 0;
/** @brief           ˮƽ���ж���*/
const int ALIGNMENT_HORCENTER = 1;
/** @brief           �Ҷ���*/
const int ALIGNMENT_RIGHT = 2;
/** @brief           �϶���*/
const int ALIGNMENT_TOP = 3;
/** @brief           ��ֱ���ж���*/
const int ALIGNMENT_VERCENTER = 4;
/** @brief           �¶���*/
const int ALIGNMENT_BOTTOM = 5;

//////////////////////////////��ɫ����/////////////////////////////////////
#define aliceblue			RGB(240, 248, 255) 
#define antiquewhite		RGB(250, 235, 215) 
#define aqua				RGB( 0, 255, 255) 
#define aquamarine			RGB(127, 255, 212) 
#define azure				RGB(240, 255, 255) 
#define beige				RGB(245, 245, 220) 
#define bisque				RGB(255, 228, 196) 
#define	black				RGB( 0, 0, 0) 
#define	blanchedalmond		RGB(255, 235, 205) 
#define blue				RGB( 0, 0, 255) 
#define blueviolet			RGB(138, 43, 226) 
#define brown				RGB(165, 42, 42) 
#define burlywood			RGB(222, 184, 135) 
#define cadetblue			RGB( 95, 158, 160) 
#define chartreuse			RGB(127, 255, 0) 
#define chocolate			RGB(210, 105, 30)
#define coral				RGB(255, 127, 80) 
#define cornflowerblue		RGB(100, 149, 237) 
#define cornsilk			RGB(255, 248, 220) 
#define crimson				RGB(220, 20, 60) 
#define cyan				RGB( 0, 255, 255) 
#define darkblue			RGB( 0, 0, 139) 
#define darkcyan			RGB( 0, 139, 139) 
#define darkgoldenrod		RGB(184, 134, 11) 
#define darkgray			RGB(169, 169, 169) 
#define darkgreen			RGB( 0, 100, 0) 
#define darkgrey			RGB(169, 169, 169) 
#define darkkhaki			RGB(189, 183, 107) 
#define darkmagenta			RGB(139, 0, 139) 
#define darkolivegreen		RGB( 85, 107, 47) 
#define darkorange			RGB(255, 140, 0) 
#define darkorchid			RGB(153, 50, 204) 
#define darkred				RGB(139, 0, 0) 
#define darksalmon			RGB(233, 150, 122) 
#define darkseagreen		RGB(143, 188, 143) 
#define darkslateblue		RGB( 72, 61, 139) 
#define darkslategray		RGB( 47, 79, 79) 
#define darkslategrey		RGB( 47, 79, 79) 
#define darkturquoise		RGB( 0, 206, 209) 
#define darkviolet			RGB(148, 0, 211) 
#define deeppink			RGB(255, 20, 147) 
#define deepskyblue			RGB( 0, 191, 255) 
#define dimgray				RGB(105, 105, 105) 
#define dimgrey				RGB(105, 105, 105) 
#define dodgerblue			RGB( 30, 144, 255) 
#define firebrick			RGB(178, 34, 34) 
#define floralwhite			RGB(255, 250, 240) 
#define forestgreen			RGB( 34, 139, 34) 
#define fuchsia				RGB(255, 0, 255) 
#define gainsboro			RGB(220, 220, 220) 
#define	ghostwhite			RGB(248, 248, 255) 
#define gold				RGB(255, 215, 0) 
#define goldenrod			RGB(218, 165, 32) 
#define gray				RGB(128, 128, 128)  
#define green				RGB( 0, 128, 0) 
#define greenyellow			RGB(173, 255, 47) 
#define honeydew			RGB(240, 255, 240) 
#define hotpink				RGB(255, 105, 180) 
#define indianred			RGB(205, 92, 92) 
#define indigo				RGB( 75, 0, 130) 
#define ivory				RGB(255, 255, 240) 
#define khaki				RGB(240, 230, 140) 
#define lavender			RGB(230, 230, 250) 
#define lavenderblush		RGB(255, 240, 245) 
#define lawngreen			RGB(124, 252, 0) 
#define lemonchiffon		RGB(255, 250, 205) 
#define lightblue			RGB(173, 216, 230) 
#define lightcoral			RGB(240, 128, 128) 
#define lightcyan			RGB(224, 255, 255) 
#define lightgoldenrodyellow RGB(250, 250, 210) 
#define lightgray			RGB(211, 211, 211) 
#define lightgreen			RGB(144, 238, 144) 
#define lightgrey			RGB(211, 211, 211) 
#define lightpink			RGB(255, 182, 193) 
#define lightsalmon			RGB(255, 160, 122) 
#define lightseagreen		RGB( 32, 178, 170) 
#define lightskyblue		RGB(135, 206, 250) 
#define lightslategray		RGB(119, 136, 153) 
#define lightslategrey		RGB(119, 136, 153) 
#define lightsteelblue		RGB(176, 196, 222) 
#define lightyellow			RGB(255, 255, 224) 
#define lime				RGB( 0, 255, 0) 
#define limegreen			RGB( 50, 205, 50) 
#define linen				RGB(250, 240, 230) 
#define magenta				RGB(255, 0, 255) 
#define maroon				RGB(128, 0, 0) 
#define mediumaquamarine	RGB(102, 205, 170) 
#define mediumblue			RGB( 0, 0, 205) 
#define mediumorchid		RGB(186, 85, 211) 
#define mediumpurple		RGB(147, 112, 219) 
#define mediumseagreen		RGB( 60, 179, 113) 
#define mediumslateblue		RGB(123, 104, 238) 
#define mediumspringgreen	RGB( 0, 250, 154) 
#define mediumturquoise		RGB( 72, 209, 204) 
#define mediumvioletred		RGB(199, 21, 133) 
#define midnightblue		RGB( 25, 25, 112) 
#define mintcream			RGB(245, 255, 250) 
#define mistyrose			RGB(255, 228, 225) 
#define moccasin			RGB(255, 228, 181) 
#define navajowhite			RGB(255, 222, 173) 
#define navy				RGB( 0, 0, 128) 
#define oldlace				RGB(253, 245, 230) 
#define olive				RGB(128, 128, 0) 
#define olivedrab			RGB(107, 142, 35) 
#define orange				RGB(255, 165, 0) 
#define orangered			RGB(255, 69, 0) 
#define orchid				RGB(218, 112, 214) 
#define palegoldenrod		RGB(238, 232, 170) 
#define palegreen			RGB(152, 251, 152) 
#define paleturquoise		RGB(175, 238, 238) 
#define palevioletred		RGB(219, 112, 147) 
#define papayawhip			RGB(255, 239, 213) 
#define peachpuff			RGB(255, 218, 185) 
#define peru				RGB(205, 133, 63) 
#define pink				RGB(255, 192, 203) 
#define plum				RGB(221, 160, 221) 
#define powderblue			RGB(176, 224, 230) 
#define purple				RGB(128, 0, 128) 
#define red					RGB(255, 0, 0) 
#define rosybrown			RGB(188, 143, 143) 
#define royalblue			RGB( 65, 105, 225) 
#define saddlebrown			RGB(139, 69, 19) 
#define salmon				RGB(250, 128, 114) 
#define sandybrown			RGB(244, 164, 96) 
#define seagreen			RGB( 46, 139, 87) 
#define seashell			RGB(255, 245, 238) 
#define sienna				RGB(160, 82, 45) 
#define silver				RGB(192, 192, 192) 
#define skyblue				RGB(135, 206, 235) 
#define slateblue			RGB(106, 90, 205) 
#define slategray			RGB(112, 128, 144) 
#define slategrey			RGB(112, 128, 144) 
#define snow				RGB(255, 250, 250) 
#define springgreen			RGB( 0, 255, 127) 
#define steelblue			RGB( 70, 130, 180) 
//#define tan					RGB(210, 180, 140) 
#define teal				RGB( 0, 128, 128) 
#define thistle				RGB(216, 191, 216) 
#define tomato				RGB(255, 99, 71) 
#define turquoise			RGB( 64, 224, 208) 
#define violet				RGB(238, 130, 238) 
#define wheat				RGB(245, 222, 179) 
#define white				RGB(255, 255, 255) 
#define whitesmoke			RGB(245, 245, 245) 
#define yellow				RGB(255, 255, 0) 
#define yellowgreen			RGB(154, 205, 50)
////////���������http://www.w3.org/TR/SVG/types.html#ColorKeywords////////

#endif