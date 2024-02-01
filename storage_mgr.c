#include <stdio.h>
#include <stdlib.h>
#include "storage_mgr.h"
#include "dberror.h"

FILE *fp;

extern void initStorageManager()
{
  fp = NULL;
  printf("**********Storage Manager V0.0.1 initialized**********\n");
}
// Creation of the page file
extern RC createPageFile(char *fName)
{
  RC returnCode = RC_OK;

  // If the file already exists, change it to empty.
  openFile(fp, fName, "wb+");

  // If file pointer is NULL, return File Not Found Error.
  if (fp == NULL)
  {
    returnCode = RC_FILE_NOT_FOUND;
    RC_message = "ERR: UNABLE TO LOCATE FILE";
    printError(*RC_message);
    return returnCode;
  }
  size_t blockNum = PAGE_SIZE;
  size_t blockSize = sizeof(char);
  // Allocating memory to PAGE_SIZE element
  SM_PageHandle newblankPage = (SM_PageHandle)calloc(blockNum,blockSize);

  if (newblankPage == NULL)
  {
    // Handle memory allocation failure.
    fclose(fp);
    returnCode = RC_MEM_ALLOC_FAILED;
    RC_message = "ERR: MEMORY ALLOCATION FAILURE";
    printError(*RC_message);
    return returnCode;
  }

  // Writing an empty page to the file
  size_t writeEmptyPageStatus = fwrite(newblankPage, blockSize, blockNum, fp);

  if (writeEmptyPageStatus == 0)
  {
    RC_message = "ERR: FAILED TO WRITE ON PAGE";
    printError(*RC_message);
    returnCode = RC_WRITE_FAILED;
  }
  else
  {
    returnCode = RC_OK;
  }

  // free memory to avoid memory leakage
  free(newblankPage);

  // Closing the file
  int cFile = fclose(fp);

  if (cFile == EOF)
  {
    // Handle file closure failure.
    RC_message = "ERR: FAILED TO CLOSE FILE";
    printError(*RC_message);
    returnCode = RC_CLOSE_FILE_FAILED;
  }

  return returnCode;
}

extern RC openPageFile(char *fileName, SM_FileHandle *fHandle)
{

  RC returnCode = RC_OK;
  // Open Page for read and write purpose
  openFile(fp, fileName, "r+");

  if (!fp)
  {
    RC_message = "ERR: FILE NOT FOUND";
    printError(*RC_message);
    return returnCode = RC_FILE_NOT_FOUND;
  }

  // Initialize fields of fHandle
  fHandle->fileName = fileName;
  fHandle->totalNumPages = 0;
  fHandle->curPagePos = 0;
  fHandle->mgmtInfo = fp;

  // Read the total number of pages from the file (if available)
  fseek(fp, 0L, SEEK_END);
  fHandle->totalNumPages = ftell(fp) / PAGE_SIZE;

  // Return success code
  return returnCode;
}

extern RC closePageFile(SM_FileHandle *fHandle)
{
  // Check if the file handle is NULL or if the file is not initialized
  openFile(fp, fHandle->fileName, "r+");
  printf("************CLOSING PAGE FILE****************\n");
  int returnCode = RC_OK;
  if (fHandle == NULL || !fp)
  {
    returnCode = RC_FILE_NOT_FOUND;
    RC_message = "ERR: FILE DOES NOT EXIST!";
    printError(*RC_message);
    return returnCode;
  }

  // Close the file
  int closeStatus = fclose(fHandle->mgmtInfo);

  if (closeStatus == 0)
  {
    returnCode = RC_OK;
    RC_message = "CLOSED SUCCESSFULLY";
    printError(*RC_message);
    return returnCode;
  }
  else
  {
    returnCode = RC_CLOSE_FILE_FAILED;
    RC_message = "ERR: UNABLE TO CLOSE FILE";
    printError(*RC_message);
    return returnCode;
  }
}

extern RC destroyPageFile(char *fileName)
{
  int returnCode = RC_OK;
  // Close the file if it is Open
  if (fp != NULL)
  {
    fclose(fp);
    fp = NULL;
  }
  // Delete the File
  int destroyPageFileStatus = remove(fileName);

  // Check if success, return Appropriate Return codes
  if (destroyPageFileStatus == 0)
  {
    returnCode = RC_OK;
    RC_message = "DESTROYED SUCCESFULLY";
    printError(*RC_message);
    return returnCode;
  }
  else
  {
    returnCode = RC_FILE_NOT_FOUND;
    RC_message = "ERR: FAILED TO DESTROY PAGE FILE!!";
    printError(*RC_message);
    return returnCode;
  }
}