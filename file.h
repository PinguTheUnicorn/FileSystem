#ifndef __FILE_H__
#define __FILE_H__

typedef struct datas {
	// Data stored
	void *data;

	// Pointer to next node
	void *next;

	// Pointer to previous
	void *last;
} datas;

typedef struct Directory {
	// The name of the directory
	char *name;

	// List of files in directory
	datas *files;

	// List of directories in directory
	datas *kidDir;

	// The parent directory of the
	// current directory (NULL for the root directory)
	struct Directory *parentDir;
} Directory;

// DO NOT MODIFY THIS STRUCTURE
typedef struct File {
	// The name of the file
	char *name;

	// The size of the file
	int size;

	// The content of the file
	char *data;

	// The directory in which the file is located
	Directory *dir;
} File;

#endif /* __FILE_H__ */
