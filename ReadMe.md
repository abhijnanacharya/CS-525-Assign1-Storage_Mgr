# CS 525 Storage Manager

    Contributors:
    - Abhijnan Acharya A20536263
    - Tanushree Halder A20554883
    - Roshan Hyalij A20547441
    - Rana Feyza Soylu A20465152

---

---

**function createPageFile**

**Description:**

This function creates a new page file with a specified filename. If the file already exists, it truncates it to an empty file.

**Prototype:**

```c
extern RC createPageFile(char *fName);
```

**Parameters:**

- **fName:** The name of the page file to create.

**Return Values:**

- **RC_OK:** The function completed successfully.
- **RC_FILE_NOT_FOUND:** The specified file could not be found.
- **RC_MEM_ALLOC_FAILED:** Memory allocation failed.
- **RC_WRITE_FAILED:** An error occurred while writing to the file.
- **RC_CLOSE_FILE_FAILED:** An error occurred while closing the file.

**Steps:**

1. Opens the specified file in "wb+" mode, which creates a new file for writing in binary mode if it doesn't exist, or truncates an existing file.
2. Checks if the file pointer is NULL, indicating an error opening the file.
3. Allocates memory for a blank page of size `PAGE_SIZE`.
4. Writes the blank page to the file.
5. Frees the allocated memory.
6. Closes the file.

**Error Handling:**

- Handles file opening errors, memory allocation failures, file writing errors, and file closure failures.

**Usage Example:**

```c
char *fileName = "my_page_file.dat";
RC rc = createPageFile(fileName);

if (rc == RC_OK) {
    // Page file created successfully
} else {
    // Handle error
}
```

---

**function openPageFile**

**Description:**

This function opens an existing page file for reading and writing, and initializes a file handle structure to manage the file.

**Prototype:**

```c
extern RC openPageFile(char *fileName, SM_FileHandle *fHandle);
```

**Parameters:**

- **fileName:** The name of the page file to open.
- **fHandle:** A pointer to a file handle structure that will be initialized with information about the opened file.

**Return Values:**

- **RC_OK:** The function completed successfully.
- **RC_FILE_NOT_FOUND:** The specified file could not be found.

**Steps:**

1. Opens the specified file in "r+" mode, which opens an existing file for both reading and writing.
2. Checks if the file pointer is NULL, indicating an error opening the file.
3. Initializes the fields of the `fHandle` structure:
   - `fileName`: The name of the file.
   - `totalNumPages`: The total number of pages in the file (determined by seeking to the end of the file and dividing the file size by `PAGE_SIZE`).
   - `curPagePos`: The current page position in the file (initialized to 0).
   - `mgmtInfo`: The file pointer, which is used for subsequent file operations.

**Error Handling:**

- Handles file opening errors.

**Usage Example:**

```c
SM_FileHandle fHandle;
char *fileName = "my_page_file.dat";
RC rc = openPageFile(fileName, &fHandle);

if (rc == RC_OK) {
    // Page file opened successfully
    // Now you can use fHandle to read and write pages
} else {
    // Handle error
}
```

---

**function closePageFile**

**Description:**

This function closes an open page file, releasing associated resources and invalidating the file handle.

**Prototype:**

```c
extern RC closePageFile(SM_FileHandle *fHandle);
```

**Parameters:**

- **fHandle:** A pointer to the file handle structure of the page file to be closed.

**Return Values:**

- **RC_OK:** The file was closed successfully.
- **RC_FILE_NOT_FOUND:** The specified file handle is NULL or invalid.
- **RC_CLOSE_FILE_FAILED:** An error occurred while closing the file.

**Steps:**

1. Checks if the file handle is NULL or invalid.
2. Opens the file again in "r+" mode to ensure it's accessible before closing.
3. Prints a message indicating that the file is being closed.
4. Closes the file using the `fclose` function.
5. Prints a success or error message based on the closing status.

**Error Handling:**

- Handles invalid file handles and file closure errors.
- Prints error messages using a `printError` function (not shown in the provided code).

**Usage Example:**

```c
SM_FileHandle fHandle;
// ... (open the file using openPageFile)

RC rc = closePageFile(&fHandle);

if (rc == RC_OK) {
    // File closed successfully
} else {
    // Handle error
}
```

---

**function destroyPageFile**

**Description:**

This function deletes a page file from the system, ensuring it's closed if it's currently open.

**Prototype:**

```c
extern RC destroyPageFile(char *fileName);
```

**Parameters:**

- **fileName:** The name of the page file to be destroyed.

**Return Values:**

- **RC_OK:** The file was destroyed successfully.
- **RC_FILE_NOT_FOUND:** The specified file does not exist or could not be deleted.

**Steps:**

1. Checks if the file is currently open using a global file pointer `fp`.
2. If the file is open, closes it using `fclose`.
3. Attempts to delete the file using the `remove` function.
4. Prints a success or error message based on the deletion status.

**Error Handling:**

- Handles file deletion errors.
- Prints error messages using a `printError` function (not shown in the provided code).

**Usage Example:**

```c
char *fileName = "my_page_file.dat";
RC rc = destroyPageFile(fileName);

if (rc == RC_OK) {
    // File destroyed successfully
} else {
    // Handle error
}
```

---

**function readBlock**

**Description:**

This function reads a specific page (block) from an open page file into a memory buffer.

**Prototype:**

```c
extern RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
```

**Parameters:**

- **pageNum:** The index of the page to read (starting from 0).
- **fHandle:** A pointer to the file handle structure of the open page file.
- **memPage:** A pointer to a memory buffer where the page data will be read into.

**Return Values:**

- **RC_OK:** The page was read successfully.
- **RC_FILE_HANDLE_NOT_INIT:** The file handle is NULL or invalid.
- **RC_READ_NON_EXISTING_PAGE:** The specified page number is out of bounds.
- **RC_MEM_ALLOC_FAILED:** Memory allocation failed within the `fseek` function.

**Steps:**

1. Checks if the file handle is valid.
2. Checks if the requested page number is within the valid range of pages in the file.
3. Seeks to the correct position in the file using `fseek`.
4. Reads the page data from the file into the memory buffer using `fread`.
5. Updates the current page position in the file handle structure.

**Error Handling:**

- Handles invalid file handles, invalid page numbers, and memory allocation errors.

**Usage Example:**

```c
SM_FileHandle fHandle;
// ... (open the file using openPageFile)

SM_PageHandle pageBuffer = (SM_PageHandle)malloc(PAGE_SIZE);
int pageNumToRead = 3;

RC rc = readBlock(pageNumToRead, &fHandle, pageBuffer);

if (rc == RC_OK) {
    // Page data is now in pageBuffer
} else {
    // Handle error
}
```

---

**function getBlockPos**:

**Description:**

This function retrieves the current block position within a file handle structure.

**Prototype:**

```c
extern int getBlockPos(SM_FileHandle *fHandle);
```

**Parameters:**

- **fHandle:** A pointer to the file handle structure.

**Return Values:**

- The current block position, or `RC_FILE_HANDLE_NOT_INIT` if the handle is invalid.

**Steps:**

1. Checks if the file handle is NULL.
2. If the handle is valid, returns the current block position stored in the `curPagePos` member of the `fHandle` structure.

**Error Handling:**

- Handles invalid file handles by returning `RC_FILE_HANDLE_NOT_INIT`.

**Usage Example:**

```c
SM_FileHandle fHandle;
// ... (initialize the file handle)

int blockPos = getBlockPos(fHandle);

if (blockPos != RC_FILE_HANDLE_NOT_INIT) {
    printf("Current block position: %d\n", blockPos);
} else {
    printf("Error: Invalid file handle\n");
}
```

---

**function readFirstBlock**

**Description:**

This function reads the first block (page) from an open page file into a memory buffer.

**Prototype:**

```c
extern RC readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage);
```

**Parameters:**

- **fHandle:** A pointer to the file handle structure of the open page file.
- **memPage:** A pointer to a memory buffer where the first page data will be read into.

**Return Values:**

- **RC_OK:** The first block was read successfully.
- **RC_FILE_HANDLE_NOT_INIT:** The file handle is NULL or invalid.
- **Other error codes:** Any errors returned by the underlying `readBlock` function.

**Steps:**

1. Checks if the file handle is valid.
2. Calls the `readBlock` function with a page number of 0 to read the first block.
3. Returns the result of the `readBlock` call.

**Error Handling:**

- Handles invalid file handles by returning `RC_FILE_HANDLE_NOT_INIT`.
- Propagates other errors returned by `readBlock`.

**Usage Example:**

```c
SM_FileHandle fHandle;
// ... (open the file using openPageFile)

SM_PageHandle firstPageBuffer = (SM_PageHandle)malloc(PAGE_SIZE);

RC rc = readFirstBlock(&fHandle, firstPageBuffer);

if (rc == RC_OK) {
    // First page data is now in firstPageBuffer
} else {
    // Handle error
}
```

---

**function readPreviousBlock**

**Description:**

This function reads the block (page) immediately preceding the current position in an open page file into a memory buffer.

**Prototype:**

```c
extern RC readPreviousBlock(SM_FileHandle *fHandle, SM_PageHandle memPage);
```

**Parameters:**

- **fHandle:** A pointer to the file handle structure of the open page file.
- **memPage:** A pointer to a memory buffer where the previous page data will be read into.

**Return Values:**

- **RC_OK:** The previous block was read successfully.
- **RC_FILE_HANDLE_NOT_INIT:** The file handle is NULL or invalid.
- **RC_READ_NON_EXISTING_PAGE:** There is no previous block to read (already at the beginning of the file).
- **Other error codes:** Any errors returned by the underlying `readBlock` function.

**Steps:**

1. Checks if the file handle is valid.
2. Retrieves the current page position from the file handle.
3. If the current position is already at the first page (0), returns `RC_READ_NON_EXISTING_PAGE`.
4. Calculates the page number of the previous block (current position - 1).
5. Calls the `readBlock` function to read the previous block into the memory buffer.
6. Returns the result of the `readBlock` call.

**Error Handling:**

- Handles invalid file handles and attempts to read beyond the beginning of the file.
- Propagates other errors returned by `readBlock`.

**Usage Example:**

```c
SM_FileHandle fHandle;
// ... (open the file and read some pages)

SM_PageHandle prevPageBuffer = (SM_PageHandle)malloc(PAGE_SIZE);

RC rc = readPreviousBlock(&fHandle, prevPageBuffer);

if (rc == RC_OK) {
    // Previous page data is now in prevPageBuffer
} else {
    // Handle error
}
```

---

**function readCurrentBlock**

**Description:**

This function reads the block (page) at the current position in an open page file into a memory buffer.

**Prototype:**

```c
extern RC readCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage);
```

**Parameters:**

- **fHandle:** A pointer to the file handle structure of the open page file.
- **memPage:** A pointer to a memory buffer where the current page data will be read into.

**Return Values:**

- **RC_OK:** The current block was read successfully.
- **RC_FILE_HANDLE_NOT_INIT:** The file handle is NULL or invalid.
- **RC_READ_NON_EXISTING_PAGE:** The current position is invalid (out of bounds).
- **Other error codes:** Any errors returned by the underlying `readBlock` function.

**Steps:**

1. Checks if the file handle is valid.
2. Retrieves the current page position from the file handle.
3. Checks if the current position is within the valid range of pages in the file.
4. Calls the `readBlock` function to read the block at the current position into the memory buffer.
5. Returns the result of the `readBlock` call.

**Error Handling:**

- Handles invalid file handles and attempts to read from invalid positions.
- Propagates other errors returned by `readBlock`.

**Usage Example:**

```c
SM_FileHandle fHandle;
// ... (open the file and navigate to a certain page)

SM_PageHandle currentPageBuffer = (SM_PageHandle)malloc(PAGE_SIZE);

RC rc = readCurrentBlock(&fHandle, currentPageBuffer);

if (rc == RC_OK) {
    // Current page data is now in currentPageBuffer
} else {
    // Handle error
}
```

---

**function readNextBlock**

**Description:**

This function reads the block (page) following the current position in an open page file into a memory buffer.

**Prototype:**

```c
extern RC readNextBlock(SM_FileHandle *fHandle, SM_PageHandle memPage);
```

**Parameters:**

- **fHandle:** A pointer to the file handle structure of the open page file.
- **memPage:** A pointer to a memory buffer where the next page data will be read into.

**Return Values:**

- **RC_OK:** The next block was read successfully.
- **RC_FILE_HANDLE_NOT_INIT:** The file handle is NULL or invalid.
- **RC_READ_NON_EXISTING_PAGE:** There is no next block to read (already at the end of the file).
- **Other error codes:** Any errors returned by the underlying `readBlock` function.

**Steps:**

1. Checks if the file handle is valid.
2. Retrieves the current page position from the file handle.
3. Checks if the current position is already at the last page of the file.
4. Calculates the page number of the next block (current position + 1).
5. Calls the `readBlock` function to read the next block into the memory buffer.
6. Returns the result of the `readBlock` call.

**Error Handling:**

- Handles invalid file handles and attempts to read beyond the end of the file.
- Propagates other errors returned by `readBlock`.

**Usage Example:**

```c
SM_FileHandle fHandle;
// ... (open the file and read some pages)

SM_PageHandle nextPageBuffer = (SM_PageHandle)malloc(PAGE_SIZE);

RC rc = readNextBlock(&fHandle, nextPageBuffer);

if (rc == RC_OK) {
    // Next page data is now in nextPageBuffer
} else {
    // Handle error
}
```

---

**function readLastBlock**

**Description:**

This function reads the last block (page) of an open page file into a memory buffer.

**Prototype:**

```c
extern RC readLastBlock(SM_FileHandle *fHandle, SM_PageHandle memPage);
```

**Parameters:**

- **fHandle:** A pointer to the file handle structure of the open page file.
- **memPage:** A pointer to a memory buffer where the last page data will be read into.

**Return Values:**

- **RC_OK:** The last block was read successfully.
- **RC_FILE_HANDLE_NOT_INIT:** The file handle is NULL or invalid.
- **RC_READ_NON_EXISTING_PAGE:** The file is empty (has no pages).
- **Other error codes:** Any errors returned by the underlying `readBlock` function.

**Steps:**

1. Checks if the file handle is valid.
2. Retrieves the total number of pages in the file from the file handle.
3. Calculates the page number of the last block (total number of pages - 1).
4. Checks if the file is empty (no pages).
5. Calls the `readBlock` function to read the last block into the memory buffer.
6. Returns the result of the `readBlock` call.

**Error Handling:**

- Handles invalid file handles and attempts to read from empty files.
- Propagates other errors returned by `readBlock`.

**Usage Example:**

```c
SM_FileHandle fHandle;
// ... (open the file)

SM_PageHandle lastPageBuffer = (SM_PageHandle)malloc(PAGE_SIZE);

RC rc = readLastBlock(&fHandle, lastPageBuffer);

if (rc == RC_OK) {
    // Last page data is now in lastPageBuffer
} else {
    // Handle error
}
```

---

**function seekToPage**

**Description:**

This function moves the file pointer to the beginning of a specific page within an open page file.

**Prototype:**

```c
extern RC seekToPage(SM_FileHandle *fHandle, int pageNum);
```

**Parameters:**

- **fHandle:** A pointer to the file handle structure of the open page file.
- **pageNum:** The index of the page to seek to (starting from 0).

**Return Values:**

- **RC_OK:** The seek operation was successful.
- **RC_FILE_HANDLE_NOT_INIT:** The file handle is NULL or invalid.
- **RC_SEEK_ERROR:** An error occurred during the `fseek` operation.

**Steps:**

1. Calculates the byte offset of the desired page within the file based on the page number and page size.
2. Calls the `fseek` function to move the file pointer to the calculated offset, using `SEEK_SET` to position from the beginning of the file.
3. Returns `RC_OK` if `fseek` succeeds, otherwise returns `RC_SEEK_ERROR`.

**Error Handling:**

- Handles invalid file handles and errors from the `fseek` function.

**Usage Example:**

```c
SM_FileHandle fHandle;
// ... (open the file)

RC rc = seekToPage(&fHandle, 5); // Seek to the 6th page (index 5)

if (rc == RC_OK) {
    // File pointer is now at the beginning of the 6th page
    // You can now read or write data from this position
} else {
    // Handle error
}
```

---

**function seekToEnd**

**Description:**

This function attempts to move the file pointer to a specified position relative to the end of a file.

**Prototype:**

```c
int seekToEnd(FILE *handle, long long fSet, int seekLoc);
```

**Parameters:**

- **handle:** A pointer to the open file stream.
- **fSet:** The offset to seek from the end of the file (can be positive or negative).
- **seekLoc:** The base position for seeking, specified as `SEEK_SET`, `SEEK_CUR`, or `SEEK_END`.

**Return Values:**

- **0:** The seek operation was successful.
- **Non-zero:** An error occurred during the `fseek` operation.

**Steps:**

1. Calls the standard `fseek` function, passing the provided parameters directly.
2. Returns the result of the `fseek` call.

**Error Handling:**

- Inherits error handling from the underlying `fseek` function.

**Usage Example:**

```c
FILE *fp = fopen("myfile.txt", "r+");

int result = seekToEnd(fp, -10, SEEK_END); // Move 10 bytes back from the end

if (result == 0) {
    // File pointer is now 10 bytes from the end
    // You can now read or write data from this position
} else {
    // Handle error
}
```

---

**function writeBlock**

**Description:**

Writes a block (page) of data from a memory buffer to a specified page within an open page file.

**Prototype:**

```c
extern RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
```

**Parameters:**

- **pageNum:** The index of the page to write to (starting from 0).
- **fHandle:** A pointer to the file handle structure of the open page file.
- **memPage:** A pointer to the memory buffer containing the data to write.

**Return Values:**

- **RC_OK:** The block was written successfully.
- **RC_WRITE_FAILED:** An error occurred during the write operation.
- **RC_READ_NON_EXISTING_PAGE:** The specified page number is invalid.

**Steps:**

1. Checks for a valid file handle and valid page number.
2. Seeks to the beginning of the specified page using `seekToPage`.
3. Writes the contents of the memory buffer to the file using `fwrite`.
4. Updates the `curPagePos` in the file handle to reflect the written page.
5. Returns `RC_OK` if successful, otherwise returns an appropriate error code.

**Error Handling:**

- Handles invalid file handles, invalid page numbers, and errors from `seekToPage` and `fwrite`.

---

**function writeCurrentBlock**

**Description:**

Writes a block (page) of data from a memory buffer to the current page in an open page file.

**Prototype:**

```c
extern RC writeCurrentBlock(SM_FileHandle *fileHandle, SM_PageHandle memoryPage);
```

**Parameters:**

- **fileHandle:** A pointer to the file handle structure of the open page file.
- **memoryPage:** A pointer to the memory buffer containing the data to write.

**Return Values:**

- Same as `writeBlock`.

**Steps:**

1. Retrieves the current block position using an assumed `getBlockPos` function.
2. Calls `writeBlock` to write the data to the current block.
3. Returns the result of the `writeBlock` call.

---

**function appendEmptyBlock**

**Description:**

Appends an empty block (page) to the end of an open page file.

**Prototype:**

```c
extern RC appendEmptyBlock(SM_FileHandle *fHandle);
```

**Parameters:**

- **fHandle:** A pointer to the file handle structure of the open page file.

**Return Values:**

- **RC_OK:** The empty block was appended successfully.
- **RC_WRITE_FAILED:** An error occurred during the write operation.

**Steps:**

1. Allocates memory for an empty page.
2. Seeks to the end of the file using `seekToEnd`.
3. Writes the empty page to the file using `fwrite`.
4. Updates the file handle information (`totalNumPages` and `curPagePos`).
5. Rewinds the file and updates the total number of pages in the file header.
6. Seeks back to the beginning of the file.
7. Frees the allocated memory.
8. Returns `RC_OK` if successful, otherwise returns `RC_WRITE_FAILED`.

---

**function ensureCapacity**

**Description:**

Ensures that a file handle has a capacity of at least the specified number of pages, appending empty blocks if necessary.

**Prototype:**

```c
extern RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle);
```

**Parameters:**

- **numberOfPages:** The desired capacity of the file handle.
- **fHandle:** A pointer to the file handle structure of the open page file.

**Return Values:**

- **RC_OK:** The capacity was ensured successfully.
- **RC_ALLOCATION_FAILED:** An error occurred while allocating new blocks.

**Steps:**

1. Loops while the current capacity is less than the desired capacity.
2. Attempts to append an empty block using `appendEmptyBlock`.
3. Returns `RC_ALLOCATION_FAILED` if `appendEmptyBlock` fails, otherwise continues.
4. Returns `RC_OK` if the loop completes without errors.
