/*************************************
 * HW1_Yasin_Tulumen_121044020
 *
 * Bulundugu dizindeki arguman olarak verilen
 * veri yolunun icindeki dosyalarda
 * yine arguman olarak verilen stringin
 * kac defa bulundugunu yazdiran, bulundugu konumlari
 * grepFromDir.log dosyasina kaydeden program
 *
*************************************/ 
#include <dirent.h>
#include <stdio.h>
#include <string.h> /* String fonksiyonlari kullanmak icin */
#include <stdlib.h> /* Malloc kullanabilmek icin */
#include <errno.h> /* Hata bastirmak icin */
#include <time.h>
#include <sys/stat.h>
#include <string.h>

/******************FUNCTION DEFINITIONS*****************/
/*parametre olarak aldigi directory icinde searchWord'u arayip
 *bulan ve adedini return eden fonksiyon */
int readDirectory(char *path, char *searchWord);
/*aldigi path'in directory veya file olup olmadigini return eden fonksiyon*/
int isdirectory(char *path);
/*butun dosyadaki kelime sayisini bulan ve return eden fonksiyon*/
int numberofWord(const char *fileName, const char *searchWord, const int maxChar);
/*source stringinin icinde dest stringini bulan ve kac defa
 *bulundugunu return eden fonksiyon*/
int match(const char *source, const char *dest, int lineCounter);
/*dosyadaki en uzun karakter sayisini bulan fonksiyon */
int findMaxCharofLines(char *fileName);

/****************FUNCTION IMPLEMENTATIONS*************/
int main(int argc, char *argv[]){
	FILE *fileP, *logFile;

	/*eger dosya acilamazsa hata verilecek ve cikilacak*/
	if(argc!=3){
		fprintf(stderr, "Usage: %s directory \n", argv[0]);
		return -1;
	}
	/*eger dosya acilamazsa hata verilecek ve cikilacak*/
	if((logFile = fopen("grepFromDir.log", "w")) == NULL){
		/*eger dosya acilamazsa hata verilecek ve cikilacak*/
		fprintf(stderr, "FILE WERE NOT OPEN grepFromDir>%s\n", strerror(errno));
		return -1;	
	}

	printf("\n\n\n-----------------------\n");
	fprintf(logFile,"#############################\n");
	fprintf(logFile,"In %s file '%s' found:\n", argv[1], argv[2]);
	fclose(logFile);
	printf("Total number of '%s' in %s : %d \n",
			argv[2], argv[1], readDirectory(argv[1], argv[2]));

	return 0;
}
int readDirectory(char *path, char *searchWord)
{
	FILE *logFile;
	struct dirent *direntp,*direntp2;
	DIR *dirp,*dirp2;
	char dirName[1000], fileName[256];
	int i=0, pathC=0, now=0, childpid;
	/* eger path file ise icindeki kelime sayisini bulmak icin
	 * numberofWord fonksiyonu cagirilacak*/
	if ((childpid = fork()) == -1) {
		perror("Failed to create child process");
		return 1;
	}
	wait(&now);
	printf("$$$$$$$$$$$$$$$$$$$$$$$4\n");
	if(isdirectory(path)>0 && childpid>0){
		printf("parent !: %ld %s\n",(long)getpid(), path );
		if ((dirp = opendir(path)) == NULL || path[0]=='.') 
			return now;

		while ((direntp = readdir(dirp)) != NULL){
			if(direntp->d_name[0]=='.')
				continue;
			strcpy(dirName,path);
			strcat(dirName,"/");
			strcat(dirName,direntp->d_name);

			now += readDirectory(dirName,searchWord);
		}
		while ((closedir(dirp) == -1) && (errno == EINTR));
		exit(EXIT_SUCCESS);
	}
	else if(isdirectory(path)==0 && childpid==0){
		printf("child !: %ld %s\n",(long)getpid(), path );
		now = numberofWord(path, searchWord, findMaxCharofLines(path));
		exit(EXIT_SUCCESS);
	}
	else if(isdirectory(path)<0){
		printf("Error! %s is not a directory or a file!\n",path );
		return -1;
	}
	/*directory */

	return now;
}
int isdirectory(char *path)
{
	struct stat statbuf;

	/*verilen dosya yolu file veya directory degil ise*/
	/*-1 return edilecek*/
	/*aksi halde hangisi oldugu return edilecek*/
	if (stat(path, &statbuf) == -1)
		return -1;
	else
		return S_ISDIR(statbuf.st_mode);
}
int numberofWord(const char *fileName, const char *searchWord, const int maxChar){
	int totalCount=0, i=1, any=0;
	char *read;
	FILE* fileP, *logFile;
	read = malloc(maxChar* sizeof(char));
	if((logFile = fopen("grepFromDir.log", "a")) == NULL){
		/*eger dosya acilamazsa hata verilecek ve cikilacak*/
		fprintf(stderr, "FILE WERE NOT OPEN grepFromDir>%s\n", strerror(errno));
		return -1;	
	}
	if((fileP = fopen(fileName, "r")) == NULL){
		/*eger dosya acilamazsa hata verilecek ve cikilacak*/
		fprintf(stderr, "FILE WERE NOT OPEN %s>%s\n", fileName, strerror(errno));
		return -1;	
	}
	fprintf(logFile,"-------------------------------\n");
	fprintf(logFile,"In %s file %s found:\n", fileName, searchWord);
	fclose(logFile);
	/*satir satir okuma yapilacak
	 *okunan satir match fonksiyonuna gonderilerek
	 *o satirdaki aranan kelimeyi bulup sayisini return edecek */
	while(fgets(read, maxChar, fileP) != NULL){

		totalCount += match(read, searchWord, i);
		++i;
	}
	free(read);
	fclose(fileP);
	return totalCount;
}
int match(const char *source, const char *dest, int lineCounter)
{
	FILE* logFile;
    int counter = 0, columnCounter=0, lengthofDest = strlen(dest);
    
	/*eger dosya acilamazsa hata verilecek ve cikilacak*/
	if((logFile = fopen("grepFromDir.log", "a")) == NULL){
		fprintf(stderr, "FILE WERE NOT OPEN grepFromDir>%s\n", strerror(errno));
		return -1;	
	}

	/*sourceun sonuna gelene kadar okuma yapacak
	 *aldigi stringin sonuna kadar giderek dest
	 *stringini arayacak*/
    while (*source != '\0') {
    	++columnCounter;
        if (!strncmp(source++, dest, lengthofDest)){ 
	        fprintf(logFile, "# Line: %d Column: %d\n", lineCounter, columnCounter );
	        counter++;
    	}
    }
    fclose(logFile);
    return counter;
}
int findMaxCharofLines(char *fileName){
	FILE* fileP;
	char ch;
	int numOfChar=0, temp=0;
	if((fileP = fopen(fileName, "r")) == NULL){
		fprintf(stderr, "READING ERROR: %s>%s\n", fileName, strerror(errno));
		return -1;	
	}
    /*dosyanin sonuna kadar okuma yapilacak
     *numOfChar degiskeni en fazla karakteri iceren satiri bulacak*/
	while(!feof(fileP)){
    	fscanf(fileP, "%c",&ch);
    	if(ch != '\n'){	
      		++temp;
			if(numOfChar<temp){
				numOfChar = temp;
      		}
      	}
      	else
			temp=0;     
  	}
  	fclose(fileP);
  	return numOfChar+2;
}


