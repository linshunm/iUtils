
//OCXInterface.h.h
//集中定义提供给包容器的结构定义、宏定义等等；

#ifndef __INTERFACE_UI__
#define __INTERFACE_UI__


#define MAX_TCHAR_TO_UTF8_LEN         3//暂时按照3倍长度上报

#define MAX_LOG_USER_LEN        64
#define MAX_LOG_PSW_LEN         128
#define MAX_LOG_IP_LEN          128
#define MAX_LOG_VER_LEN         32
#define MAX_DOMAIN_LEN          64
//IP地址的长度
#define MAX_ADDR16
#define MAX_IP_LENGTH           64
#define MAX_GROUP_NUM           100
#define MAX_ROSTER_NUM          210

#define MAX_IMS_URI_LEN         256
#define MAX_IMS_ETAG_LEN                20

#define MAX_SUBJECT_LEN                  64
#define MAX_FECTION_NUM_LEN              64
#define MAX_FAX_NUM_LEN                  64
#define MAX_AFFILIATION_LEN              32
#define MAX_POSITION_LEN                 32
#define MAX_EMPLOYEE_NUM_LEN             32
#define MAX_PERSON_DESCRIP_LEN           200

#define MAX_IDENTIFIER_LEN              128
#define MAX_SEL_LEN                                             256
#define MAX_NAME_LEN                    96
#define MAX_ENTRY_INFO_LEN              7
#define MAX_PHONE_NUM_LEN               20
#define MAX_COMPANY_LEN                 32
#define MAX_PHOTO_URL_LEN               64
#define MAX_EMPLOYEE_LEN                32
#define MAX_STATE_LEN                   20
//#define MAX_NOTE_LEN                    20
#define MAX_DEPARTMENT_LEN              32
#define MAX_POST_CODE_LEN               10
#define MAX_REG_NUM_LEN                 128
#define MAX_ADDRESS_LEN                             128
#define MAX_EMAIL_LEN                                   64
#define  MAX_MSG_ID_LEN                 38
#define MAX_USERCAPACITY_LEN            20

#define MAX_IMS_SUBJECT_LEN                             64
#define MAX_IMS_IPADDRESS_LEN                   45
#define MAX_IMS_ROLE_LEN                                11
#define MAX_IMS_CONF_USER_STATUS                13
#define MAX_DISC_METHOD_LEN                             8
#define MAX_DATE_TIME_LEN                               40
#define MAX_DISPLAYNAME_LEN             128
//私人聊天室最大成员数
#define MAX_IMS_CHAT_ROOM_MEMBER_NUM    200
//DAS关于ECP2的号码定义的长度都是64+1
#define MAX_ECP2_PHONE_NUM_LEN          64
//群组标题
#define MAX_GROUP_BULLETIN_LEN          1800
//群组公告
#define MAX_GROUP_SUBJECT_LEN           200
#define MAX_GROUPMEMBER_TYPE_LEN        32
#define MAX_GROUPMEMBER_CAPS_LEN        8
#define MAX_GROUPCLASS_LEN              128
#define MAX_GROUPCLASS_NUM              200
#define MAX_GROUP_SEARCH_STRING_LEN     32

#define MAX_GROUP_TYPE_NAME_LEN          20
#define MAX_FILE_NAME_LEN		  255
//其它号码
#define  MAX_OTHER_TEL_LEN         256
//拼音长度
#define  MAX_SPELL_LEN             64
#define MAX_CLIENT_TYPE_LEN        32
#define MAX_CLIENT_VER_LEN         32
#define MAX_MAC_ADDR_LEN           32
#define MAX_OS_TYPE_LEN            32
#define MAX_USER_SEARCH_NUM        20
//验证码长度
#define MAX_AUTH_CODE_LEN          8

//公众号相关长度
#define MAX_STRING_LEN_16          16
#define MAX_STRING_LEN_20          20
#define MAX_STRING_LEN_32          32
#define MAX_STRING_LEN_36          36
#define MAX_STRING_LEN_64          64
#define MAX_STRING_LEN_128         128
#define MAX_STRING_LEN_256         256
#define MAX_STRING_LEN_512         512
#define MAX_STRING_LEN_1024        1024

//消息类型, 对应bIMSSendMessage的iType参数
#define IM_TYPE_UNKNOWN             0
//即时消息
#define IM_TYPE_IM                  1
//定时消息
#define IM_TYPE_TM                  2
#define IM_TYPE_SYSTEM              3
//即时匿名聊天
#define IM_TYPE_IM_AM               4
#define IM_TYPE_REPORT              5
//定时匿名聊天
#define IM_TYPE_TM_AM               6
//针对所收到的匿名消息的答复
#define IM_TYPE_RE_AM               7
//修改定时消息
#define IM_TYPE_EDIT_TM             8
//Group send message
#define IM_TYPE_GROUP_IM            9
//手机短信
#define IM_TYPE_SMS                 10
//自定义jpg图片
#define IM_TYPE_FILE_JPG            11
//自定义音频
#define IM_TYPE_FILE_WAV            12
//自定义bmp图片
#define IM_TYPE_FILE_BMP            13
//自定义消息，IM服务器透传即可
#define IM_TYPE_CM                  14
//聊天室中用MSRP发聊天消息
#define IM_TYPE_CHATROOM            15
#define IM_TYPE_MMS                 16      //收到彩信
#define IM_TYPE_SMS_SS              17      //通过SS发送短信 0521
#define IM_TYPE_SCRIPT              18      //发送手写字图片
#define IM_TYPE_FILE_TRANS_REQ      19      //发送文件请求
#define IM_TYPE_FILE_TRANS_RESPONSE 20      //文件发送请求的应答
#define IM_TYPE_CTD                 21      //CTD相关消息
#define IM_TYPE_TM_SESSION_MODE     22      //定时消息，采用session mode
#define IM_TYPE_IM_SESSION_MODE     23      //采用session mode方式的即时消息
#define IM_TYPE_GROUP_IM_SESSION_MODE 24    //Group send message by session mode
#define IM_TYPE_FAX                 25      //收到FAX
#define IM_TYPE_TM_SMS              26      //通过IM定时发送短信
#define IM_TYPE_ALARM    27 //SM或MM发送告警
#define IM_TYPE_EDIT_SM  28 //修改定时短信
#define IM_TYPE_URL      29 //PUSH URL消息 palma 2009-07-21
#define IM_TYPE_FACE_REQ                31   //向发送防索取表情图片
#define IM_TYPE_FACE_REQ_RESULT 32   //索取表情图片结果
#define IM_TYPE_SMS_SEND        33  //长短信的拆分和合并都由客户端完成方式。
#define IM_TYPE_CONF_IM         34   //会议中的即时消息
#define IM_TYPE_GROUP_FILE      35   //群组聊天中传输文件
#define IM_TYPE_GROUP_FILE_MSG  39   //群组聊天中收到文件的消息
#define IM_TYPE_ORDER_CONF      37   //预约会议召开回复//add on 2010-11-15,根据文档《多媒体会议业务3.64.04详细设计方案》

#define IM_TYPE_CHATRCS 45      //聊天中用MSRP发聊天消息
#define IM_TYPE_CHAT_FILE_RCS 46        //聊天中用MSRP发文件
#define IM_TYPE_CHATROOM_FILE_RCS 47    //聊天室中用MSRP发附件

#define IM_TYPE_UPDATE_INDICATION	40 //更新输入状态 
#define IM_TYPE_CONF_MMCONF			41

//MOA 发送大消息流程
#define IM_TYPE_SEND_LARGE_FILE		42    //点对点聊天发送大消息
#define IM_TYPE_GET_LARGE_FILE      43    //点对点聊天，在收到有人发送大消息后，主动去获取
#define IM_TYPE_LARGE_FILE_MSG		44   //通知有人给我发送大消息

//推送消息
//#define IM_TYPE_PUBLISH_MSG			45   (没用到)
#define IM_TYPE_PUBLISH_MSG_URL           46   //推送内容过大，下发的是URL，客户端需要自己去获取

//群组离线消息新增:"GRPMEMHIS"-群组成员变化
#define IM_TYPE_GROUP_MEMBER_CHANGED  49 
//转发的图文消息
#define IM_TYPE_TRANS_MSG             50
//系统下发任务消息(任务文件名)，由客户端去下载获取具体任务文件
#define IM_TYPE_SYSTEM_TASK_MSG            51 
//发起消息撤回操作
#define IM_TYPE_WITHDRAW_MSG             52
//阅后即焚消息
#define IM_TYPE_BURN_AFTER_READ_MSG      53
//收到阅后即焚大消息通知(URL),单聊发送阅后即焚文件
#define IM_TYPE_BURN_AFTER_READ_LARGE_MSG      54
//群聊发送阅后即焚文件
#define IM_TYPE_BURN_AFTER_READ_GROUP_FILE     55
//通用的第三方消息通知
#define IM_TYPE_COMMON_NOTIFY          56
//消息回执通知
#define IM_TYPE_RECEIPT_NOTIFY         57
//MMC会议通知消息，用于拉起VT100(警务通接口)
#define IM_TYPE_MMC_VT100_NOTIFY       58
//用于拉起VT100后的结果上报(警务通接口)
#define IM_TYPE_MMC_VT100_RESULT       59
//文件传输:接收方FILEID通知
#define IM_TYPE_FILE_ID_MSG            60
//推送公众号消息内容
#define IM_TYPE_PUBLISH_MSG            61 
//RCS离线消息推送URL
#define IM_TYPE_RCS_OFFLINEMSG_URL     62

//UI发起下载缩略图对应的原图
#define IM_TYPE_GET_GROUP_ORIGINAL_FILE   350    //下载原图(群组)
#define IM_TYPE_GET_ORIGINAL_FILE   430         //下载原图(点对点)
#define IM_TYPE_GET_SYSTEM_TASK_FILE   510    //下载获取系统任务文件

#define IM_LARGE_MSG_TYPE_TEXT  0
#define IM_LARGE_MSG_TYPE_JPG   1
#define IM_LARGE_MSG_TYPE_WAV   2
#define IM_LARGE_MSG_TYPE_BMP   3
#define IM_LARGE_MSG_TYPE_SCRIPT 4

//是否发送报告, 对应bIMSSendMessage和IMSGetFetchMsgResult的iReport参数
#define IM_REPORT_YES   1
#define IM_REPORT_NO    2

//机构或部门每批上报最大数
#define NUMS_FIRED_PER_TIME 150

//关于文件传送的上报给界面的事件类型
//#define GT_EVENT_FILE_TREANS_RATE        1//上报文件传送的进度条
#define GT_EVENT_RECVR_CANCEL_RECV       2 //上面对方拒绝或取消接收事件
#define GT_EVENT_INVITED_TO_RECEIVE_FILE 3 //收到给本人传送文件的请求invite
#define GT_EVENT_SENDER_CANCEL_SEND      4 //发送方取消了发送
#define MAX_USER_CAPACITY_LEN            64 //权限

//IMSGotGroupInfo(获取与本人相关的群组信息)事件的参数
#define FIRE_GROUP_TYPE_PRIVATE			1	//类型为私有群组
#define FIRE_GROUP_TYPE_PUBLIC			2	//类型为公共群组
#define FIRE_GROUP_TYPE_NOGROUP			3   //类型为未分组联系人//add by chenhongling 2006-5-16	
//#define FIRE_GROUP_TYPE_COMPANY			4	//类型为企业团队群组
#define FIRE_GROUP_TYPE_PUBLIC_BASIC	5	//类型为公共群组的基本群组  EcpGroup
#define FIRE_GROUP_TYPE_PUBLIC_ASSOC	6	//类型为公共群组的互助群组  EcpGroup
#define FIRE_GROUP_TYPE_PUBLIC_TEMP	    7	//类型为公共群组的临时群组  EcpGroup


#define FIRE_GROUP_USAGE_BLACK_LIST		1	//类型为私有群组时，用途为黑名单
#define FIRE_GROUP_USAGE_WHITE_LIST		2	//类型为私有群组时，用途为白名单
#define FIRE_GROUP_USAGE_OTHERS			3	//类型为私有群组时，用途为自定义分组
#define FIRE_GROUP_USAGE_ADDRESS_LIST	4	//联系人地址簿

#define MAX_IMS_GROUP_NUM               400 //最大群组数

//定义群组成员状态
#define GROUP_MEMBER_STATUS_JOINED              1       //已加入
#define GROUP_MEMBER_STATUS_INVITING_CONFIRM    2       //邀请待确认
#define GROUP_MEMBER_STATUS_JOINING_CONFIRM     3       //主动加入待批准

//用于表示公共群组 或 公共群组成员 的状态
#define PUBG_GROUP_MEMBER_STATUS_NONE                     0
#define PUBG_GROUP_MEMBER_STATUS_JOINED                   1
#define PUBG_GROUP_MEMBER_STATUS_WAIT_INVITEE_APPROVAL    2
#define PUBG_GROUP_MEMBER_STATUS_WAIT_CREATER_APPROVAL    3
#define MAX_GROUP_TYPE_NAME_LEN          20

//4.15.3.12	获取某公共群组的属性和成员列表事件 中的 参数
typedef struct tagFireMetaData 
{
    char cOwner[256+1];         //群组的所有者
    char cCreator[256+1];       //创建者
    char cTimeStamp[40+1];      //YYYY-MM-DDTHH:MM:SSZ
    int  iDuration;		        //存活时间，单位：秒；0表示不限时
    int  iVisibitily;	        //可见性，1:admin; 3:all
    int  iNeedPermit;           //是否自动接受加入请求，0自动同意，1需要确认
    int  iMax;                  //最大成员数
    int  forbid_2D_join;        //20151030新增，1-表示群禁止二维码扫描加入，0-表示可运行二维码扫描加入
    int  member_invite_enable;  //20160330新增，1-任何成员能添加群组成员，0-只能群主能添加成员
    char cSubject[MAX_GROUP_SUBJECT_LEN+1];        //ECP group 主题
    char cTopic[MAX_GROUP_BULLETIN_LEN+1];          //ECP group 公告栏
    char cType[MAX_GROUP_TYPE_NAME_LEN+1];//如果为person或者空,表示个人公共群组;company表示企业团队;basic表示基本组;association表示互助组
    char cWebUrl[MAX_IMS_URI_LEN+1];//表示团队的空间url地址
    char cGroupClassID[MAX_GROUPCLASS_LEN+1]; //群组类别  有两种形式，举例说明：如果填写成"3"，则表示一级类为Sport；如果填写成"3|5"则表示一级类为Sport，二级
	char cIdentifier[MAX_IDENTIFIER_LEN+1];   
	char bulletinSetterUri[64+1];//公告内容设置人URI
	char bulletinSetterName[64+1];//公告内容设置人姓名
	char bulletinSettime[32+1];//公告内容设置时间
}ATTRIBUTE_TYPE;

typedef struct tagEntryType
{
//	char cDisplayName[64+1]; //在该公共群组的显示名
	char cDisplayName[128+1]; //在该公共群组的显示名
	char cFullSpelling[MAX_NAME_LEN+1];//姓名全拼
	char cInfo[7+1];        //"Forword"：已加入该公共群组；"Defend"：正在等待Owner审批
//	char cURI[256+1];       //格式为sip:帐号@域名
	char cURI[64+1];       //1500个群成员内存开销大,与DAS确认后可改为64
	char cPhone[20+1];      //在该公共群组的显示名
	int iMemberStatus;        //成员加入状态：1已加入 2owner 邀请的待确认 3主动加入待批准的 0表示正式加入的成员
	char cMemberType[MAX_GROUPMEMBER_TYPE_LEN+1]; //成员类型：admin---管理员；member-普通成员   要求全部小写
	char cAllowModifyMyCaps[MAX_GROUPMEMBER_CAPS_LEN+1]; //第一个字符表示是否允许管理员修改自身属性(显示名)，'0'-不允许 '1'-允许；其他个预留;
}ENTRY_TYPE; //对应原来的 PUBLIC_GROUP_MEMBER

//群组列表过多时,通过JNI接口传递无法一次完成,需分批上报
#define MAX_GROUP_NUM_ONCE   100     
typedef struct tagFireAllGroup 
{
    char cGroupURI[256+1];      //定位群组的URI，私有群组时此字段无效
    char cGroupName[MAX_TCHAR_TO_UTF8_LEN*64+1];	    //群组名
    int  iGroupType;	        //群组类型 FIRE_GROUP_TYPE_XXX
    int  iGroupUsage;	        //群组用途（类型为公共群组时，此字段无效）	 FIRE_GROUP_USAGE_XXX			  
  //int  iGroupStatus;       //群组加入状态: 1已加入 2owner邀请待确认 3成员申请待批准（类型为公共群组时，此字段有效）    
    char cOwnerURI[256+1];	    //群组所有者（类型为私有群组时，此字段无效；
                                //为公共群组时，如果此字段为空，则所有者为别人（暂时不知道是谁），否则为本人的URI)  
    char cType[MAX_GROUP_TYPE_NAME_LEN+1];           //公共群组的分类: company:企业团队, person或空:个人公共群组
    int	 iMax;  //最大成员数
}FIRE_ALL_GROUP;
typedef struct tagGroupInfo
{
	int iNum;       //本次所返回的成员数
	FIRE_ALL_GROUP* pFireGroupInfo; //群组结构体
}FIRE_GROUP_INFO;


//4.12.3	获取本人或联系人的某单独信息
typedef struct subInfoPhotoPara {
	char cURI[256 + 1];                 //本人或某联系人的URI
	char cPhotoIndex[32 + 1];           //服务器返回详细信息中的大头像的文件名
}SUBINFO_PHOTO_PARA, *LP_SUBINFO_PHOTO_PARA;


//#define MAX_NAME_LEN					64
//#define MAX_IMS_URI_LEN				256
#define MAX_IMS_GROUP_LIST_NUM 200
typedef struct tagPublicGroupInputPara 
{
    char cGroupURI[256+1];      //如果为空字符串，则为新创建公共群组，否则表示修改该公共群组的属性
    char cGroupName[64+1];      //公共群组名称，如果此群组已存在，则是修改；否则是新建
    char cMyDisplayName[64+1];  //本人在该公共群组中的显示名称，最大长度64字节
    long lDuration;             //群组存活时长，单位为秒，0表示永远存在
    long lVisibitily;           //群组的可见性，1管理员可见；2成员可见；3所有用户可见
    long lNeedPermit;           //对加入本群组请求的处理方式：0自动同意；1需要确认
    long lMax;                  //本群组的最大成员数，如果大于系统允许的最大数，则以系统允许的最大数为准。当创建成功后，请通过bIMSSearchPublicGroupInfo方法来确定最大成员数
	int  forbid_2D_join;        //20151030新增，1-表示群禁止二维码扫描加入，0-表示可运行二维码扫描加入
	int  member_invite_enable;  //20160330新增，1-任何成员能添加群组成员，0-只能群主能添加成员
	char cSubject[MAX_GROUP_SUBJECT_LEN+1];        //公告栏信息
    char cTopic[MAX_GROUP_BULLETIN_LEN+1];          //pBulletin[64+1] 主题
    char cType[10+1];           //创建企业团队还是个人公共群组
    char cWebUrl[256+1];        //团队的空间url地址
    char cGroupClassID[MAX_GROUPCLASS_LEN+1];
    int  iListNum;              //创建群组时,指定的成员列表数目
    char *pGroupMemberList[MAX_IMS_GROUP_LIST_NUM];    //创建群组时,成员列表的URI
    char *pGroupMemberDispList[MAX_IMS_GROUP_LIST_NUM];    //创建群组时,成员对应的显示名
}PUBLIC_GROUP_INPUT_PARA, *LP_PUBLIC_GROUP_INPUT_PARA;

typedef struct tagPublicGroupInputParaTemp 
{
	char cGroupURI[MAX_IMS_URI_LEN+1];
	char cGroupName[MAX_NAME_LEN+1];
	char cMyDisplayName[MAX_NAME_LEN+1];
	long lDuration;
	long lVisibitily;
	long lNeedPermit;
	long lMax;
	int  forbid_2D_join;        //20151030新增，1-表示群禁止二维码扫描加入，0-表示可运行二维码扫描加入
	int  member_invite_enable;  //20160330新增，1-任何成员能添加群组成员，0-只能群主能添加成员
	char cSubject[MAX_GROUP_SUBJECT_LEN+1];
	char cBulletin[MAX_GROUP_BULLETIN_LEN+1];
	char cGroupClassID[MAX_GROUPCLASS_LEN+1];
    char cType[10+1];           //创建企业团队还是个人公共群组
    int  iListNum;              //创建群组时,指定的成员列表数目
    char *pGroupMemberList[MAX_IMS_GROUP_LIST_NUM];        //创建群组时,成员列表的URI
    char *pGroupMemberDispList[MAX_IMS_GROUP_LIST_NUM];    //创建群组时,成员对应的显示名
}PUBLIC_GROUP_INPUT_PARA_TEMP, *LP_PUBLIC_GROUP_INPUT_PARA_TEMP;


//4.15.3.19	模糊查询公共群组结果事件
typedef struct tagPublicGroupExplorer 
{
    char cGName[64+1];                  //公共群组名
    char cGURI[256+1];                  //公共群组URI
    char cOwnerURI[256+1];              //此公共群组创建者的URI
    char cGSubject[MAX_GROUP_SUBJECT_LEN+1];               //此公共群组主题
    char cGBulletin[MAX_GROUP_BULLETIN_LEN+1];              //此公共群组的公告栏
    char cIdentifier[128+1];            //此公共群组文档的URL
    int  iNeedPermit;                   //是否自动接受加入请求，0自动同意，1需要确认
    char cGroupClassID[MAX_GROUPCLASS_LEN+1];
}PUBLIC_GROUP_EXPLORER;
  
typedef struct tagPublicGroupBlacklistMember {
    char cURI[MAX_IMS_URI_LEN+1];
}PUBLIC_GROUP_BLACKLIST_MEMBER, *LP_PUBLIC_GROUP_BLACKLIST_MEMBER;

//新建或修改一个公共群组的结果事件 IMSCreatePublicGroupResult
typedef struct
{
    char cGroupURI[MAX_IMS_URI_LEN+1];  //定位此公共群组的唯一URI
    char cGroupName[MAX_NAME_LEN+1];   //群组名称，最大长度64字节
    char cTimeStamp[MAX_DATE_TIME_LEN+1];  //创建该群组的服务器时间
    int  iMax;   //最大成员数 不允许修改
}FIRE_CREATE_PUB_GROUP_INFO;


//4.15.3.24	申请者收到是否批准加入某公共群组的应答事件 IMSReqJoinPublicGroupResult的iResult含义
#define GROUP_INVITE_OPTION_YES		1
#define GROUP_INVITE_OPTION_NO		2
#define GROUP_INVITE_OPTION_TIMEOUT	3

typedef struct tagGroupInvite 
{
    char cGName[64+1];                  //公共群组名称
    char cGOwnerURI[256+1];             //发出邀请的URI，格式为：sip:帐号@域名
    char cGOwnerName[64+1];             //创建者在该群组中的昵称
    char cGURI[256+1];                  //定位此公共群组的唯一URI，
    char cGIdentifier[128+1];           //当用bIMSDoPublicGroupInvite方法处理此邀请时，请将该字符串原样返回
    char cDesc[256+1];//desc描述我们自己客户端界面只需要简单显示表示客户端可以正常处理.ecp的界面他们自己处理
}GROUP_INVITE, *LP_GROUP_INVITE;

//4.15.3.25	收到某人请求加入我的某个公共群组事件 参数
typedef struct tagGroupJoin 
{
    char cGName[64+1];                  //公共群组名称
    char cGProposerURI[256+1];          //申请者的URI 
    char cGProposerName[64+1];          //申请者在该群的昵称
    char cGURI[256+1];                  //公共群组的URI
    char cGIdentifier[128+1];           //公共群组文档的URL
}GROUP_JOIN, *LP_GROUP_JOIN;

//>> 群组 ECP需求200911

// 群组：设置是否允许管理员修改自己的属性（目前只有显示名）
// BOOL bIMSDispUICmdMethod(long lType=13, short FAR* pCmdPara)
typedef struct  
{
	char cGroupURI[MAX_IMS_URI_LEN+1]; // 群组URI
	char cAllowModifyMyCaps[MAX_GROUPMEMBER_CAPS_LEN+1]; //第一个字符表示是否允许管理员修改自身属性(显示名)，'0'-不允许 '1'-允许；其他个预留; 
}charGROUP_SET_CAPS_METHOD_PARAM, *LP_GROUP_SET_CAPS_METHOD_PARAM;

// 群组：设置是否允许管理员修改自己的属性的返回事件结构
// void IMSDispUICmdResult(long lType=13, short* pPara)
// 界面若10s内没有收到此事件，则认为设置失败；
typedef struct
{
	char cGroupURI[MAX_IMS_URI_LEN+1];//群组URI
	char cAllowModifyMyCaps[MAX_GROUPMEMBER_CAPS_LEN+1]; //第一个字符表示是否允许管理员修改自身属性(显示名)，'0'-不允许 '1'-允许；其他个预留; 
	int  iResultCode; //来自服务器的响应码 200----成功、403-----没有设置权限、404----表示群组或成员不存在；其他--------失败；
}GROUP_SET_CAPS_EVENT_PARAM, *LP_GROUP_SET_CAPS_EVENT_PARAM;

// 群组：管理员设置群成员属性(显示名)
// BOOL bIMSDispUICmdMethod(long lType=14, short FAR* pCmdPara)
typedef struct  
{
	char cGroupURI[MAX_IMS_URI_LEN+1];  //群组URI
	char cMemberURI[MAX_IMS_URI_LEN+1]; //成员URI；即,对此成员修改显示名
	char cNewDispName[MAX_NAME_LEN+1];  //新显示名
}GROUP_SET_MEMBER_DISPNAME_METHOD_PARAM, *LP_GROUP_SET_MEMBER_DISPNAME_METHOD_PARAM;

// 群组：管理员设置群成员属性(显示名)结果事件
// void IMSDispUICmdResult(long lType=14, short* pPara)
// 界面若10s内没有收到此事件，则认为设置失败(可以提示下用户)；设置成功的话，直接刷新界面；
typedef struct  
{
	char cGroupURI[MAX_IMS_URI_LEN+1];//群组URI
	char cMemberURI[MAX_IMS_URI_LEN+1];//成员URI；即,对此成员修改显示名
	char cNewDispName[MAX_NAME_LEN+1];  //新显示名
	int  iResultCode; //来自服务器的响应码；来自服务器的响应码 200----成功、403-----没有设置权限、404----表示群组或成员不存在；其他--------失败；
}GROUP_SET_MEMBER_DISPNAME_EVENT_PARAM, *LP_GROUP_SET_MEMBER_DISPNAME_EVENT_PARAM;

// 群组：群主设置成员为管理员/普通成员
// BOOL bIMSDispUICmdMethod(long lType=15, short FAR* pCmdPara)
typedef struct  
{
	char cGroupURI[MAX_IMS_URI_LEN+1];  // 群组URI
	char cMemberURI[MAX_IMS_URI_LEN+1]; // 成员URI
	char cMemberAttr[MAX_NAME_LEN+1];   // admin-设置为管理员；member-取消管理员；注意，不区分大小写
}GROUP_SET_MEMBER_ADMIN_METHOD_PARAM, *LP_GROUP_SET_MEMBER_ADMIN_METHOD_PARAM;

// 群组：群主设置成员为管理员/普通成员 结果事件结构
// void IMSDispUICmdResult(long lType=15, short* pPara)
typedef struct
{
	char cGroupURI[MAX_IMS_URI_LEN+1];  // 群组URI
	char cMemberURI[MAX_IMS_URI_LEN+1]; // 成员URI
	char cMemberAttr[MAX_NAME_LEN+1];   // admin-设置为管理员；member-取消管理员;  注意，不区分大小写
	int  iResultCode;  //来自服务器的响应码 200----成功、403-----没有设置权限、404----表示群组或成员不存在；409-----管理员数达上限；其他--------失败；
}GROUP_SET_MEMBER_ADMIN_EVENT_PARAM, *LP_GROUP_SET_MEMBER_ADMIN_EVENT_PARAM;

// 群组：按分类查询群组，查询时cGroupClassName字段无效
// BOOL bIMSDispUICmdMethod(long lType=16, short FAR* pCmdPara)
// 查询结果中也用该结构
typedef struct
{
	char cGroupURI[MAX_IMS_URI_LEN+1];              // 群组URI; 查某群组的分类信息时该字段有效
	char cIdentifier[MAX_IDENTIFIER_LEN+1];         //此公共群组文档的URL
    char cGroupClassID[MAX_GROUPCLASS_LEN+1];       // 群组分类的ID，查系统的所有群组分类信息时有效//可以为空，表示查询一级分类；否则查询某一级分类下属的二级分类
    char cGroupClassName[MAX_GROUPCLASS_LEN +1];    // 群组分类名， 查询结果中有效
	int  iResultCode;
}GROUP_QUERY_CLASS_METHOD_PARAM, *LP_GROUP_QUERY_CLASS_METHOD_PARAM, GROUP_CLASS, *LP_GROUP_CLASS;

// 群组：按分类查询群组结果
// void IMSDispUICmdResult(long lType=16, short* pPara)
typedef struct
{
	char cParentClass[MAX_GROUPCLASS_LEN +1];     //一级分类信息；可以为空；
	int  iClassNum;                               //组分类信息的个数，不超过MAX_GROUPCALSS_NUM个；
	// GROUP_CLASS  aGroupClass[MAX_GROUPCLASS_NUM]; //分类信息列表；若cParentClass为空，则cClassAttr为一级分类信息列表；
	                                              //若cParentClass不为空，则cClassAttr为二级分类信息列表；
	GROUP_CLASS *aGroupClass;
	int  iResultCode;                             //来自服务器的响应码 200----成功、404----不存在；其他--------失败；
}GROUP_QUERY_CLASS_EVENT_PARAM, *LP_GROUP_QUERY_CLASS_EVENT_PARAM;

typedef struct
{
	char cConfURI[MAX_IMS_URI_LEN+1];
	char cServer[32+1];
	int  iResultCode;//200：成功;400：非法请求;403：不允许;404：会议不存在;408：超时;500：服务器内部错误
}HTTP_HANGUP_CONF_EVENT_PARAM, *LP_HTTP_HANGUP_CONF_EVENT_PARAM;

// 群组：查询群组指定的群组分类
// bool bIMSSearchPublicGroup(int iSearchType, LPCTSTR pSearchString, long iPage, LPCTSTR pDomain, short*  pSearchAttrEx)
// 结果事件为 IMSSearchPuGroupResult(...) , 结果结构 tagPublicGroupExplorer
typedef struct tagGroupSearchAttr
{
	long iSearchType;
	char cSearchString[MAX_GROUP_SEARCH_STRING_LEN+1];
	long iPage;
	char cDomain[MAX_DOMAIN_LEN+1];
	char cClassID[MAX_GROUPCLASS_LEN+1]; //群组类别; 根据群组名或群组创建者搜索群组(可同时指定群组的分类，可只指定一级分类，也可同时指定一级分类和二级分类，一级分类和二级分类间以|分隔)
}GROUP_SEARCH_ATTR;

//<< 群组 ECP需求200911

//===========================================================================
// 转让群组请求 及对方回应
//===========================================================================
typedef struct tagGroupTrsReq {
	char cGName[MAX_NAME_LEN+1];
	char cGOwnerURI[MAX_IMS_URI_LEN+1];
	char cGOwnerName[MAX_NAME_LEN+1];
	char cGURI[MAX_IMS_URI_LEN+1];
	char cGIdentifier[MAX_IDENTIFIER_LEN+1];
}GROUP_TRS_REQ, *LP_GROUP_TRS_REQ;

// 转让群组结果事件
typedef struct tagGroupTrsResult
{
	char cGURI[MAX_IMS_URI_LEN+1];         // 群组URI
	char cGOwnerURI[MAX_IMS_URI_LEN+1];    // 新OwnerURI
	long iResult;                          // 结果 1-同意， 2-拒绝
}GROUP_TRS_RESULT;


//====================================================
//按字节对齐开始
//#pragma pack(1)           //这个地方界面没有对齐字节，我们也就使用默认的字节对齐

//lType ＝ 1  DAS + NGN环境，结构体定义为：
typedef struct tagLogonParaNGNDAS
{
    char chSoftPhone[MAX_LOG_USER_LEN+1];
	char chPassword[MAX_LOG_PSW_LEN+1];
	char chSSIP[MAX_LOG_IP_LEN+1];
	long lSSPort;
	char chSipServerIP[MAX_LOG_IP_LEN+1];
	long lSipServerPort;
	char chVersion[MAX_LOG_VER_LEN + 1]; //版本号
	int  iLoginType; //0-auto, 1-manual
}LOGON_PARA_NGN_DAS, *LP_LOGON_PARA_NGN_DAS;

//lType ＝ 2  Office OA方式 IMS环境，结构体定义为：
typedef struct tagLogonParaOA
{
	char chSoftPhone[MAX_LOG_USER_LEN+1]; //用户名
	char chPassword[MAX_LOG_PSW_LEN+1]; //密码
	char chSSIP[MAX_LOG_IP_LEN + 1];    //IMS的Domain  PCSCF
	char chBGWIP[MAX_LOG_IP_LEN + 1];   //暂时预留
	long lSSPort;                       //向IMS注册的端口号
	long lBGWPort;                      //暂时预留
	char chVersion[MAX_LOG_VER_LEN + 1]; //版本号
}LOGON_PARA_OA, *LP_LOGON_PARA_OA;

//lType ＝ 3  非OA 方式 IMS环境，结构体定义和2一致

//RCS登录的参数(登录的场景3)
typedef struct tagLogonParaRCS
{
	char szIMPU[MAX_LOG_USER_LEN+1];        //IMPU，用户名
	char szIMPI[MAX_LOG_USER_LEN+1];        //IMPI，鉴权号
	char szPassword[MAX_LOG_PSW_LEN+1]; //密码
	char szSipIP[MAX_LOG_IP_LEN+1];
	long lSipPort;
	char szXcapIP[MAX_LOG_IP_LEN+1];
	long lXcapPort;
	char szVersion[MAX_LOG_VER_LEN + 1];    //暂时预留
}LOGON_PARA_RCS, *LP_LOGON_PARA_RCS;

//lType ＝ 6  SS + DAS + CTD环境，结构体定义和1一致

//取xcap能力集的结构体
#define MAX_XCAP_CAPS_MCU_LEN	32
#define MAX_PSW_NOTIFY_LEN      300
#define MAX_USER_CAPS_LEN       8
typedef struct {
	char cMcu[MAX_XCAP_CAPS_MCU_LEN+1];

	bool bAUID_IMS_PIM;             // \\xcap-caps\\auids\\auid\\ims-pim
	bool bAUID_SMS_PIM;             // \\xcap-caps\\auids\\auid\\sms-pim
	bool bAUID_MMS_PIM;             // \\xcap-caps\\auids\\auid\\mms-pim
	bool bAUID_FAX_PIM;             // \\xcap-caps\\auids\\auid\\fax-pim
	bool bAUID_CALL_PIM;             // \\xcap-caps\\auids\\auid\\call-pim
	bool bAUID_MMC_PIM;	         //\\xcap-caps\\auids\\auid\\mmc-pim

	bool bAUID_Email_PIM;             // \\xcap-caps\\auids\\auid\\email-pim
	bool bAUID_Schedule_PIM;             // \\xcap-caps\\auids\\auid\\schedule-pim
	bool bAUID_Notice_PIM;             // \\xcap-caps\\auids\\auid\\notice-pim
	bool bAUID_Netdisk_PIM;	         //\\xcap-caps\\auids\\auid\\netdisk-pim

    int nIMSendCaps;
    int nSMSSendCaps;
	int nAutoGetMyLocation;  //登录成功后是否马上取"我的位置"节点详细信息
	int nNeedSet;
	int nValDays;
	int nDays;
	char cDateTime[MAX_DATE_TIME_LEN+1];
	char cPwdDateTime[MAX_DATE_TIME_LEN+1];
    char cPwdNotify1[MAX_PSW_NOTIFY_LEN+1];
    char cPwdNotify2[MAX_PSW_NOTIFY_LEN+1];
    char cPwdNotify3[MAX_PSW_NOTIFY_LEN+1];
    char cUserCaps[MAX_USER_CAPS_LEN+1];//用户自定义能力，仅使用第一位 0不需要密码提示 1 需要密码提示  长度为8  ECP专用
    //int nEcpNeedSet;                    //ecp密码提示功能 取消
	int iContact_Rls_Sync;//联系人和rls列表是否同步的标志
}XCAP_CAPS_INFO, *LP_XCAP_CAPS_INFO;//该结构体待扩展


//========================用户(本人)个人信息，和联系人信息================
//IMSGotUserInfo(获取本人详细信息事件)事件的参数
typedef struct tagIMSUserInfo
{
	char cName[64+1];           //昵称
	char cSex[6+1];             //性别：male,female
	int iOld;                   //年龄
	char cBirthday[10+1];       //出生日期，格式为：YYYY-MM-DD
//	char cInterest[200+1];      //个人兴趣
	char cInterest[400+1];      //个人兴趣
//	char cOccupation[32+1];     //职业
	char cOccupation[64+1];     //职业
	char cCountry[32+1];        //国家
	char cZone[128+1];          //时区：GMT+08:00
	char cCity[32+1];           //城市

	char cUcNum[20+1];          //UC号码
	char cPocNum[20+1];         //POC号码
	char cMessageNum[20+1];     //Message号码
	char cFaxNum[20+1]; //Fax号码

	char cHomeTel[20+1];        //住宅电话
	char cOfficeTel[20+1];      //办公电话 ECP第二终端
	char cMobileTel[20+1];      //移动电话 ECP第三终端
	char cAutoTel[20+1];        //车载电话
	char cSHomeTel[20+1];       //第二住宅电话
	char cSOfficeTel[20+1];     //第二办公电话
	char cSMobileTel[20+1];     //第二移动电话,//ECP版本时为小灵通号码 //ECP 第一终端

//	char cHomeAddr[128+1];      //住宅地址
	char cHomeAddr[256+1];      //住宅地址
//	char cOfficeAddr[128+1];    //办公地址
	char cOfficeAddr[256+1];    //办公地址
//	char cOtherAddr[128+1];     //其它地址
	char cOtherAddr[256+1];     //其它地址

	char cEmailAddr[64+1];      //Email地址

	char cDomain[64+1];         //本人所属的域名，所有联系人暂时也属于本域
	int iNotifyMode;            //iBWRule 黑白名单的启用策略：0启用黑名单；1启 用白名单；2黑、白名单都不启用

	char cURI[256+1];           //本人的URI, 如 sip:107214@zte.com.cn

	char cPhotoIndex[32 + 1];   //个人大头照索引(1~9，如果不是，则为自定义大头像的文件名，文件名格式为"年月日时分秒.jpg，例如20080205110230.jpg"
//	char cPhotoFile[12288+1];   //采用base64编码的JPG文件，如果此字段为空，则使用cPhoneIndex
	char *cPhotoFile;   //采用base64编码的JPG文件，如果此字段为空，则使用cPhoneIndex
	char cSoftPhone[20+1];      //该字段已经废弃了(个人的软号码在呼叫权限中返回好友已经没有该字段了)

	//个人的企业信息
	char cAffiliation[256+1];   //机构URI
	char cDepartment[256+1];    //部门（预留）
	char cDepartmentName[256+1]; //部门名
	char cDepartmentURI[256+1]; //部门URI
	char cEmployeeNum[256+1];   //员工编号
	char cPersonEmailAddr[256+1]; //
	char cPosition[256+1];      //职位
	char cPhotoURI[256+1];      //预留
	char cURI2[256+1];          //第二URI

	//ECP兼容
	char cBusinessPhone[20+1];  //ECP 业务号码
	char cSmsPhone[20+1];       //该字段已经废弃了(<sms-tel>1234567</sms-tel>)

	char cSmsBusiness[MAX_ECP2_PHONE_NUM_LEN+1]; //ECP2 短信业务号码
	char cFaxBusiness[MAX_ECP2_PHONE_NUM_LEN+1];
	char cFaxExt[MAX_ECP2_PHONE_NUM_LEN+1]; //该字段已经废弃(将原来的传真总机cFaxBusiness和传真分机号码cFaxExt合并为一个字段，在原来的business-fax字段中返回)
	int iVersionType; //ECP2 版本类型
	char cRealName[MAX_NAME_LEN+1]; //真实姓名 64
	char cOtherTel[MAX_OTHER_TEL_LEN+1]; //其他号码中间用"|"分割 256

	char cFullSpelling[MAX_NAME_LEN+1];//姓名全拼
	char cShortSpelling[MAX_NAME_LEN+1];//姓名简拼
	char cFullSpelling1[MAX_NAME_LEN+1];//昵称全拼
	char cShortSpelling1[MAX_NAME_LEN+1];//昵称简拼
	int  iAreaCodeFlag;//区域标识(0-未知 1-北方 2-南方)
	
	char cCompanyUri[MAX_IMS_URI_LEN+1];
	char cUserCapacity[MAX_USER_CAPACITY_LEN+1];
	//以下字段待扩展，未使用
	char cAnswerMode[10+1];     //待定
	char cMsgDeliverReport[10+1]; //待定
	char cOnlyNotify[10+1];     //待定

	char cAreaCode[20+1];			//国家码
	char cPersonalID[20+1];		//个人ID号
	int  IsVoipEnabled;  //是否有voip权限
	int  IsRecieptEnabled;  //是否有发送带回执消息的权限
	int  IsWatermarkEnabled;  //数字水印开关
	int  IsTransfileEnabled;  //文件传输功能开关
	int  iMaxGroupNum;   //群组数量最大值
	char capabilitySet[30];//新能力控制开关集（每个字节代表一个能力开关值,从第1个字节开始依次表示:音频会议,视频会议。其中字符‘1’-on,‘0’-off; ）
}USER_INFO, *LP_USER_INFO;

//===========================================================================
//4.18	PS管理
//===========================================================================
#define MAX_NOTE_LEN                    64	
#define MAX_PRESENCE_LOCATION_LEN       128
#define MAX_BOOL_CAMERA_LEN             6
#define MAX_PHONESTATUS_LEN             20

#define PS_TYPE_PRENSENCE	(long)0//可以扩展.下一个方法类型定义为：PS_TYPE_... 1
#define PS_TYPE_REGCIRCLE	(long)1		//EcpCircle
#define PS_TYPE_PHONESTATUS (long)2		//PhoneStatus
#define PS_TYPE_PRESCENE_CHECK_CONNECT	(long)3  //通过publish测试客户端和服务器的链接是否正常

typedef struct tagPsMethodPara
{
    char cNote[MAX_NOTE_LEN+1];    //本人状态
    char cLocation[MAX_PRESENCE_LOCATION_LEN+1];  //本人地理位置
    char cCamera[MAX_BOOL_CAMERA_LEN+1];  //是否有摄像头 ture false 
//  char cMood[128+1];
	char cMood[512+1];
    char cJob[128+1];
    char cPhoneStatus[MAX_PHONESTATUS_LEN+1]; //电话状态
//  char cEnlarge2[MAX_BOOL_CAMERA_LEN+1]; // (目前没有使用)
    char CTerminalType[16+1];//终端类型
    char COsType[16+1];//终端操作系统类型
}PS_PRENSENCE_METHOD_PARA,LP_PS_PRENSENCE_METHOD_PARA;

typedef struct tagPsEventPara
{
    char cURI[MAX_IMS_URI_LEN+1];    //好友的URI
    char cNote[MAX_NOTE_LEN+1];                 //状态
    char cLocation[MAX_PRESENCE_LOCATION_LEN+1];  //好友的地理位置
    char cCamera[MAX_BOOL_CAMERA_LEN+1];  //是否有摄像头 ture false    
    char cMood[128+1];
    char cJob[128+1];
    char cPhoneStatus[MAX_PHONESTATUS_LEN+1]; // 电话状态
//  char cEnlarge2[MAX_BOOL_CAMERA_LEN+1];   // (目前没有使用)
    char CTerminalType[16+1];//终端类型
    char COsType[16+1];//终端操作系统类型
    char cPhone[MAX_PHONE_NUM_LEN+1]; //好友SoftDA号码 可能为空
    char cSipFromURI[MAX_IMS_URI_LEN+1];    //发出此消息的URI
    int  iTimestamp; //= 1表示本人登录成功后，好友的状态是服务器主动
    //下发的;
    //= 0表示联系人主动修改自己的状态，向其好友发布的状态
}PS_PRENSENCE_EVENT_PARA, *LP_PS_PRENSENCE_EVENT_PARA;

//4.23.2IMSGotDeptAddrListResult下载企业通讯录结果的事件 中的 iType定义
#define DeptAddrListType_end            -1  //表示上报结束
#define DeptAddrListType_Company        0   //0公司信息
#define DeptAddrListType_Affiliation    1   //1机构信息(废弃)
#define DeptAddrListType_Departments    2   //2部门信息
#define DeptAddrListType_EmployeeInfo   3   //3员工信息
#define DeptAddrListType_Dept_info      4   //查询部门详细信息

typedef struct tagEmployeeInfo{
	char cDepartmentName[MAX_NAME_LEN+1];
	char cDepartmentURI[MAX_IMS_URI_LEN+1];
	char cName[MAX_NAME_LEN+1];
	char cURI[MAX_IMS_URI_LEN+1];
	//int iOrder;
	//char cURI1[MAX_IMS_URI_LEN+1];//0309 删除，被cTelUri取代
    char chSerialNo[20 + 1];  //ECP2用于排序
    char cEtag[MAX_IMS_ETAG_LEN+1];//同步个人通讯录与企业通讯录时使用  MAX_IMS_ETAG_LEN
	char cEmployeeNum[MAX_EMPLOYEE_LEN+1];
	char cOfficeTel[MAX_PHONE_NUM_LEN+1];//办公电话总机
	char cOfficeTel2[MAX_PHONE_NUM_LEN+1];//办公电话分机
	char cMobileTel[MAX_PHONE_NUM_LEN+1];
	char cMobileTel2[MAX_PHONE_NUM_LEN+1];
	char cTelUri[MAX_IMS_URI_LEN+1];
	//char cNumber[64+1];
	char cHomeTel[MAX_PHONE_NUM_LEN+1];
	char cHomeTel2[MAX_PHONE_NUM_LEN+1];
    char cSmsBusiness[MAX_ECP2_PHONE_NUM_LEN+1];// palma 2009-04-29 ECP2 短信业务号码
    char cFaxBusiness[MAX_ECP2_PHONE_NUM_LEN+1];// palma 2009-04-29 ECP2 传真业务号码
	char cWorkAddress[MAX_ADDRESS_LEN+1];
	char cEmailAddress[MAX_EMAIL_LEN+1];
	char cPersonEmailAddress[MAX_EMAIL_LEN+1];
	int  iEnableCreateGroupNum;
	char cUserCapacity[64+1];//返回用户的所有权限
	char cPosition[MAX_POSITION_LEN+1];
	char cSex[6+1];//性别
	char cBirthday[10+1];
	char cMood[128+1];//心情短语
	char cJob[128+1];//工作范围
	char cHomePageURI[MAX_IMS_URI_LEN+1];
	//char cAffiliation[MAX_NAME_LEN+1];  //机构
	char cAffiliationDefaultNum[20+1];//机构编号
	//char cDepartment[MAX_NAME_LEN+1];   //部门
	//char cSection[MAX_NAME_LEN+1];      //处
	//char cSectionURI[MAX_IMS_URI_LEN+1];    //处室_uri
	char cPhotoURL[MAX_PHOTO_URL_LEN+1];
	char cCompanyName[MAX_NAME_LEN+1];		//2008.01.28 Advanced Search
	char cCompanyUri[MAX_IMS_URI_LEN+1];
	//char cAffiliationUri[MAX_IMS_URI_LEN +1];
	char cStatus[MAX_STATE_LEN+1];
	char cNote[MAX_NOTE_LEN+1];	
    int iClientUseType;//用于区分同步搜索、企业通讯录搜索、聊天室中的搜索
	char cWorkAffiliation[MAX_IMS_URI_LEN+1];//工作单位(即为cDepthPath)  
    char cOtherTel[MAX_OTHER_TEL_LEN+1];//ECP环境下其他号码 CMCC此字段为空   
	char cFullSpelling[MAX_NAME_LEN+1];
	char cShortSpelling[MAX_NAME_LEN+1];
	char cFullSpelling1[MAX_NAME_LEN+1];//目前没使用但预留
	char cShortSpelling1[MAX_NAME_LEN+1]; //目前没使用但预留
}EMPLOYEE_INFO, *LP_EMPLOYEE_INFO;

typedef struct tagDepartmentsInfo{
	char cAffiliationName[MAX_NAME_LEN+1];
	char cAffiliationURI[MAX_IMS_URI_LEN+1];
	char cDepthPath[MAX_IMS_URI_LEN+1];//部门路径
    int  iFatherDepartmentURIType;//0----机构，1--部门
	char cName[MAX_NAME_LEN+1];
	char cURI[MAX_IMS_URI_LEN+1];
	//int  iOrder;
    char  chSerialNo[20 + 1];  //ECP2用于排序
	char cDepartmentNum[MAX_DEPARTMENT_LEN+1];
	char cDepartmentTel[MAX_PHONE_NUM_LEN+1];
	char cDepartmentTel1[MAX_PHONE_NUM_LEN+1];
	char cFaxNum1[MAX_PHONE_NUM_LEN+1];
	char cFaxNum2[MAX_PHONE_NUM_LEN+1];
	char cDepartmentDetailAddr[MAX_IMS_URI_LEN+1];
	char cDepartmentMailAddr[MAX_IMS_URI_LEN+1];
	char cDepartmentPostCodeAddr[MAX_IMS_URI_LEN+1];
	char cDepartmentEmailAddr[MAX_IMS_URI_LEN+1];
}DEPARTMENTS_INFO, *LP_DEPARTMENTS_INFO;

typedef struct tagAffiliationInfo{
	char cName[MAX_NAME_LEN+1];
	char cURI[MAX_IMS_URI_LEN+1];
    char chSerialNo[20 + 1];  //ECP2用于排序
	int  iAffiliationType;//0306//0－》集团 1－》省市 2－》地市3－》县市
	char cFatherAffiliationName[MAX_NAME_LEN+1];
	char cFatherAffiliationURI[MAX_IMS_URI_LEN+1];
	int  iFatherAffiliationURIType;//0----公司，1--机构
	char cAffiliationNum[MAX_AFFILIATION_LEN+1];
	char cAffiliationTel[MAX_PHONE_NUM_LEN+1];
	char cAffiliationTel2[MAX_PHONE_NUM_LEN+1];//0306
	char cAffiliationAddress[MAX_ADDRESS_LEN+1];
	char cAffiliationMailAddress[MAX_ADDRESS_LEN+1];
	char cAffiliationPostCode[MAX_POST_CODE_LEN+1];//MAX_POSTCODE_LEN
	char cAffiliationEmailAddress[MAX_EMAIL_LEN+1];
	char cAffiliationFaxNum[MAX_FAX_NUM_LEN+1];
	char cAffiliationFaxNum2[MAX_FAX_NUM_LEN+1];
	char cRegisterNum[MAX_REG_NUM_LEN+1];//0306
	char cHomePageURI[MAX_IMS_URI_LEN+1];//0306
}AFFILIATION_INFO, *LP_AFFILIATION_INFO;


typedef struct tagCompanyInfo{
	char cURI[MAX_IMS_URI_LEN+1];
	char cName[MAX_NAME_LEN+1];
	char cCompanyNum[MAX_COMPANY_LEN+1];
	char cCompanyTel[MAX_PHONE_NUM_LEN+1];
    //char cOfficeTelNumDef2[MAX_PHONE_NUM_LEN+1];  //规范没有, 准备废弃
	char cCompanyAddress[MAX_ADDRESS_LEN+1];
	char cCompanyMailAddress[MAX_ADDRESS_LEN+1];
	char cCompanyPostCode[MAX_POST_CODE_LEN+1];
	char cCompanyEmailAddress[MAX_EMAIL_LEN+1];
	char cCompanyFax[MAX_PHONE_NUM_LEN+1];
	char cCompanyUrl[MAX_IMS_URI_LEN+1];
	char cCompanyStatus[10+1];//company-status的长度要暂时定义为10
	char cAffiliationURI[MAX_IMS_URI_LEN+1];
	char cHomePageURI[MAX_IMS_URI_LEN+1];
    
    //没有用到, 准备废弃
	//int  iMMSRight;
	//int  iSMSRight;
	//int  iPhoneCallRight;
	//int  iFaxRight;
	//int  iAudioConfRight;
	//int  iVideoConfRight;
	//int  iDataConfRight;
	//int  iDatap2pRight;
	//char cUserCapacity[MAX_USERCAPACITY_LEN+1]; //大容量中应废弃，用户权限功能集已经不在企业通讯录中返回了，而是调整在用户个人信息中返回了
    
    char cCapacityFlag[MAX_USERCAPACITY_LEN+1];//第一位是SMAP标志
	EMPLOYEE_INFO MyDetailInfo;
}COMPANY_INFO, *LP_COMPANY_INFO;


//IMSGotAddressList获取本人联系人之一的列表事件的pListInfo参数
typedef struct tagListInfo
{
    char cUcNum[20+1];              //UC号码
    char cPocNum[20+1];             //POC号码
    char cMessageNum[20+1];         //Message号码
    char cFaxNum[20+1];             //Fax号码
    
    char cHomeTel[20+1];            //住宅电话
    char cOfficeTel[20+1];          //办公电话
    char cMobileTel[20+1];          //移动电话
    char cAutoTel[20+1];            //车载电话
    char cSHomeTel[20+1];           //第二住宅电话
    char cSOfficeTel[20+1];         //第二办公电话
    char cSMobileTel[20+1];         //第二移动电话
    
    char cHomeAddr[128+1];          //住宅地址
    char cOfficeAddr[128+1];        //办公地址
    char cOtherAddr[128+1];         //其它地址
    
    char cEmailAddr[64+1];          //Email地址
    
    char cName[64+1];     	        //昵称
    char cSex[6+1];                 //性别
    int  iOld;                      //年龄
    char cBirthday[10+1];		    //出生日期，格式为：YYYY-MM-DD
    char cInterest[200+1];          //个人兴趣
    char cOccupation[32+1];         //职业
    char cCountry[32+1];            //国家
    
    char cSoftphone[20+1];          //软号码
    char cRealName[64+1];   //真实姓名 64
}LIST_INFO, *LP_LIST_INFO;

//IMSGotPersonSubInfo事件的参数， lType=1时"ECP用户的业务号码和能力集信息"
typedef struct tagUserEcpInfo 
{
	char cBusinessPhone[20+1];      //业务号码
    int  nBusinessType; //1:固话实号码；2：小灵通号码；3：虚拟号码；4：固话虚号码；5：天翼手机实号码（预留）；6：天翼手机虚号码（预留）
	char cCapacityFlag[6+1];        //能力集
}USER_ECP_INFO, *LP_USER_ECP_INFO;

//联系人的个人变化时对本人的通知
typedef struct tagPIMNotify{
	char cURI[256+1];   //发生变化的某联系人的URI
	char cChangeType[64 + 1]; //变化的类型，目前仅支持大头像，即值为photo
	char cPhotoIndex[32 + 1];  //大头像的文件名
}PIM_NOTIFY, *LP_PIM_NOTIFY;

//4.27.2(5)
#define MAX_IMS_TRANSMIT_PHONE_LEN   20
#define MAX_IMS_TRANSMIT_STATUS      8
#define TransmitStatusLen 16
typedef struct 
{
    char cStatus[TransmitStatusLen+1];
}TransmitStatus_T;
typedef struct tagPersonImRelatedInfo{
	int  iReturnCode;//返回的状态码

	char cBusinessPhone[20+1];//business-tel
	char cURI[256+1];//该字段暂时没有用到
	char cUserCapacity[64+1];//返回用户的所有权限

	int  iTransmitType;//转移模式：0－不转移，1－转移到移动终端，2－转移到移动终端并保存离线消息
	char cTransmitPhone[MAX_IMS_TRANSMIT_PHONE_LEN+1];// 转移到的号码

	char cServicePhone[MAX_IMS_TRANSMIT_PHONE_LEN+1];
	
	int  nStatusNum;
	TransmitStatus_T vTransmitStatus[MAX_IMS_TRANSMIT_STATUS];//在线转移的状态 Busy/Away..., 每个状态占用16字节，最多3个状态
	bool bStatusChange;
}PERSON_IM_RELATED_INFO, *LP_PERSON_IM_RELATED_INFO;

typedef struct tagPersonCallInfo{
	int  iReturnCode;//返回的状态码
	char cOfficeTel[MAX_PHONE_NUM_LEN+1];//办公电话总机
	char cMobileTel[MAX_PHONE_NUM_LEN+1];
	char cSecondMobileTel[MAX_PHONE_NUM_LEN+1];//第二移动电话
	char cBusinessPhone[MAX_PHONE_NUM_LEN+1];//business-tel呼叫业务号码(ECP)
    char cBusinessSms[MAX_PHONE_NUM_LEN+1];//短信业务号码
    char cBusinessFax[MAX_PHONE_NUM_LEN+1];//传真业务号码
	char cURI[MAX_IMS_URI_LEN+1];
	char cName[MAX_NAME_LEN+1];
	char cName1[MAX_NAME_LEN+1];
	int  iVersionType;//版本类型
	int  nBusinessType;//1:固话实号码;2:小灵通号码;3:虚拟号码;4:固话虚号码;5:天翼手机实号码(预留);6:天翼手机虚号码(预留)(ECP)
	char cAreaCode[MAX_PHONE_NUM_LEN+1];
	char cCapacityFlag[6+1];//该字段即为原来返回个人信息中的呼叫的同一个字段(ECP)
	char cRuleCapacity[MAX_USER_CAPACITY_LEN+1];
	char cUserCapacity[MAX_USER_CAPACITY_LEN+1];
	int  nAudioConfMaxMember;  //音频会议最大成员数
	int  nVideoConfMaxMember;   //视频会议最大成员数
	char cSoftPhone[MAX_PHONE_NUM_LEN+1];//本人软号码
//	char csSoftPhonePWD[256+1];
//	char csSSIP[128+1];
//	char csSSPort[128+1];
	int  iLocalFlag;   //是否为同一集群标志,1-同一个集群,0-集群外,默认为1
}PERSON_CALL_INFO, *LP_PERSON_CALL_INFO;//呼叫相关权限

typedef struct tagPersonMMCInfo{
	int  iReturnCode;//返回的状态码
	char cBusinessPhone[MAX_PHONE_NUM_LEN+1];
	char cURI[MAX_IMS_URI_LEN+1];
	char cName[MAX_NAME_LEN+1];
	char cAreaCode[MAX_PHONE_NUM_LEN+1];
	char cUserCapacity[MAX_USER_CAPACITY_LEN+1];
	int  nAudioConfMaxMember;  //音频会议最大成员数,0表示不限制
	int  nVideoConfMaxMember;  //视频会议最大成员数,0表示不限制
	int  nDataConfMaxMember;   //数据会议最大成员数,0表示不限制
	int  nConfMaxDuration;     //会场最大时长，0表示不限制
}PERSON_MMC_INFO,*LP_PERSON_MMC_INFO;//多媒体会议权限

typedef struct tagImPreferenceStruct{
    int    iOperType;
    char   cURI[MAX_IMS_URI_LEN+1];
    short* pAddrList;//不同类别对应不同的结构指针
}IM_PREFERENCE_STRUCT, *LP_IM_PREFERENCE_STRUCT;

//隐身策略功能
#define RULE_SET_STEALTHY_ALLOW 1  //stealthy-allow-rule
#define RULE_SET_STEALTHY_BLOCK 2  //stealthy-block-rule

typedef struct tagRuleSetMethod
{
	long lOpeType;  //操作类别 0:增加 1:删除 2:查询
	char cID[MAX_STRING_LEN_32+1];  //ID
	char cURI[MAX_IMS_URI_LEN+1];	 //uri
}RULE_SET_METHOD, *LP_RULE_SET_METHOD;

typedef struct tagRuleSetEventInfo
{
	long lType;  //类别(对应上面两个宏)
	char cID[MAX_STRING_LEN_32+1];  //ID
	char cURI[MAX_IMS_URI_LEN+1];	 //uri
}RULE_SET_EVENT_INFO, *LP_RULE_SET_EVENT_INFO;

typedef struct tagRuleSetEvent
{
	long lResultCode;//服务器返回的结果代号
	long lOpeType; //操作类别 0:增加 1:删除 2:查询
	long lNum;     //pInfo对应的数组大小
	LP_RULE_SET_EVENT_INFO pInfo;
}RULE_SET_EVENT, *LP_RULE_SET_EVENT;

typedef struct  
{
    long lType;	
	LP_RULE_SET_EVENT pRulesetevent;
}IMSRuleSetEvent_T;


typedef struct tagUserLogonInfo {
    char 	cClientType[MAX_CLIENT_TYPE_LEN+1];   //客户端类别      32
	char    cClientVersion[MAX_CLIENT_VER_LEN+1];	 //客户端版本   32
	char    cMacAddr[MAX_MAC_ADDR_LEN+1];	 //MAC地址：32
	char    cOSType[MAX_OS_TYPE_LEN +1];	//OS类型32
} USER_LOGON_INFO_METHOD, *LP_USER_LOGON_INFO_METHOD;

//DispUICmdForEcp2Result事件的lType=8时用到。发送登录信息结果
typedef struct 
{
	int  iReturnCode;            //服务器返回的状态码
}ReturnCode_LoginInfo;

//DispUICmdForEcp2Result事件的lType=1时用到。修改个人密码结果通知
typedef struct {
	long lCode;
}ReturnCode_Modify_MyPassword, *LP_ReturnCode_Modify_MyPassword;


/***事件******************************************/
//nType:0:查询 1.添加 2.修改 3.删除
typedef struct tagDeptFavoriteEventParam
{
	int nType;
	int iCode;
	int iNum;
	short *pData;
}DEPT_FAVORITE_EVENT_PARAM, *LP_DEPT_FAVORITE_EVENT_PARAM;

//4.27.2	事件
typedef struct  
{
    int lType;
    //    short* pPara;
    union
    {
       IM_PREFERENCE_STRUCT   ImPreferenceStruct;
	   #if 0
	//>> ECP 群组需求
	   GROUP_SET_MEMBER_ADMIN_EVENT_PARAM    GroupSetMemberAdminEventParam;     // ECP 群组 设置成员为"admin/member"
	   GROUP_SET_CAPS_EVENT_PARAM            GroupSetCapsEventParam;      // ECP 群组 设置成员自己caps为 允许/不允许(0/1) 管理员修改显示名
	   GROUP_SET_MEMBER_DISPNAME_EVENT_PARAM GroupSetMemberDispnameEventParam;  // ECP 群组 管理员修改成员
	   GROUP_QUERY_CLASS_EVENT_PARAM         GroupQueryClassEventParam;         // ECP 群组 查询群组分类(按ClassID/GroupURI)
	   #endif
	   GROUP_CLASS                           GroupClass;
	//<< ECP 群组需求
	   DEPT_FAVORITE_EVENT_PARAM DeptFavoriteEvent;
	   GROUP_QUERY_CLASS_EVENT_PARAM         HttpHangUpConf;
    };
}IMSDispUICmdResult_T;


//4.16.3	收到发送消息递送报告事件
typedef struct 
{
    long nMsgType;                           //Message类型
	char pReportID[MAX_MSG_ID_LEN+1];                   //所发送message的ID，与方法bIMSSendMessage中的pLocalMsgID对应
    char pReportType[16];                   //递送结果，取值如下: FAILED/SUCCESSFUL/REP_NOT_RECEIVED
    char pDoneTime[40];                     //递送成功时的时间，格式为RFC1123-date
    char pStatusCode[256];                  //错误状态码，当为广州电信环境时，且pReportType＝FAILED时，有效
    char pReportFrom[256+1];
	char pTerminal[256+1];
}IMSSendMessageReport_T;

//===========================================================================
//聊天室
//===========================================================================

//收到某聊天室中成员变化的信息事件(IMSGotChatRoomUserInfo)中的参数：GET_PUB_CHATROOM_USERS_INFO
typedef struct tagPubChatRoomUsersInfo
{
	char cConfURI[MAX_IMS_URI_LEN+1];//聊天室的URI(新增), 原控件解析xml有问题而加
	char cUserURI[MAX_IMS_URI_LEN+1]; //成员之一的URI
	char cUserRole[MAX_IMS_ROLE_LEN+1]; //此成员的角色：participant成员；creater创建	                                     者；chairman也是创建者或主席
	char cUserStatus[MAX_IMS_CONF_USER_STATUS+1]; //此成员当前状态：pending创建者已邀请此成员，此成员还未答复；connected已加入（此时cWhenJoin有效）；disconnecting正在退出；	                                     disconnected已退出（此时cDiscMethod和cWhenDisc有效）；
	char cWhenJoin[MAX_DATE_TIME_LEN+1]; //此成员加入聊天室的时间
	char cDiscMethod[MAX_DISC_METHOD_LEN+1]; //此成员离开聊天室原因：booted被创建者踢出departed自己主动离开；
	char cWhenDisc[MAX_DATE_TIME_LEN+1]; //此成员离开聊天室时间
	char cInviterURI[MAX_IMS_URI_LEN+1];     //这个字段来记录是哪个人邀请cUserURI进入聊天室的
}GET_PUB_CHATROOM_USERS_INFO, *LP_GET_PUB_CHATROOM_USERS_INFO;

typedef struct tagConferenceInfo
{
	char cDlgId[256+1];         //用于界面窗口和控件消息对应的id
	char cConfURI[256+1];       //唯一定位此聊天室的URI
	char cSubject[64+1];        //聊天室的主题
	int  iMaxUser;              //本聊天室允许的最大成员数
	int  iUserNum;              //本聊天室的当前成员数
	int  iActive;               //目前没有用，预留
	int  iLocked;               //聊天室是否被锁定 1锁定 0未锁定 公共聊天室有效。锁定时，聊天室中成员不能发言，也不能申请加入该聊天室
	int  iThisUserNum;          //本次所返回的成员数
	LP_GET_PUB_CHATROOM_USERS_INFO pChatRoomUsers;  //指向聊天室各成员状态相关信息的首地址，界面根据成员数分配，目前每次下发一个
}CONFERENCE_INFO, *LP_CONFERENCE_INFO;

typedef struct tagEcpDeptSearchAdvMethod{
	char 	cName[MAX_NAME_LEN+1];   //姓名   64
	char    cURI[MAX_IMS_URI_LEN+1];	 //帐号   256
	char    cSpell[MAX_SPELL_LEN+1];	 //拼音：全拼或简拼 64
	char    cPhone[MAX_PHONE_NUM_LEN +1];	//电话号码 20
}ECP_DEPT_SEARCH_ADV_METHOD, *LP_ECP_DEPT_SEARCH_ADV_METHOD;

//4.23.3	搜索通讯录的方法
typedef struct tagDeptSearchAdvMethod{
	char cDepartmentName[MAX_NAME_LEN+1];
	char cName[MAX_NAME_LEN+1];
	char cEmployeeNum[MAX_EMPLOYEE_NUM_LEN+1];
	char cEmail[MAX_EMAIL_LEN+1];
	char cMobile[MAX_PHONE_NUM_LEN+1];
	char cUniPhone[MAX_IMS_URI_LEN+1];
	char cCompanyURI[MAX_IMS_URI_LEN+1];
}DEPT_SEARCH_ADV_METHOD;

//bIMSLoadAddrList上传/下载个人通讯录方法 的参数
typedef struct tagPersonListInfo
{
    char cName[64+1];                   //昵称
    char cURI[256+1];                   //URI
    char cURI2[256+1];                  //第二URI
    char cOfficeTel[20+1];              //办公电话
    char cMobileTel[20+1];              //移动电话
    char cFectionNum[64+1];             //Fection号码
    char cFaxNum[64+1];                 //Fax号码
    char cAutoTel[20+1];                //Auto号码
    char cHomeTel[20+1];                //家庭电话
    char cSecondOfficeTel[20+1];        //第二办公电话
    char cSecondMobileTel[20+1];        //第二移动电话
    char cOfficeAddress[128+1];         //办公地址
    char cOtherAddress[128+1];          //其他地址
    char cEmailAddress[64+1];           //Email地址
    char cAffiliation[32+1];            //机构名
    char cSex[6+1];                     //性别：
    int  iOld;                          //年龄
    char cPhotoURL[256+1];              //照片的URL
    char cPosition[32+1];               //位置
    char cEmployeeNum[32+1];            //员工号
    char cPersonalDescription[200+1];   //个人描述
    char cGroupName[64+1];              //组名
    int  iCategory;                     //标识来自企业，还是自行添加
    
    char cEtag0[MAX_IMS_ETAG_LEN +1];   // 该电话簿总的ETag, 把总的ETag放在每条记录中是为做的改动最小
    char cEtag[MAX_IMS_ETAG_LEN +1];    // 该电话簿记录的ETag
	int bNeedSynchronize;              // 是否需要同步
	char cWorkAffiliation[MAX_IMS_URI_LEN+1];//工作单位
}PERSON_LIST_INFO, *LP_PERSON_LIST_INFO;

#define MSG_INDICATION_MSK_STATE			0x00000001
#define MSG_INDICATION_MSK_LASTACTIVE		0x00000002
#define MSG_INDICATION_MSK_REFRESH          0x00000004
typedef struct  
{
	char  cMemberURI[MAX_IMS_URI_LEN+1];
	int mask;
	int state;		//1->active or 0->idle(default)
	char  tLastactive[MAX_DATE_TIME_LEN+1]; //dateTime
	int refresh;     // refresh time
}MsgIndication_T;


#ifndef MAX_PASSWORD_LEN
#define MAX_PASSWORD_LEN 128
#endif



#endif
