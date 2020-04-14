#include <unistd.h>		// chdir(), lstat()
#include <stdio.h>		// fprintf, printf 
#include <dirent.h>		// DIR*, struct dirent, opendir(), readdir(), closedir()
#include <string.h>		// strcmp()
#include <sys/stat.h>	// struct stat, opendir(), lstat(), S_ISDIR(), closedir()
#include <stdlib.h>		// exit()

void printdir(char *dir, int depth) // dir: 문자열로 파일경로 매개변수, depth: 파일이 얼마나 안족에 위치하는 지 출력할 때 사용.
{
	DIR *dp;					// 디렉토리 포인터 DIR* dp, 해당 디렉토리 스트림을 열 때 사용됨.
	struct dirent *entry;		// 디렉토리에 속하는 파일들의 이름을 확인하기 위해 선언된 dirent 구조체형 entry 
	struct stat statbuf;		// 상태를 저장하는 stat 구조체형 버퍼

	if ((dp = opendir(dir)) == NULL) { // dir 디렉토리 경로를 열었을 때 NULL이면 경로가 없다는 말이므로
		fprintf(stderr, "cannot open directory: %s\n", dir); // stderr에 "dir 경로를 열 수 없다"를 출력한 후 
		return;												// return하여 프로그램 종료
	}

	chdir(dir); // 존재하고 있는 해당 경로로 위치 설정

	while((entry = readdir(dp)) != NULL) {	// 해당 디렉토리에 정보가 있으면
		lstat(entry->d_name, &statbuf);		// d_name 파일의 상태를 statbuf에 저장
		if(S_ISDIR(statbuf.st_mode)) {		// 디렉토리(폴더) 일 경우, 
			if (strcmp(".", entry->d_name) == 0 ||		// 만약 파일 이름이 '.' 
					strcmp("..", entry->d_name) == 0)	// 혹은 '..'이면
				continue;								// 밑에 줄은 수행하지 않고 바로 while 조건문 실행
			printf("%*s%s/\n", depth,"", entry->d_name);// '.', '..'이 아닌 파일 이름을 depth 수 만큼 띄워서 출력
			/* Recurse at a new indent level */
			printdir(entry->d_name, depth+4);			// 현재 디렉토리 안의 내부를 스캔하기 위해 depth를 4만큼 증가하고 자기 자신을 다시 호출
		}
		else // 파일일 경우
			printf("%*s%s\n", depth,"",entry->d_name); // depth 수 만큼 띄우고 파일 이름 출력
	}
	chdir(".."); // 현재 작업 디렉토리를 이전 경로로 변경
	closedir(dp); // 디렉토리 닫음
}

int main(int argc, char* argv[]) // argc: argument 갯수, argv: 입력받은 argument 문자열
{
	char *topdir = ".";  // topdir를 "."로 선언 및 초기화
	if (argc >= 2)		// 만약 입력받은 argument가 있다면
		topdir=argv[1];	// topdir 값으로 설정

	printf("Directory scan of %s\n", topdir);	// "topdir의 디렉토리 스캔" 출력
	printdir(topdir, 0);						// topdir 경로 스캔. 처음 시작하는 경로니 depth=0으로 설정	
	printf("done.\n");							// 스캔 끝낸 후 "done." 출력

	exit(0);	// 에러 없이 정상 종료
}
