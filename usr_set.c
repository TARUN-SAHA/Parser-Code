#include "usr_set.h"
#define TEST_DEBUG 1

static sFixedRecords *gRecords;
static int gFileSize;
static int gActualDataSize;
unsigned char *gReadData;
unsigned int recordSize;
static sUserSettings gUserSettings;
static sSTBsettings gStbSettings;


#ifdef TEST_DEBUG
void printBlockedContentString(unsigned int contentFlag)
{
   unsigned int bitPos;
   while(contentFlag)
   {
      bitPos = contentFlag ^ ( contentFlag & ( contentFlag - 1 ) );
	  contentFlag = ( contentFlag & ( contentFlag - 1 ) );
	  
	  switch (bitPos)
	  {
	     case   CONTENT_SEXUAL_CONTENT:
		        printf("CONTENT_SEXUAL_CONTENT \n");
		        break;
         case   CONTENT_VIOLENCE:
		        printf("CONTENT_VIOLENCE \n");
		        break;
         case   CONTENT_EXPLICIT_LANGUAGE:
		        printf("CONTENT_EXPLICIT_LANGUAGE \n");
		        break;
         case   CONTENT_STRONG_DIALOG:
		        printf("CONTENT_STRONG_DIALOG \n");
		        break;
         case   CONTENT_FANTASY_VIOLENCE:
		        printf("CONTENT_FANTASY_VIOLENCE \n");
		        break;		
         case   CONTENT_ADULT_SITUATIONS:
		        printf("CONTENT_ADULT_SITUATIONS \n");
		        break;		
         case   CONTENT_BRIEF_NUDITY:
		        printf("CONTENT_BRIEF_NUDITY \n");
		        break;		
         case   CONTENT_GRAPHIC_LANGUAGE:
		        printf("CONTENT_GRAPHIC_LANGUAGE \n");
		        break;		
         case   CONTENT_GRAPHIC_VIOLENCE:
		        printf("CONTENT_GRAPHIC_VIOLENCE \n");
		        break;		
         case   CONTENT_LANGUAGE:
		        printf("CONTENT_LANGUAGE \n");
		        break;		
         case   CONTENT_MILD_VIOLENCE:
		        printf("CONTENT_MILD_VIOLENCE \n");
		        break;		
         case   CONTENT_NUDITY:
		        printf("CONTENT_NUDITY \n");
		        break;		
         case   CONTENT_RAPE:
		        printf("CONTENT_RAPE \n");
		        break;					
         case   CONTENT_STRONG_SEXUAL_CONTENT:
		        printf("CONTENT_STRONG_SEXUAL_CONTENT \n");
		        break;
		 default:
				break;
/*         case   CONTENT_CLEAR_ALL:
		        printf("CONTENT_CLEAR_ALL \n");
		        break; */
	  }
   }
}

void printBlockedRatedString(unsigned int ratingMap)
{
   unsigned int bitPos, i = 0x01;
   while(ratingMap)
   {
      bitPos = ratingMap ^ ( ratingMap & ( ratingMap - 1 ) );
	  ratingMap = ( ratingMap & ( ratingMap - 1 ) );
	  
	  switch (bitPos)
	  {
	     case   1 << kRating_NR:
		        printf("CONTENT NOT RATED \n");
		        break;
         case   1 << kRating_TV_Y:
		        printf("TV_Y Rating Blocked \n");
		        break;
         case   1 << kRating_TV_Y7:
		        printf("TV_Y7 Rating Blocked \n");
		        break;
         case   1 << kRating_TV_G:
		        printf("TV_G Rating Blocked \n");
		        break;
         case   1 << kRating_G:
		        printf("G Rating Blocked \n");
		        break;		
         case   1 << kRating_TV_PG:
		        printf("TV_PG Rating Blocked \n");
		        break;		
         case   1 << kRating_PG:
		        printf("PG Rating Blocked \n");
		        break;		
         case   1 << kRating_PG_13:
		        printf("PG_13 Rating Blocked \n");
		        break;		
         case   1 << kRating_TV_14:
		        printf("PG Rating Blocked \n");
		        break;		
         case   1 << kRating_TV_MA:
		        printf("TV_14 Rating Blocked \n");
		        break;		
         case   1 << kRating_R:
		        printf("R Rating Blocked \n");
		        break;		
         case   1 << kRating_NC_17:
		        printf("NC_17 Rating Blocked \n");
		        break;		
         case   1 << kRating_AO:
		        printf("AO Rating Blocked \n");
		        break;					
         case   1 << kRating_CA:
		        printf("CA Rating Blocked \n");
		        break;
		 default:
				break;
	  }
   }
}

#endif

int getFixedRecord(int bytePos)
{
   int i, numBytes = bytePos;
   for(i=0; i<recordSize; i++)
   {
      gRecords[i].rec_DataType  = gReadData[numBytes++];
//      printf("rec_DataType = 0x%X \n",  gRecords[i].rec_DataType);
       
	  gRecords[i].rec_DataSize = getIntValue(&numBytes, 4);
//      printf("rec_DataSize = %d \n",  gRecords[i].rec_DataSize);

	  gRecords[i].rec_DataRef = getIntValue(&numBytes, 4);
//      printf("rec_DataRef = %d \n",  gRecords[i].rec_DataRef);
   }

   return (numBytes - bytePos); 
}

int getIntValue(int *startPos, int numBytes)
{
   unsigned int retValue = 0;
   int i;
   
   for (i = 1; i <= numBytes; i++)
   {
      retValue = retValue | ((int)gReadData[(*startPos)++] << ( ( numBytes - i ) * 8 ));
   }

   return retValue;			   
}

void getUserSettingsData(int startPos)
{
   int array_startPos = startPos;
   int numSettingsParam = (int)gReadData[(array_startPos)++];
   int i, dataSize, dataTag, dataFlag;

   printf("numSettingsParam = %d \n", numSettingsParam);

   for (i = 0; i < numSettingsParam; i++)
   {
      dataTag = getIntValue(&array_startPos, DATA_TAG_LENGTH);
//	  printf("dataTag = %d\n", dataTag);
	  dataFlag = (int)gReadData[(array_startPos)++];
//	  printf("dataFlag = 0x%X\n", dataFlag);
	  dataSize = getIntValue(&array_startPos, DATA_SIZE_LENGTH);
//	  printf("dataSize = %d\n", dataSize);

	  switch ( dataTag )
	  {
	     case   0x03E8:
		        gUserSettings.stbDispLanguage = getIntValue(&array_startPos, dataSize);
		        break;
         case   0x03F8:
		        gUserSettings.sapAudioLanguage = getIntValue(&array_startPos, dataSize);
                break;
         case   0x044E:
		        gUserSettings.fpDisplay = getIntValue(&array_startPos, dataSize);
                break;
         case   0x044C:
		        gUserSettings.acOutlet = getIntValue(&array_startPos, dataSize);
                break;
         case   0x04B0:
		        gUserSettings.blockRatingMap = getIntValue(&array_startPos, dataSize);
#ifdef TEST_DEBUG
                printBlockedRatedString(gUserSettings.blockRatingMap);
#endif	
                break;
         case   0x04B4:
		        gUserSettings.blockContentFlag = getIntValue(&array_startPos, dataSize);
#ifdef TEST_DEBUG
                printBlockedContentString(gUserSettings.blockContentFlag);
#endif				
                break;
         case   0x04B2:
		        gUserSettings.blockingPIN = getIntValue(&array_startPos, dataSize);
                break;
         case   0x04B1:
		        gUserSettings.blockingPinEnabled = getIntValue(&array_startPos, dataSize);
                break;
         case   0x04B3:
		        gUserSettings.blockTitle = getIntValue(&array_startPos, dataSize);
                break;
         default:
                array_startPos += dataSize;
                break;				
	  }
   }
}

void getSTBSettingsData(int startPos)
{
   int numSettingsParam = (int)gReadData[(startPos)++];
   int i, dataSize, dataTag, dataFlag;
   int iterator;
   
   
   printf("numSettingsParam = %d \n", numSettingsParam);
   
   for (i = 0; i < numSettingsParam; i++)
   {
      dataTag = getIntValue(&startPos, DATA_TAG_LENGTH);
//	  printf("dataTag = %d\n", dataTag);
	  dataFlag = (int)gReadData[(startPos)++];
//	  printf("dataFlag = 0x%X\n", dataFlag);
	  dataSize = getIntValue(&startPos, DATA_SIZE_LENGTH);
//	  printf("dataSize = %d\n", dataSize);
      
	  switch ( dataTag )
	  {
	     case   0x1000:
		        iterator = 0;
		        gStbSettings.numBlockedChannels = dataSize / 4;
				
				if (gStbSettings.numBlockedChannels)
				{
				   if ( gStbSettings.BlockedChannelSvcIds )
				   {
				      free (gStbSettings.BlockedChannelSvcIds);
				      gStbSettings.BlockedChannelSvcIds = 0;
				   }
				   gStbSettings.BlockedChannelSvcIds = (int *)malloc(gStbSettings.numBlockedChannels * sizeof(int));
				   
				   while(iterator < gStbSettings.numBlockedChannels)
				   {
				      gStbSettings.BlockedChannelSvcIds[iterator] = getIntValue(&startPos, 4);
					  iterator++;
				   }
				}
		        break;
	     case   0x1001:
		        iterator = 0;
		        gStbSettings.numFavChannels = dataSize / 4;
				
				if (gStbSettings.numFavChannels)
				{
				   if ( gStbSettings.FavChannels )
				   {
				      free (gStbSettings.FavChannels);
				      gStbSettings.FavChannels = 0;
				   }
				   gStbSettings.FavChannels = (int *)malloc(gStbSettings.numFavChannels * sizeof(int));
				   
				   while(iterator < gStbSettings.numFavChannels)
				   {
				      gStbSettings.FavChannels[iterator] = getIntValue(&startPos, 4);
					  iterator++;
				   }
				}
		        break;
	     case   0x1200:
	     case   0x1201:
	     case   0x1202:
	     case   0x1203:
	     case   0x1204:
	     case   0x1205:
	     case   0x1206:
	     case   0x1207:
		        gStbSettings.reminderTimers[gStbSettings.reminder_count].timeofday = getIntValue(&startPos, 2);
				
				//TODO: identify significance of corresponding byte
				startPos += 4;
				gStbSettings.reminderTimers[gStbSettings.reminder_count].month = getIntValue(&startPos, 1);
				gStbSettings.reminderTimers[gStbSettings.reminder_count].day = getIntValue(&startPos, 1);
				gStbSettings.reminderTimers[gStbSettings.reminder_count].channelNum = getIntValue(&startPos, 2);
				gStbSettings.reminderTimers[gStbSettings.reminder_count].year = getIntValue(&startPos, 2);
				gStbSettings.reminderTimers[gStbSettings.reminder_count].autotune = getIntValue(&startPos, 1);
                startPos += dataSize;
				gStbSettings.reminder_count++;
		        break;
         default:
                startPos += dataSize;
                break;
	  }
   }

}

#ifdef TEST_DEBUG
void printUserSettings()
{
   printf("stbDispLanguage = %d \n sapAudioLanguage = %d \n fpDisplay = %d \n acOutlet = %d \n blockRatingMap = %d \n blockContentFlag = %d \n blockingPIN = %d \n blockingPinEnabled = %d \n blockTitle = %d \n",
          gUserSettings.stbDispLanguage, gUserSettings.sapAudioLanguage, gUserSettings.fpDisplay, gUserSettings.acOutlet, gUserSettings.blockRatingMap,gUserSettings.blockContentFlag, gUserSettings.blockingPIN, gUserSettings.blockingPinEnabled, gUserSettings.blockTitle);
}

void printSTBSettings()
{
   int i;
   
   printf("Fav Channels: \n");
   for (i=0; i<gStbSettings.numFavChannels; i++)
   {
      printf("		%d \n", gStbSettings.FavChannels[i]);
   }
   
   printf("Blocked Channels: \n");
   for (i=0; i<gStbSettings.numBlockedChannels; i++)
   {
      printf("		%d \n", gStbSettings.BlockedChannelSvcIds[i]);
   }
   
   printf(" %d Timers: \n", gStbSettings.reminder_count);
   for (i=0; i<gStbSettings.reminder_count; i++)
   {
      printf("		Ch No. %d --- Date: %d/%d/%d @ [%d:%02d] \n", gStbSettings.reminderTimers[i].channelNum, gStbSettings.reminderTimers[i].month, gStbSettings.reminderTimers[i].day, gStbSettings.reminderTimers[i].year, gStbSettings.reminderTimers[i].timeofday / 60, gStbSettings.reminderTimers[i].timeofday % 60);
   }   
}
#endif

int main (int argc, char *argv[])
{
   FILE *fd;
   int ByteRead, i = 0, numFiles, file_itr = 1;
   char read2Byte[2];
   int array_itr = 0, temp_itr;

   if ( argc <= 1 )
   {
      printf("Error\n");
      return 0;
   }
      
   while( file_itr < argc) 
   {
   
      fd = fopen(argv[file_itr], "rb+");
      if (!fd)
	  {
	     printf("file open error\n");
         return 0;
	  }

	   fseek(fd, 0, SEEK_END);
	   gFileSize = ftell(fd);
	   gReadData = (char *)malloc(gFileSize + 1);
	   printf("memory allocated %d bytes \n", gFileSize);
	   
	   fseek(fd, 0, SEEK_SET);
	   fread(gReadData, gFileSize, 1, fd);
	   
	   array_itr = NUMRECORD_POS;
	   recordSize = ((int)gReadData[array_itr] << 8) | ((int)gReadData[++array_itr]);
	   printf("number of records = %d\n", recordSize);
	   gRecords = (sFixedRecords * )malloc(recordSize * (sizeof(sFixedRecords)));

	   array_itr = FIXEDRECORD_STARTPOS;

	   ByteRead = getFixedRecord(array_itr);
	   array_itr += ByteRead;
		
	   gActualDataSize = getIntValue(&array_itr, 4);
	   
	   i = 0;
	   while( i < recordSize )
	   {
			temp_itr = array_itr + gRecords[i].rec_DataRef;
  /*        if(gRecords[i].rec_DataType == 0x14)
			   getMSOSettingsData(&array_itr);
			else*/ if(gRecords[i].rec_DataType == 0x15)
			   getUserSettingsData(temp_itr);
			else if(gRecords[i].rec_DataType == 0x16)
			   getSTBSettingsData(temp_itr);
			i++;
	   }
	   file_itr++;
	}
	   
	#ifdef TEST_DEBUG
	   printUserSettings();
	   printSTBSettings();
	#endif 

   return 0;
}
