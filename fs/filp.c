#include "fs.h"

static filp_t fd_table[NR_FILPS];

int get_fd(struct proc *curr,int start, int *k, filp_t **fpt){
    register int i = -1;
    register filp_t *f;

    for( i=start; i< OPEN_MAX; i++){
        if(current_proc->fp_filp[i] == NIL_FILP){
            *k = i;
            break;
        }
    }

    if(i==-1)
        return ERR;

     for (f = &fd_table[0]; f < &fd_table[NR_FILPS]; f++) {
        if (f->filp_count == 0) {
            // f->filp_mode = bits;
            f->filp_pos = 0L;
            f->filp_flags = 0;
            *fpt = f;
            return(OK);
        }
    }
    return ERR;
}


filp_t *get_filp(int fd){
    return &fd_table[fd];
}

filp_t *find_filp(inode_t *inode){
    int i;
    for(i = 0; i< NR_FILPS; i++){
        if(fd_table[i].filp_ino == inode){
            return &fd_table[i];
        }
    }
    return NULL;
}

filp_t *get_free_filp(){
    register filp_t* rep;
    for(rep = &fd_table[0]; rep < &fd_table[NR_FILPS]; rep++ ){
        if(rep->filp_ino = NIL_INODE){
            return rep;
        }
    }
}

void init_filp(){
    register filp_t* rep;
    int i = 0;
    for(rep = &fd_table[0]; rep < &fd_table[NR_FILPS]; rep++ ){
        rep->filp_ino = NIL_INODE;
        rep->filp_table_index = i;
        i++;
    }
}
