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
			printf("ȱ�ٲ��� �봫���ļ��ļ�·��");
			exit(1);
			}
			
			file_name = argv[1];
			
			pid_t pid;//����fork����ֵ
			int pfd[2];//���ܹܵ���д���ļ�������
			
			if(pipe(pfd) <0 )
				{
					LOG(FDFS_LOGMODULE,FDFS_LOGPROC,"pipe error");
					exit(1);
					}
					
					
					pid = fork();//�����ӽ���
					if(pid == 0)
						{
							//�ӽ������д����
							close(pfd[0]);//�رչܵ�����
							dup2(pfd[1],STDOUT_FILENO);
							//exec �ӽ���ִ�й���
							execlp("fdfs_upload_file","fdfs_upload_file","./conf/client.conf",file_name,NULL);
							//���execlpִ��ʧ���򲻻����� ��ӡ������־
							LOG(FDFS_LOGMODULE,FDFS_LOGPROC,"execlp error");
							
							}else if(pid > 0)
								{
									//������ִ�д����
									close(pfd[1]);//�رչܵ�д��
									
									//�ӹܵ���������
									read(pfd[0],file_id,FILE_ID_LEN);
									
									LOG(FDFS_LOGMODULE,FDFS_LOGPROC,"fdfs_upload_file[%s] succ!",file_id);
									
									//�����ȴ��ӽ��̽���
									wait(NULL);
									}
									return 0;
	}