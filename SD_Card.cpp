#include "SD_Card.h"

uint32_t SDCard_Size; // 修改为 uint32_t，防止 64GB/128GB 以上大卡容量计算溢出
uint16_t Flash_Size;

bool SD_Init() {
  // 1. 初始化引脚
  pinMode(SD_CS, OUTPUT);    
  digitalWrite(SD_CS, HIGH);               

  // 2. ✨【核心优化】：将 ESP32-C6 读写 SD 卡的 SPI 速率强行拉满到 24MHz
  // 这样从 64GB 卡里边读边解调 MJPEG 视频流时，才不会卡顿
  if (SD.begin(SD_CS, SPI, 24000000)) { 
    printf("SD card initialization successful!\r\n");
  } else {
    printf("SD card initialization failed!\r\n");
    return false;
  }

  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE){
    printf("No SD card attached\r\n");
    return false;
  }
  else{
    printf("SD Card Type: ");
    if(cardType == CARD_MMC){
      printf("MMC\r\n");
    } else if(cardType == CARD_SD){
      printf("SDSC\r\n");
    } else if(cardType == CARD_SDHC){
      // 64GB 的大卡如果是 FAT32 格式，硬件层通常识别为 SDHC/SDXC
      printf("SDHC/SDXC\r\n"); 
    } else {
      printf("UNKNOWN\r\n");
    }

    uint64_t totalBytes = SD.totalBytes();
    uint64_t usedBytes = SD.usedBytes();
    
    SDCard_Size = totalBytes / (1024 * 1024); // 放心计算，uint32_t 不再害怕溢出
    
    printf("Total space: %llu Bytes (~%u MB)\n", totalBytes, SDCard_Size);
    printf("Used space: %llu Bytes\n", usedBytes);
    printf("Free space: %llu Bytes\n", totalBytes - usedBytes);
  }
  return true;
}

bool File_Search(const char* directory, const char* fileName)    
{
  File Path = SD.open(directory);
  if (!Path) {
    printf("Path: <%s> does not exist\r\n",directory);
    return false;
  }
  File file = Path.openNextFile();
  while (file) {
    if (strcmp(file.name(), fileName) == 0) {                                           
      if (strcmp(directory, "/") == 0)
        printf("File '%s%s' found in root directory.\r\n",directory,fileName);  
      else
        printf("File '%s/%s' found.\r\n",directory,fileName); 
      file.close(); // ✨ 修正：别忘了关闭遍历出来的子文件句柄，防止句柄泄露
      Path.close();                                                                     
      return true;                                                                     
    }
    file.close(); // ✨ 修正：未匹配成功时也需要关闭
    file = Path.openNextFile();                                        
  }
  printf("File '%s' not found in directory: %s\r\n", fileName, directory);           
  Path.close();                                                                         
  return false;                                                                         
}

uint16_t Folder_retrieval(const char* directory, const char* fileExtension, char File_Name[][100], uint16_t maxFiles)    
{
  File Path = SD.open(directory);
  if (!Path) {
    printf("Path: <%s> does not exist\r\n",directory);
    return 0;
  }
  
  uint16_t fileCount = 0;
  char filePath[100];
  File file = Path.openNextFile();
  while (file && fileCount < maxFiles) {
    if (!file.isDirectory() && strstr(file.name(), fileExtension)) {
      strncpy(File_Name[fileCount], file.name(), 100); 
      if (strcmp(directory, "/") == 0) {                                             
        snprintf(filePath, 100, "%s%s", directory, file.name());   
      } else {                                                                       
        snprintf(filePath, 100, "%s/%s", directory, file.name());
      }
      printf("File found: %s\r\n", filePath);
      fileCount++;
    }
    file.close(); // ✨ 修正：每次用完记得关闭临时文件
    file = Path.openNextFile();                                      
  }
  Path.close();                                                                         
  if (fileCount > 0) {
    printf(" %d <%s> files were retrieved\r\n",fileCount,fileExtension);
    return fileCount;                                                                 
  } else {
    printf("No files with extension '%s' found in directory: %s\r\n", fileExtension, directory);
    return 0;                                                                         
  }
}

void remove_file_extension(char *file_name) {
  char *last_dot = strrchr(file_name, '.');
  if (last_dot != NULL) {
    *last_dot = '\0'; 
  }
}

void Flash_test()
{
  printf("/********** RAM & Flash Test**********/\r\n");
  uint32_t flashSize = ESP.getFlashChipSize();
  Flash_Size = flashSize/1024/1024;
  printf("Flash size: %d MB \r\n", Flash_Size);
  
  // 顺便打印一下当前剩余的可用堆内存，心里有个底
  printf("Free Heap Memory: %d Bytes\r\n", ESP.getFreeHeap());
  printf("/******* RAM Test Over********/\r\n\r\n");
}