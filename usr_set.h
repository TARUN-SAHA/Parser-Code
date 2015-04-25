#include<stdio.h>

typedef int Boolean;

#define NUMRECORD_POS 7		//2 byte[7-8] indicates number of records available 
#define FIXEDRECORD_STARTPOS 20
#define FIXEDRECORD_SIZE 9
#define DATA_TAG_LENGTH 2
#define DATA_SIZE_LENGTH DATA_TAG_LENGTH


//TV content Flags
#define CONTENT_SEXUAL_CONTENT			0x0001
#define CONTENT_VIOLENCE				0x0002
#define CONTENT_EXPLICIT_LANGUAGE		0x0004
#define CONTENT_STRONG_DIALOG			0x0008
#define CONTENT_FANTASY_VIOLENCE		0x0010
#define CONTENT_ADULT_SITUATIONS		0x0020
#define CONTENT_BRIEF_NUDITY			0x0040
#define CONTENT_GRAPHIC_LANGUAGE		0x0080
#define CONTENT_GRAPHIC_VIOLENCE		0x0100
#define CONTENT_LANGUAGE				0x0200
#define CONTENT_MILD_VIOLENCE			0x0400
#define CONTENT_NUDITY					0x0800
#define CONTENT_RAPE					0x1000
#define CONTENT_STRONG_SEXUAL_CONTENT	0x2000
#define CONTENT_CLEAR_ALL				0x4000

typedef enum eLanguageSTB
{
   kLang_eng = 0,
   kLang_spa,
   kLang_fre
}eLanguageSTB;

typedef enum eLanguageSAP
{
   kAudioLang_eng = 0,
   kAudioLang_spa,
   kAudioLang_fre
}eLanguageSAP;

typedef enum eFPdisplay
{
   kFP_NoDisplay = 0,
   kFP_Time,
   kFP_Channel,
   kFP_ChannelOnTune
}eFPdisplay;

typedef enum eACoutlet
{
   kAC_OFF = 0,
   kAC_ON,
   kAC_STB
}eACoutlet;

typedef struct fixed_records
{
   unsigned char rec_DataType;
   unsigned int rec_DataSize;
   unsigned int rec_DataRef;
}sFixedRecords;

typedef enum eTVrating
{
   kRating_NR = 0,
   kRating_TV_Y,
   kRating_TV_Y7,
   kRating_TV_G,
   kRating_G,
   kRating_TV_PG,
   kRating_PG,
   kRating_PG_13,
   kRating_TV_14,
   kRating_TV_MA,
   kRating_R,
   kRating_NC_17,
   kRating_AO,
   kRating_CA
}eTVratings;

typedef struct user_Settings		//TAG:
{
   eLanguageSTB stbDispLanguage;	//0x03E8
   eLanguageSAP sapAudioLanguage;	//0x03F8
   eFPdisplay fpDisplay;			//0x044E
   eFPdisplay acOutlet;				//0x044C
   unsigned int blockRatingMap;				//0x04B0
   unsigned int blockContentFlag;			//0x04B4
   unsigned int blockingPIN;					//0x04B2
   Boolean blockingPinEnabled;		//0x04B1
   Boolean blockTitle;				//0x04B3
}sUserSettings;

typedef struct timer_data
{
   int timeofday;
   int month;
   int day;
   int year;
   int channelNum;
   Boolean autotune;
}sTimer_data;

typedef struct stb_settings
{
   int *BlockedChannelSvcIds;			//0x1000
   int numBlockedChannels;
   int *FavChannels;					//0x1001
   int numFavChannels;
   sTimer_data reminderTimers[8];		//0x1200 to 0x1207
   short int reminder_count;
}sSTBsettings;


int getFixedRecord(int bytePos);
int getIntValue(int *startPos, int numBytes);
void getUserSettingsData(int startPos);