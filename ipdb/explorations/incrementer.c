#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <semaphore.h>

#include "ipdb.h"

#define SHM_SIZE 1024

int get_fd_size(int fd);

int main() {
    int smkey = 31415;
    int shmfd;
    char *addr;
    //char *name = "/test";
    int *val;
    char *name = malloc(NAME_SIZE);
    SHARED_MEM_NAME(name);
    printf("%s\n", name);
    /*
    int ret = shm_unlink(name);
    perror("unlink");
    exit(0);
    */
    char *path = "/dev/shm/IPDB__vagrant";
 
    shmfd = shm_open(name, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
    assert(shmfd >= 0);

    if (access(path, F_OK) == -1) {
        puts("File does not exist, truncating");
        int ret = ftruncate(shmfd, SHM_SIZE);
        if (ret == -1) {
            perror("ftruncate");
            exit(-1);
        }
    }
    
    addr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (errno) {
        perror("mmap failed");
        exit(-1);
    }
    int size = get_fd_size(shmfd);
    printf("There are %lu rows in the db\n", size/sizeof(struct ip_row));
    printf("%s", addr);
    struct ip_row *cur_row = (struct ip_row*)addr;
    for (int i = 0; i < size/sizeof(struct ip_row); i++) {
        printf("row name: %s\n", cur_row->row_name[i]);
    }
    munmap(addr, SHM_SIZE);
    close(shmfd);
    while(1){}
}

int get_fd_size(int fd) {
    struct stat statfd;
    int err = fstat(fd, &statfd);
    if (err == -1) {
        perror("Could not stat file");
        exit(-1);
    }
    return statfd.st_size;
}
