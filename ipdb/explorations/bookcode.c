
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
int
main(int argc, char *argv[])
{
    int fd;
    char *addr;
    struct stat sb;
    if (argc != 2 || strcmp(argv[1], "--help") == 0) printf("%s shm-name\n", argv[0]);
    fd = shm_open(argv[1], O_CREAT | O_RDONLY, 0); /* Open existing object */ if (fd == -1)
            perror("shm_open");
    /* Use shared memory object size as length argument for mmap() and as number of bytes to write() */
        if (fstat(fd, &sb) == -1)
            perror("fstat");
    addr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0); if (addr == MAP_FAILED)
            perror("mmap");
        if (close(fd) == -1)
            perror("close");
    write(STDOUT_FILENO, addr, sb.st_size); printf("\n");
    exit(0);
}
