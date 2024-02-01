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

**Additional Notes:**

- The function uses the `openFile` function, which is defined as a Marco in the header library.

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
