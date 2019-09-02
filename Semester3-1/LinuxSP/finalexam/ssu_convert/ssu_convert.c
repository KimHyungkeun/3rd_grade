#include "ssu_convert.h"

FILE *fp;

int c_fd;
int braceNum;

char *javaFname;
char *currentFname;
char *className[BUFSIZE];
char *conList[BUFSIZE];

char *forPList[BUFSIZE];

int classNameCnt = 0;
int conListCnt = 0;
int forPListCnt = 0;

int forkfd = 0;
int mainPos = 0;

pid_t pid;

int main(int argc, char *argv[])
{
	struct timeval begin_t, end_t;
	int option;

	if((fp = fopen(argv[1], "r")) == NULL){
		fprintf(stderr, "%s fopen error\n", argv[1]);
		exit(1);
	}

	javaFname = argv[1];
	currentFname = malloc(sizeof(char) * MAXLEN);

	//Get Option Flag
	while((option = getopt(argc, argv, "jcpflr")) != -1){
		switch(option){
			case 'j':
				optjFlag = TRUE;
				break;
			case 'c':
				optcFlag = TRUE;
				break;
			case 'p':
				optpFlag = TRUE;
				break;
			case 'f':
				optfFlag = TRUE;
				break;
			case 'l':
				optlFlag = TRUE;
				break;
			case 'r':
				optrFlag = TRUE;
				pid = fork();
				if((forkfd = open(FORKFLAG, MODE, 0644)) == 0){
					fprintf(stderr, "open error for %s\n", FORKFLAG);
					exit(1);
				}
				break;
			case '?':
				return 0;
				break;
		}
	}

	if(pid == 0 && optrFlag == TRUE){
		while(1){
			do_r_Option();
		}
	}
	else{

		int java_fd;

		gettimeofday(&begin_t, NULL);

		if((java_fd = open(TEMPJAVA, MODE, 0644)) == 0){
			fprintf(stderr, "open error for %s\n", TEMPJAVA);
			exit(1);
		}

		while(!feof(fp)){

			char strJava[BUFSIZE] = {0};
			char * temp;

			temp = malloc(sizeof(char) * MAXLEN);
			memset(temp, 0, sizeof(temp));

			int cnt = 0;

			if(fread(strJava, 1, 1, fp) == 0)
				break;
			else
				fseek(fp, -1, SEEK_CUR);


			strcpy(temp, fgets(strJava, sizeof(strJava), fp));

			lseek(java_fd, 0, SEEK_END);

			write(java_fd, temp, strlen(temp));

			temp = leftTrim(temp);

			strcpy(strJava, temp);

			javaCmp(strJava);

			if(optrFlag == TRUE){
				char * tempbuf;

				tempbuf = malloc(sizeof(char) * MAXLEN);
				memset(tempbuf, 0, sizeof(tempbuf));

				lseek(forkfd, 0, SEEK_SET);

				if(forkfd != 0)
					read(forkfd, tempbuf, 1);

				if(strcmp(tempbuf, "1") == 0)
					sleep(1);
				
			}

		}

		close(java_fd);

		if(pid != 0)
			kill(pid, SIGKILL);
	}

	fclose(fp);

	remove(FORKFLAG);
	remove(TEMPJAVA);

	closeFile();

	for(int i=0; i<classNameCnt; i++)
		printf("%s.c converting is finished!\n", className[i]);

	for(int i=0; i<classNameCnt; i++){
		if(checkExistMain(className[i]) == 1){
			mainPos = i;
			adjustMain(i);
			break;
		}
	}

	if(optjFlag == TRUE){
		do_j_c_Option(javaFname);
		printf("\n");
	}

	if(optcFlag == TRUE){
		for(int i=0; i<classNameCnt; i++){
			char * temp;
			temp = malloc(sizeof(char) * MAXLEN);
			
			sprintf(temp, "%s.c", className[i]);
			do_j_c_Option(temp);
		}
		
		printf("\n");
	}

	if(optpFlag == TRUE){
		for(int i=0; i<forPListCnt; i++)
			printf("%d %s\n", i+1, forPList[i]);

		printf("\n");
	}

	if(optfFlag == TRUE)
		do_f_Option();

	if(optlFlag == TRUE)
		do_l_Option();

	do_makeFile(argv[1]);
	
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);

	return 0;
}

void ssu_runtime(struct timeval* begin_t, struct timeval* end_t)
{
	end_t -> tv_sec -= begin_t -> tv_sec;

	if(end_t -> tv_usec < begin_t -> tv_usec){
		end_t -> tv_sec--;
		end_t -> tv_usec += SECOND_TO_MICRO;
	}

	end_t -> tv_usec -= begin_t -> tv_usec;
	printf("Runtime : %ld:%06ld(sec:usec)\n", end_t -> tv_sec, end_t -> tv_usec);
}

void do_r_Option(void)
{
		char buf[MAXLEN]={0};

		char * token;

		lseek(forkfd, 0, SEEK_SET);
			
		token = malloc(sizeof(char) * MAXLEN);
		memset(token, 0, sizeof(token));
		read(forkfd, buf, MAXLEN);

		token = strtok(buf, "/");


		if(token == NULL){
			char code[BUFSIZE] = {0};
			int j_fd;

			sleep(1);

			system("clear");

			if((j_fd = open(TEMPJAVA, O_RDONLY)) == 0){
				fprintf(stderr, "open error for %s\n", TEMPJAVA);
				exit(1);
			}

			if(j_fd != 0)
				read(j_fd, code, sizeof(code));


			printf("%s Converting....\n", javaFname);
			printf("--------\n");
			printf("%s\n", javaFname);
			printf("-------\n");

			int cnt = 0;
			int isLF = FALSE;

			for(int i=0; i<strlen(code); i++){
				if(i == 0)
					printf("%d ", ++cnt);

				printf("%c", code[i]);

				if(((i < strlen(code)-1) && (code[i] == '\n'))){
					isLF = TRUE;
					printf("%d ", ++cnt);
					isLF = FALSE;
				}
			}

			close(j_fd);
			return ;
		}

		if(token != NULL && (strcmp(token, "1") == 0)){

			char code[BUFSIZE] = {0};
			int j_fd;

			system("clear");

			if((j_fd = open(TEMPJAVA, O_RDONLY)) == 0){
				fprintf(stderr, "open error for %s\n", TEMPJAVA);
				exit(1);
			}

			if(j_fd != 0)
				read(j_fd, code, sizeof(code));


			printf("%s Converting....\n", javaFname);
			printf("--------\n");
			printf("%s\n", javaFname);
			printf("-------\n");

			int cnt = 0;
			int isLF = FALSE;

			for(int i=0; i<strlen(code); i++){
				if(i == 0)
					printf("%d ", ++cnt);

				printf("%c", code[i]);

				if(((i < strlen(code)-1) && (code[i] == '\n'))){
					isLF = TRUE;
					printf("%d ", ++cnt);
					isLF = FALSE;
				}
			}

			close(j_fd);

			char * temp;

			temp = malloc(sizeof(char) * MAXLEN);
			memset(code, 0, sizeof(code));
				
			if((token = strtok(NULL, "/")) != NULL){

				printf("\n--------\n");
				printf("%s\n", token);
				printf("--------\n");

				int temp_fd;

				memset(code, 0, sizeof(code));
					
				lseek(forkfd, 0, SEEK_SET);


				if((temp_fd = open(token, O_RDONLY)) == 0){
					fprintf(stderr, "open error for %s\n", buf);
					exit(1);
				}

				if(temp_fd != 0)
					read(temp_fd, code, sizeof(code));

				cnt = 0;
				isLF = FALSE;

				for(int i=0; i<strlen(code); i++){
					if(i == 0)
						printf("%d ", ++cnt);

					printf("%c", code[i]);

					if(((i < strlen(code)-1) && (code[i] == '\n'))){
						isLF = TRUE;
						printf("%d ", ++cnt);
						isLF = FALSE;
					}
				}

				removeEnd(token);
				removeEnd(token);
				sprintf(temp, "0/%s", token);

				write(forkfd, temp, strlen(temp));

				if(temp_fd != 0)
					close(temp_fd);
			}
		}
}

void do_l_Option(void)
{
	printf("%s line number is %d lines\n", javaFname, getLineNum(javaFname));

	for(int i=0; i<classNameCnt; i++){
		char * temp;
		temp = malloc(sizeof(char) * MAXLEN);

		sprintf(temp, "%s.c", className[i]);
		printf("%s line number is %d lines\n", temp, getLineNum(temp));
	}
	printf("\n");
}

void do_f_Option(void)
{
	printf("%s file size is %d bytes\n", javaFname, getFileSize(javaFname));

	for(int i=0; i<classNameCnt; i++){
		char * temp;
		temp = malloc(sizeof(char) * MAXLEN);

		sprintf(temp, "%s.c", className[i]);

		printf("%s file size is %d bytes\n", temp, getFileSize(temp));
	}
	printf("\n");
}

void do_j_c_Option(char * fname)
{
	FILE * jopt_fp;
	int cnt = 1;

	if((jopt_fp = fopen(fname, "r")) == NULL){
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}

	while(!feof(jopt_fp)){
		char strJava[BUFSIZE];

		if(fread(strJava, 1, 1, jopt_fp) == 0)
			break;
		else
			fseek(jopt_fp, -1, SEEK_CUR);

		strcpy(strJava, fgets(strJava, sizeof(strJava), jopt_fp));

		printf("%d %s", cnt, strJava);

		cnt++;
	}

	fclose(jopt_fp);
}

void isOptrFlag(char isWrite)
{
	char * tempFname;
	tempFname = malloc(sizeof(char) * MAXLEN);

	memset(tempFname, 0, sizeof(tempFname));

	lseek(forkfd, 0, SEEK_SET);

	sprintf(tempFname, "%c/%s", isWrite, currentFname);

	if(forkfd != 0)
		write(forkfd, tempFname, strlen(tempFname));
	
}

void do_makeFile(char * fname)
{
	char * makeFile;
	int make_fd;
	char * tempStr;
	char * compileFile;

	makeFile = malloc(sizeof(char) * MAXLEN);
	compileFile = malloc(sizeof(char) * MAXLEN);
	tempStr = malloc(sizeof(char) * MAXLEN);

	memset(makeFile, 0, sizeof(makeFile));
	memset(compileFile, 0, sizeof(compileFile));
	memset(tempStr, 0, sizeof(tempStr));

	strtok(fname, ".");

	sprintf(makeFile, "%s_Makefile", className[mainPos]);

	if((make_fd = open(makeFile, MODE, 0644)) == 0){
		fprintf(stderr, "open error for %s\n", makeFile);
		exit(1);
	}

	for(int i=0; i<classNameCnt; i++){
		strcat(className[i], ".c");
		strcat(compileFile, className[i]);
		strcat(compileFile, " ");
	}

	strtok(className[mainPos], ".");

	sprintf(tempStr, "%s :\n", className[mainPos]);
	write(make_fd, tempStr, strlen(tempStr));


	memset(tempStr, 0, sizeof(tempStr));
	sprintf(tempStr, "\tgcc -o %s %s\n", className[mainPos], compileFile);
	write(make_fd, tempStr, strlen(tempStr));

	memset(tempStr, 0, sizeof(tempStr));
	sprintf(tempStr, "clean : \n");
	write(make_fd, tempStr, strlen(tempStr));

	memset(tempStr, 0, sizeof(tempStr));
	sprintf(tempStr, "\trm %s\n", className[mainPos]);
	write(make_fd, tempStr, strlen(tempStr));

	close(make_fd);
}

char * makeClose(char * javaCode)
{
	int lineNum;
	int cnt=0;
	int fd;
	int fileSize;
	FILE * closefp;
	char code[BUFSIZE] = {0};

	lineNum = getLineNum(currentFname);

	if((closefp = fopen(currentFname, "r")) == NULL){
		fprintf(stderr, "%s fopen error\n", currentFname);
		exit(1);
	}

	while(!feof(closefp)){
		char strJava[BUFSIZE] = {0};

		if(fread(strJava, 1, 1, closefp) == 0)
			break;
		else
			fseek(closefp, -1, SEEK_CUR);

		strcpy(strJava, fgets(strJava, sizeof(strJava), closefp));

		cnt++;
		
		if(cnt == lineNum)
			break;
		else
			strcat(code, strJava);
	}

	fclose(closefp);

	if((closefp = fopen(currentFname, "w")) == NULL){
		fprintf(stderr, "%s fopen error\n", currentFname);
		exit(1);
	}

	fwrite(code, 1, strlen(code), closefp);

	fclose(closefp);

	return "close(fd)";
}

char * makeWrite(char * javaCode)
{
	char * text;
	char * code;

	code = malloc(sizeof(char) * MAXLEN);
	text = malloc(sizeof(char) * MAXLEN);

	strtok(javaCode, "(");
	text = strtok(NULL, "(");

	removeEnd(text);
	removeEnd(text);
	removeEnd(text);

	findHeader("strlen");

	sprintf(code, "write(fd, %s, strlen(%s));\n", text, text);

	return code;
}

char * makeNewFile(char * javaCode)
{
	int tempPos;

	char * fname;
	char * code;

	code = malloc(sizeof(char) * MAXLEN);

	tempPos = ftell(fp);

	fseek(fp, 0L, SEEK_SET);

	while(!feof(fp)){
		char strJava[BUFSIZE];

		int cnt = 0;

		if(fread(strJava, 1, 1, fp) == 0)
			break;
		else
			fseek(fp, -1, SEEK_CUR);

		strcpy(strJava, fgets(strJava, sizeof(strJava), fp));

		if(strstr(strJava, "File(") != NULL){
			fname = getFilename(strJava);
			fseek(fp, tempPos, SEEK_SET);
			break;
		}
	}

	if(strstr(javaCode, "true") != NULL){
		sprintf(code, "if((fd = open(\"%s\", O_RDWR|O_CREAT|O_APPEND, 0644)) < 0){\n", fname);
	}
	else if(strstr(javaCode, "false") != NULL){
		sprintf(code, "if((fd = open(\"%s\", O_RDWR|O_CREAT|O_TRUNC, 0644)) < 0){\n", fname);
	}

	return code;
}

char * makeDefine(char * javaCode)
{
	char * code[MAXLEN];
	char * temp;

	int cnt = 0;

	temp = malloc(sizeof(char) * MAXLEN);

	code[cnt] = strtok(javaCode, " ");

	while(code[cnt] != NULL){
		cnt++;
		code[cnt] = strtok(NULL, " ");
	}

	removeEnd(code[6]);
	removeEnd(code[6]);

	sprintf(temp, "%s %s", code[4], code[6]);

	return temp;
}

char * makeArray(char * javaCode)
{
	char tempjavaCode[MAXLEN];

	strcpy(tempjavaCode, javaCode);

	strtok(javaCode, " ");

	char * varName = strtok(NULL, " ");
	char * front;

	front = malloc(sizeof(char) * MAXLEN);

	strcpy(front, "new ");

	strcat(front, strtok(tempjavaCode, "["));
	strcat(front, "[");

	int tempPos;

	//Remove '\n' ';'
	removeEnd(varName);
	removeEnd(varName);

	tempPos = ftell(fp);

	fseek(fp, 0L, SEEK_SET);

	while(!feof(fp)){
		char strJava[BUFSIZE];
		char * temp;

		temp = malloc(sizeof(char) * MAXLEN);

		int cnt=0;

		if(fread(strJava, 1, 1, fp) == 0)
			break;
		else
			fseek(fp, -1, SEEK_CUR);

		strcpy(strJava, leftTrim(fgets(strJava, sizeof(strJava), fp)));

		if(strstr(strJava, front) != NULL){

			strtok(strJava, "[");
			strcpy(temp, strtok(NULL, "["));
			strtok(temp, "]");

			fseek(fp, tempPos, SEEK_SET);

			char * code;
			code = malloc(sizeof(char) * MAXLEN);

			sprintf(code, "%s[%s];\n", varName, temp);
			return code;
		}

	}
}

char * makeScanf(char * javaCode)
{
	char * valueName;
	char * cCode;

	cCode = malloc(sizeof(char) * MAXLEN);
	memset(cCode, 0, sizeof(cCode));

	valueName = strtok(javaCode, " ");

	strcat(cCode, "scanf(\"%d\", &");
	strcat(cCode, valueName);
	strcat(cCode, ");\n");

	return cCode;
}

char * makePrintf(char * javaCode)
{
	char *front, *rear;
	char *temp;
	char *cCode;
	
	front = malloc(sizeof(char) * MAXLEN);
	rear = malloc(sizeof(char) * MAXLEN);
	cCode = malloc(sizeof(char) * MAXLEN);
	temp = malloc(sizeof(char) * MAXLEN);

	memset(front, 0, sizeof(front));
	memset(rear, 0, sizeof(rear));
	memset(cCode, 0, sizeof(cCode));
	memset(temp, 0, sizeof(temp));

	strcpy(cCode, "printf(");

	strcpy(front, strstr(javaCode, "("));
	strcpy(rear, strstr(javaCode, ");"));

	strncpy(temp, front+1, strlen(front) - strlen(rear) - 1);

	for(int i=0; i<conListCnt; i++){
		if(strstr(temp, conList[i]) != NULL){
			temp = replaceAll(temp, strcat(conList[i], "."), "");
			removeEnd(conList[i]);
		}
	}

	strcat(cCode, temp);
	strcat(cCode, ");\n");

	return cCode;
}

int getFileSize(char * fname)
{
	int fd;
	int length = 0;

	if((fd = open(fname, O_RDONLY)) == 0){
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}

	length = lseek(fd, 0, SEEK_END);
	close(fd);
	
	return length;
}

int getLineNum(char * fname)
{
	FILE * linefp;
	int cnt = 0;

	if((linefp = fopen(fname, "r")) == NULL){
		fprintf(stderr, "%s fopen error\n", fname);
		exit(1);
	}

	while(!feof(linefp)){
		
		char strJava[BUFSIZE];

		if(fread(strJava, 1, 1, linefp) == 0)
			break;
		else
			fseek(linefp, -1, SEEK_CUR);

		strcpy(strJava, fgets(strJava, sizeof(strJava), linefp));

		cnt++;
	}

	fclose(linefp);

	return cnt;
}

char * getFilename(char * javaCode)
{
	char * fname;

	strtok(javaCode, "\"");

	fname = strtok(NULL, "\"");

	return fname;

}

void adjustMain(int mainPos)
{
	for(int i=0; i<classNameCnt; i++){
		if(i != mainPos){

			FILE * header_fp;
			FILE * java_fp;

			int fd;

			char * header;
			char * cCode;
			char * javaCode;
			char * javaClass;

			header = malloc(sizeof(char) * MAXLEN);
			cCode = malloc(sizeof(char) * MAXLEN);
			javaCode = malloc(sizeof(char) * MAXLEN);
			javaClass = malloc(sizeof(char) * MAXLEN);

			sprintf(header, "%s.h", className[i]);
			sprintf(cCode, "%s.c", className[mainPos]);
			sprintf(javaCode , "%s", javaFname);
			sprintf(javaClass, "class %s", className[mainPos]);

			if((header_fp = fopen(header, "w+")) == NULL){
				fprintf(stderr, "fopen error for %s\n", header);
			}

			if((java_fp = fopen(javaCode, "r")) == NULL){
				fprintf(stderr, "fopen error for %s\n", javaCode);
			}

			if((fd = open(cCode, O_RDWR)) == 0){
				fprintf(stderr, "open error for %s\n", cCode);
			}

			while(!feof(java_fp)){

				char strJava[BUFSIZE] = {0};

				if(fread(strJava, 1, 1, java_fp) == 0)
					break;
				else
					fseek(java_fp, -1, SEEK_CUR);

				strcpy(strJava, leftTrim(fgets(strJava, sizeof(strJava), java_fp)));

				if(strstr(strJava, javaClass) != NULL){
					while(1){
						if(fread(strJava, 1, 1, java_fp) == 0)
							break;
						else
							fseek(java_fp, -1, SEEK_CUR);

						strcpy(strJava, fgets(strJava, sizeof(strJava), java_fp));

						if(strcmp(strJava, "}\n") == 0){
							break;
						}
					}
				}
				else if(strstr(strJava, "public") != NULL && strstr(strJava, "static final") == NULL){

					char * funcFormat;

					funcFormat = malloc(sizeof(char) * MAXLEN);

					funcFormat = chkFuncformat(strJava);

					removeEnd(funcFormat);

					if(strchr(funcFormat, '{') != NULL)
						removeEnd(funcFormat);

					strcat(funcFormat, ";\n");

					fwrite(funcFormat, strlen(funcFormat), 1, header_fp);
				}
			}

			char * insertCode;

			insertCode = malloc(sizeof(char) * MAXLEN);

			sprintf(insertCode, "#include \"%s\"\n", header);

			insertToC(fd, insertCode, HEADER);

			if(optrFlag == TRUE){
				isOptrFlag('1');
			}
		}
	}
}

int checkExistMain(char * fname)
{
	FILE *fp;
	char *buf;
	int size, count;

	char * temp;

	temp = malloc(sizeof(char) * MAXLEN);

	sprintf(temp, "%s.c", fname);

	if((fp = fopen(temp, "r")) == NULL){
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}

	while(!feof(fp)){
		
		char strJava[BUFSIZE];

		int cnt = 0;

		if(fread(strJava, 1, 1, fp) == 0)
			break;
		else
			fseek(fp, -1, SEEK_CUR);

		strcpy(strJava, leftTrim(fgets(strJava, sizeof(strJava), fp)));

		if(strstr(strJava, "int main(") != NULL){

			fclose(fp);

			return 1;
		}
	}

	fclose(fp);

	return 0;
}

void closeFile(void)
{
	for(int i=classNameCnt; i>0; i--){
		close(c_fd - i + 1);
	}
}

char* eliminateBrace(char *str, char ch)
{
	char * code;

	code = malloc(sizeof(char) * BUFSIZE);

	for(; *str != '\0'; str++){
		if(*str == ch){
			str++;
		}
		append(code, *str);
	}

	return code;
}

void append(char * dst, char c)
{
	char *p = dst;
	while(*p != '\0')
		p++;
	*p = c;
	*(p+1) = '\0';
}

char* leftTrim(char *str)
{
	char *temp;
	char cmp[] = {'\t', '\0', '\r', '\n'};
	int cnt = 0;

	temp = str;

	while(*temp != '\0'){
		if(*temp == cmp[0] || *temp == cmp[1] || *temp == cmp[2] || *temp == cmp[3]){
			++temp;
		}
		else{
			str = temp;
			break;
		}
	}

	return str;
}

char* toString(const char* src)
{
	char * ptr = NULL;

	ptr = (char*)malloc(sizeof(char) * strlen(src) + 1);

	strcpy(ptr, src);

	return ptr;
}

void removeEnd(char * buf)
{
	int i=0;

	while(buf[i])
		i++;

	buf[i-1] = '\0';
}

char * replaceAll(char *s, const char *olds, const char *news)
{
	char * result, *sr;
	size_t i, count = 0;
	size_t oldlen = strlen(olds);
	
	if(oldlen < 1)
		return s;
	size_t newlen = strlen(news);

	if(newlen != oldlen){
		for(i=0; s[i] != '\0';){
			if(memcmp(&s[i], olds, oldlen) == 0)
				count++, i+=oldlen;
			else
				i++;
		}
	}
	else
		i = strlen(s);

	result = (char *)malloc(i + 1 + count * (newlen - oldlen));

	if(result == NULL)
		return NULL;

	sr = result;
	while(*s){
		if(memcmp(s, olds, oldlen) == 0){
			memcpy(sr, news, newlen);
			sr += newlen;
			s += oldlen;
		}
		else
			*sr++ = *s++;
	}

	*sr = '\0';

	return result;
}

void javaCmp(char * javaCode)
{

	//Ignoring OK
	char * ignore[] = {"import", "Scanner", "nextLine", "flush()"};
	char * fname;
	char * insertCode;


	for(int i=0; i < sizeof(ignore) / sizeof(ignore[0]); i++){
		if(strstr(javaCode, ignore[i]) != NULL){
			return ;
		}
	}

	if(conFlag == TRUE){
		if(strcmp(javaCode, "}\n") == 0)
			conFlag = FALSE;
		return ;
	}

	if(strstr(javaCode, "class") != NULL){

		if(strstr(javaCode, "{") == NULL)
			publicFlag = TRUE;
		if(strstr(javaCode, "public") == NULL)
			mainFlag = TRUE;

		if((fname = createCFile(javaCode)) != NULL){

			c_fd = openCFile(fname);

			if(pid != getpid() && pid != 0){

				close(forkfd);

				if((forkfd = open(FORKFLAG, MODE, 0644)) == 0){
					fprintf(stderr, "open error for %s\n", FORKFLAG);
					exit(1);
				}
			}

			strcpy(currentFname, fname);

			className[classNameCnt] = toString(strtok(fname, "."));
			classNameCnt++;
			isOptrFlag('1');
			return ;
		}
	}

	insertCode = malloc(sizeof(char) * BUFSIZE);
	memset(insertCode, 0, sizeof(insertCode));

	if(strchr(javaCode, '}') != NULL){
		if(--braceNum < 0){
			braceNum = 0;
			isOptrFlag('1');
			return ;
		}
	}

	for(int i=0; i<braceNum; i++){
		if(i == 0)
			strcpy(insertCode, "\t");
		else
			sprintf(insertCode, "\t%s", insertCode);
	}

	if(javaCode[0] == '{'){
		if(tabFlag == TRUE)
			tabFlag = FALSE;
		if(publicFlag == TRUE){
			publicFlag = FALSE;
			return ;
		}
		braceNum++;
	}
	else if(strchr(javaCode, '{') != NULL){
		braceNum++;
	}

	if((strstr(javaCode, "= new") != NULL || strstr(javaCode, "=new") != NULL)){

		if(strstr(javaCode, "File(") != NULL)
			return ;
		else if(strstr(javaCode, "FileWriter(") != NULL){
			char temp[MAXLEN] = {0};

			findHeader("open");

			if(openFlag == FALSE){
				forPList[forPListCnt] = "FileWriter() -> open()";
				forPListCnt++;
				openFlag = TRUE;
			}

			strcpy(temp, insertCode);
			strcat(insertCode, "int fd;\n");
			insertToC(c_fd, insertCode, CODE);

			strcpy(insertCode, temp);
			strcat(insertCode, makeNewFile(javaCode));
			insertToC(c_fd, insertCode, CODE);

			strcpy(insertCode, temp);
			strcat(insertCode, "\tfprintf(stderr, \"open error\\n\");\n");
			insertToC(c_fd, insertCode, CODE);

			strcpy(insertCode, temp);

			findHeader("exit");
			strcat(insertCode, "\texit(1);\n");
			insertToC(c_fd, insertCode, CODE);

			strcpy(insertCode, temp);
			strcat(insertCode, "};\n");
			insertToC(c_fd, insertCode, CODE);

			if(optrFlag == TRUE)
				isOptrFlag('1');
			
			return ;
		}
		else {

			char *ptr = strtok(javaCode, " ");

			for(int i=0; i < classNameCnt; i++){
				if(strcmp(ptr, className[i]) == 0)
					conList[conListCnt++] = toString(strtok(NULL, " "));
			}

			return ;
		}
	}
	else if(strstr(javaCode, "void main") != NULL){
		mainFlag = TRUE;
		if(strchr(javaCode, '{') != NULL)
			sprintf(insertCode, "%s%s", insertCode, "int main(void){\n");
		else
			sprintf(insertCode, "%s%s", insertCode, "int main(void)\n");
			
	}
	else if(strstr(javaCode, "printf") != NULL){

		if(printfFlag == FALSE){
			forPList[forPListCnt] = "System.out.printf() -> printf()";
			forPListCnt++;
			printfFlag = TRUE;
		}

		findHeader("printf");
		sprintf(insertCode, "%s%s", insertCode, makePrintf(javaCode));

	}
	else if(strstr(javaCode, "nextInt") != NULL){

		if(scanfFlag == FALSE){
			forPList[forPListCnt] = "nextInt() -> scanf()";
			forPListCnt++;
			scanfFlag = TRUE;
		}

		findHeader("scanf");
		sprintf(insertCode, "%s%s", insertCode, makeScanf(javaCode));

	}
	else if(strstr(javaCode, "return ;") != NULL){

		findHeader("exit");
		sprintf(insertCode, "%s%s", insertCode, "exit(0);\n");

	}
	else if(strstr(javaCode, "public static final") != NULL){
		sprintf(insertCode, "#define %s\n", makeDefine(javaCode));
		insertToC(c_fd, insertCode, HEADER);

		if(optrFlag == TRUE)
			isOptrFlag('1');
		
		return ;
	}
	else if(strstr(javaCode, "public") != NULL){
		sprintf(insertCode, "%s%s", insertCode, chkFuncformat(javaCode));

		char temp[MAXLEN] = {0};
		sprintf(temp, " %s()", className[classNameCnt-1]);

		if(strstr(insertCode, temp) != NULL){
			conFlag = TRUE;
			if(strchr(insertCode, '{') != NULL)
				braceNum--;
			return;
		}
	}
	else if(strstr(javaCode, "[]") != NULL)
		sprintf(insertCode, "%s %s", strtok(javaCode, "["), makeArray(javaCode));
	else if(strstr(javaCode, "write(") != NULL){

		if(writeFlag == FALSE){
			forPList[forPListCnt] = "write() -> write()";
			forPListCnt++;
			writeFlag = TRUE;
		}

		findHeader("write");
		sprintf(insertCode, "%s%s\n", insertCode, makeWrite(javaCode));
	}
	else if(strstr(javaCode, "close()") != NULL){
		if(closeFlag == FALSE){
			forPList[forPListCnt] = "close() -> close()";
			forPListCnt++;
			closeFlag = TRUE;
		}
		sprintf(insertCode, "%s;\n", makeClose(javaCode));
	}
	else if(cmpConList(javaCode)){
		sprintf(insertCode, "%s%s", insertCode, getConfunc(javaCode));
	}
	else{
		sprintf(insertCode, "%s%s", insertCode, javaCode);

		if((strstr(javaCode, "if") != NULL || strstr(javaCode, "for") != NULL 
					|| strstr(javaCode, "else")) && strchr(javaCode, '{') == NULL){
			tabFlag = TRUE;
			insertToC(c_fd, insertCode, CODE);

			if(optrFlag == TRUE)
				isOptrFlag('1');

			return ;
		}
		else if(strstr(javaCode, "int") != NULL && strstr(javaCode, "=") == NULL){
			char temp[MAXLEN];
			char * result;

			strcpy(temp, javaCode);

			result = malloc(sizeof(char) * MAXLEN);
			memset(result, 0, sizeof(result));

			result = checkValue(javaCode);

			if(result != NULL){
				removeEnd(insertCode);
				removeEnd(insertCode);
				sprintf(insertCode, "%s=%s;\n", insertCode, result);
			}
		}
	}

	if(tabFlag == TRUE){

		char * temp;

		temp = malloc(sizeof(char) * MAXLEN);

		sprintf(temp, "\t%s", insertCode);

		tabFlag = FALSE;

		strcpy(insertCode, temp);

	}

	if(c_fd != 0)
		insertToC(c_fd, insertCode, CODE);

	if(forkfd != 0)
		isOptrFlag('1');

	return ;
}

void insertToC(int fd, char * code, int position)
{
	char * tmp = NULL;
	char buf[BUFSIZE];

	memset(buf, 0, sizeof(buf));

	lseek(fd, 0, SEEK_SET);

	// Go to beginning to insert a header
	if(position == HEADER){

		read(fd, buf, sizeof(buf));

		if(checkHeader(buf, code))
			lseek(fd, 0, SEEK_SET);
		else if(strstr(code, ".h\"") != NULL)
			lseek(fd, 0, SEEK_SET);
		else
			return ;
	}
	else if(position == CODE)
		lseek(fd, 0, SEEK_END);
	
	// Insert existing code after inserting new code
	write(fd, code, strlen(code));
	write(fd, buf, strlen(buf));

}

char * chkFuncformat(char * javaCode)
{
	char temp[BUFSIZE];
	char * result[BUFSIZE];
	char * factor[BUFSIZE];

	char * code;

	int cnt = -1;

	code = (char *)malloc(sizeof(char) * BUFSIZE);
	memset(code, 0, BUFSIZE);

	strcpy(temp, javaCode);

	factor[cnt+1] = strtok(temp, "(");
	cnt++;

	while(factor[cnt] != NULL){
		factor[cnt+1] = strtok(NULL, "(");
		cnt++;
	}

	strcpy(temp, javaCode);
	cnt = -1;
	strtok(temp, "(");

	strcpy(temp, strtok(temp, "("));

	result[cnt+1] = strtok(temp, " ");
	cnt++;


	while(result[cnt] != NULL){
		result[cnt+1] = strtok(NULL, " ");
		cnt++;
	}

	if(cnt == 2){

		strcat(code , "void ");
		strcat(code , result[1]);

		strcat(code, "(");
		strcat(code, factor[1]);
	}
	else if(cnt == 3){

		strcat(code, result[1]);
		strcat(code, " ");

		strcat(code, result[2]);
		strcat(code, "(");

		strcat(code , factor[1]);

	}

	return code;
}

char * getConfunc(char * javaCode)
{
	char * result;
	result = strtok(javaCode, ".");

	while(result != NULL){
		result = strtok(NULL, ".");
		return result;
	}
}

int cmpConList(char * javaCode)
{
	char temp[BUFSIZE];
	strcpy(temp, javaCode);

	if(strchr(temp, '.') == NULL){
		return 0;
	}

	for(int i=0; i < conListCnt; i++){
		if(strcmp(conList[i], strtok(temp, ".")) == 0)
			return 1;
	}

	return 0;
}

void findHeader(char * func)
{
	char *fname = "header";
	FILE *fp;
	int cnt = 0;

	if((fp = fopen(fname, "r")) == NULL){
		fprintf(stderr, "%s fopen error\n", fname);
		exit(1);
	}

	while(!feof(fp)){

		char strJava[BUFSIZE];

		char * tmp;

		// Prevent duplicate ouput of last line
		if(fread(strJava, 1, 1, fp) == 0)
			break;
		else
			fseek(fp, -1, SEEK_CUR);

		// Read one line
		fgets(strJava, sizeof(strJava), fp);
		
		// Function name separation
		tmp = strtok(strJava, "#");

		// Insert header if function is appropriate
		if(strstr(tmp, func) != NULL){
			while(1){
				char str[BUFSIZE];

				if((tmp = strtok(NULL, "#")) == NULL)
					break;
				
				strcpy(str, tmp);

				for(int i=strlen(str)-1; i>0; i--){
					str[i] = str[i-1];
				}

				str[0] = '#';
				insertToC(c_fd, strcat(str, "\n"), HEADER);
			}
		}

	}
	
	fclose(fp);
}

int checkHeader(char * code, char * header)
{
	if(strstr(code, header) == NULL){
		return 1;
	}

	return 0;
}

int openCFile(char * fname)
{
	int fd;

	if((fd = open(fname, O_RDWR)) < 0){
		fprintf(stderr, "%s open error\n", fname);
		exit(1);
	}

	return fd;
}

char * createCFile(char * javaCode)
{
	char *ptr = strtok(javaCode, " ");
	char *fname;
	int fd;

	while(ptr != NULL){
		fname = ptr;
		ptr = strtok(NULL, " ");
	}

	// For removing '{'
	if(fname[strlen(fname) -2] == '{')
		fname[strlen(fname)-2] = '\0';
	else
		fname[strlen(fname)-1] = '\0';
	strcat(fname, ".c");

	if((fd = creat(fname, 0644)) < 0){
		fprintf(stderr, "%s creat error\n", fname);
		exit(1);
	}

	close(fd);

	return fname;

}

char * checkValue(char * javaCode)
{
	FILE * temp_fp;
	char strJava[BUFSIZE] = {0};
	char * temp;

	temp = malloc(sizeof(char) * MAXLEN);
	memset(temp, 0, sizeof(temp));

	javaCode = strtok(javaCode, " ");
	javaCode = strtok(NULL, " ");

	javaCode[strlen(javaCode) -2] = '\0';

	if((temp_fp = fopen(javaFname, "r")) == NULL){
		fprintf(stderr, "fopen error for %s\n", javaFname);
		exit(1);
	}

	while(!feof(temp_fp)){

		if(fread(strJava, 1, 1, temp_fp) == 0)
			break;
		else
			fseek(temp_fp, -1, SEEK_CUR);

		fgets(strJava, sizeof(strJava), temp_fp);

		if(strstr(strJava, "=") != NULL && strstr(strJava, javaCode) != NULL){
			fclose(temp_fp);
			break;
		}
	}

	temp = strtok(strJava, "=");
	temp = strtok(NULL, "=");
	removeEnd(temp);
	removeEnd(temp);

	for(int i=0; i<strlen(temp); i++){
		if((temp[i] >= '0' && temp[i] <= '9') ||  temp[i] == '-' || temp[i] == ' '){
		}
		else
			return NULL;
	}

	return temp;
}
