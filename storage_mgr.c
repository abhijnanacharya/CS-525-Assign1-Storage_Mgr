#include <stdio.h>
#include <stdlib.h>
#include "storage_mgr.h"
#include "dberror.h"

FILE *fp = NULL;

extern void initStorageManager()
{
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
    return RC_FILE_NOT_FOUND;
  }

  // Allocating memory to PAGE_SIZE element
  SM_PageHandle blankPage = (SM_PageHandle)calloc(PAGE_SIZE, sizeof(char));

  if (blankPage == NULL)
  {
    // Handle memory allocation failure.
    fclose(fp);
    return RC_MEM_ALLOC_FAILED;
  }

  // Writing an empty page to the file
  size_t wPStatus = fwrite(blankPage, sizeof(char), PAGE_SIZE, fp);

  if (wPStatus == 0)
  {
    returnCode = RC_WRITE_FAILED;
  }
  else
  {
    returnCode = RC_OK;
  }

  // free memory to avoid memory leakage
  free(blankPage);

  // Closing the file
  int cFile = fclose(fp);

  if (cFile == EOF)
  {
    // Handle file closure failure.
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
    return returnCode = RC_FILE_NOT_FOUND;

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