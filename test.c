/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2015年05月04日 星期一 16时53分50秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>

#define BUFSIZE 128

int main(void)
{
    int ret;
    int i;
    char buf[BUFSIZE] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

    printf("buf = %s\n", buf);
    ret = snprintf(buf, BUFSIZE, "%s", "abc");

    printf("ret = %d\nbuf = %s\n", ret, buf);

    return 0;
}
