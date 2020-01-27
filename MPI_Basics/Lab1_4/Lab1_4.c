#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
	int rank, size;

	FILE * fid;
	fid = fopen("textfile.txt","r");

	char buf[256];
	const char* otag = "<TEXT>";
	const char* etag = "</TEXT>";
	int s_start, s_end, in_tag=0;

	while(fgets(buf,256,fid)){
		fprintf(stdout,"%s\n",buf);
		s_start = 0;

		for(int i=0; i<strlen(buf); i++){ //splicer
			if(buf[i] == ' '){

					const char * word = substr(buf,s_start,i);

					if(!in_tag && strcmp(word,otag)==0){ //start tag
						in_tag=1;
					}
					else if(!in_tag){ //not in tag
						s_start=i+1;
					}
					else if(in_tag && strcmp(word,etag)==0){ //end tag
						in_tag=0;
					}
					else{ //in tag
						fprintf(stdout,"%s\n",substr(buf,s_start,i));
						s_start=i+1;
					}
			}
		}

	}

	fclose(fid);

	return 0;
}
