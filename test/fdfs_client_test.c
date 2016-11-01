#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/wait.h>

#include "make_log.h"

#define FDFS_LOGMODULE "test"
#define FDFS_LOGPROC "fdfs_test"
#define FILE_ID_LEN 4096

int main(int argc,char *argv[])
{
	char *file_name = NULL;
	char file_id[FILE_ID_LEN] = {0};
	
	if(argc < 2)
		{
			printf("缺少参数 请传入文件文件路径");
			exit(1);
			}
			
			file_name = argv[1];
			
			pid_t pid;//接受fork返回值
			int pfd[2];//接受管道都写端文件描述符
			
			if(pipe(pfd) <0 )
				{
					LOG(FDFS_LOGMODULE,FDFS_LOGPROC,"pipe error");
					exit(1);
					}
					
					
					pid = fork();//创建子进程
					if(pid == 0)
						{
							//子进程运行代码块
							close(pfd[0]);//关闭管道读端
							dup2(pfd[1],STDOUT_FILENO);
							//exec 子进程执行工作
							execlp("fdfs_upload_file","fdfs_upload_file","./conf/client.conf",file_name,NULL);
							//如果execlp执行失败则不会跳出 打印出错日志
							LOG(FDFS_LOGMODULE,FDFS_LOGPROC,"execlp error");
							
							}else if(pid > 0)
								{
									//父进程执行代码块
									close(pfd[1]);//关闭管道写端
									
									//从管道读出数据
									read(pfd[0],file_id,FILE_ID_LEN);
									
									LOG(FDFS_LOGMODULE,FDFS_LOGPROC,"fdfs_upload_file[%s] succ!",file_id);
									
									//阻塞等待子进程结束
									wait(NULL);
									}
									return 0;
	}