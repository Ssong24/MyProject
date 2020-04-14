#include <unistd.h>		// chdir(), lstat()
#include <stdio.h>		// fprintf, printf 
#include <dirent.h>		// DIR*, struct dirent, opendir(), readdir(), closedir()
#include <string.h>		// strcmp()
#include <sys/stat.h>	// struct stat, opendir(), lstat(), S_ISDIR(), closedir()
#include <stdlib.h>		// exit()

void printdir(char *dir, int depth) // dir: ���ڿ��� ���ϰ�� �Ű�����, depth: ������ �󸶳� ������ ��ġ�ϴ� �� ����� �� ���.
{
	DIR *dp;					// ���丮 ������ DIR* dp, �ش� ���丮 ��Ʈ���� �� �� ����.
	struct dirent *entry;		// ���丮�� ���ϴ� ���ϵ��� �̸��� Ȯ���ϱ� ���� ����� dirent ����ü�� entry 
	struct stat statbuf;		// ���¸� �����ϴ� stat ����ü�� ����

	if ((dp = opendir(dir)) == NULL) { // dir ���丮 ��θ� ������ �� NULL�̸� ��ΰ� ���ٴ� ���̹Ƿ�
		fprintf(stderr, "cannot open directory: %s\n", dir); // stderr�� "dir ��θ� �� �� ����"�� ����� �� 
		return;												// return�Ͽ� ���α׷� ����
	}

	chdir(dir); // �����ϰ� �ִ� �ش� ��η� ��ġ ����

	while((entry = readdir(dp)) != NULL) {	// �ش� ���丮�� ������ ������
		lstat(entry->d_name, &statbuf);		// d_name ������ ���¸� statbuf�� ����
		if(S_ISDIR(statbuf.st_mode)) {		// ���丮(����) �� ���, 
			if (strcmp(".", entry->d_name) == 0 ||		// ���� ���� �̸��� '.' 
					strcmp("..", entry->d_name) == 0)	// Ȥ�� '..'�̸�
				continue;								// �ؿ� ���� �������� �ʰ� �ٷ� while ���ǹ� ����
			printf("%*s%s/\n", depth,"", entry->d_name);// '.', '..'�� �ƴ� ���� �̸��� depth �� ��ŭ ����� ���
			/* Recurse at a new indent level */
			printdir(entry->d_name, depth+4);			// ���� ���丮 ���� ���θ� ��ĵ�ϱ� ���� depth�� 4��ŭ �����ϰ� �ڱ� �ڽ��� �ٽ� ȣ��
		}
		else // ������ ���
			printf("%*s%s\n", depth,"",entry->d_name); // depth �� ��ŭ ���� ���� �̸� ���
	}
	chdir(".."); // ���� �۾� ���丮�� ���� ��η� ����
	closedir(dp); // ���丮 ����
}

int main(int argc, char* argv[]) // argc: argument ����, argv: �Է¹��� argument ���ڿ�
{
	char *topdir = ".";  // topdir�� "."�� ���� �� �ʱ�ȭ
	if (argc >= 2)		// ���� �Է¹��� argument�� �ִٸ�
		topdir=argv[1];	// topdir ������ ����

	printf("Directory scan of %s\n", topdir);	// "topdir�� ���丮 ��ĵ" ���
	printdir(topdir, 0);						// topdir ��� ��ĵ. ó�� �����ϴ� ��δ� depth=0���� ����	
	printf("done.\n");							// ��ĵ ���� �� "done." ���

	exit(0);	// ���� ���� ���� ����
}
