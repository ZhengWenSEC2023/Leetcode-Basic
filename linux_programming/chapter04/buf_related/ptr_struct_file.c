#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define prt(CONTENT, MSG) printf(CONTENT":\t%p\n", MSG)

int main(int argc, char *argv[]) {
    FILE *fp_src, *fp_dst;
    char buffer[10];
    int i;
    if ((fp_src = fopen(argv[1], "r+")) == NULL) {
        perror("open1");
        exit(EXIT_FAILURE);
    }
    if ((fp_dst = fopen(argv[2], "w+")) == NULL) {
        perror("open2");
        exit(EXIT_FAILURE);
    }
    setvbuf(fp_src, buffer, _IOLBF, 10);            // core dumped, if last para != 10

    do {
        // prt("src_IO_read_ptr", fp_src->_IO_read_ptr);        // changed + 10
        // prt("src_IO_read_end", fp_src->_IO_read_end);        // unchanged
        // prt("src_IO_read_base", fp_src->_IO_read_base);      // unchanged
        // prt("src_IO_write_ptr", fp_src->_IO_write_ptr);      // unchanged
        // prt("src_IO_write_base", fp_src->_IO_write_base);    // unchanged
        // prt("src_IO_write_end", fp_src->_IO_write_end);      // unchanged
        // prt("src_IO_buf_base", fp_src->_IO_buf_base);        // unchanged
        // prt("src_IO_buf_end\t", fp_src->_IO_buf_end);        // unchanged

        memset(buffer, '\0', 10);
        i = fread(buffer, 1, 10, fp_src);
        fwrite(buffer, 1, i, fp_dst);

        // prt("dst_IO_read_ptr", fp_dst->_IO_read_ptr);         // unchanged
        // prt("dst_IO_write_ptr", fp_dst->_IO_write_ptr);       // changed + 10
    } while (i == 10);
    fclose(fp_src);
    fclose(fp_dst);
}