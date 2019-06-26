#include<stdio.h>
#include<stdlib.h>
#define MEM_LEN 3000

void except(FILE* file,char* source,int e){
  free(source);
  fclose(file);
  if(e>=0) exit(e);
}
int interpret(FILE* file){
  char ch;
  int mem=0,code=0;
  char memory[MEM_LEN+1];
  fseek(file,0,SEEK_END);
  int source_len=ftell(file);
  char* source=(char*)malloc(source_len);
  rewind(file);
  for(int a=0;a<source_len;a++) source[a]=fgetc(file);
  while(code<source_len){
    char c=source[code];
    if(c=='>' && (++mem)==MEM_LEN) mem=0;
    else if(c=='<' && !(--mem)) mem=MEM_LEN-1;
    else if(c=='.') printf("%c",memory[mem]);
    else if(c==',') memory[mem]=fgetc(stdin);
    else if(c=='+') memory[mem]++;
    else if(c=='-') memory[mem]--;
    else if(c=='[' && !memory[mem]){
      int open=1;
      while(open){
        if(++code==source_len) except(file,source,0);
        if(source[code]=='[') open++;
        if(source[code]==']') open--;
      }
    }else if(c==']' && memory[mem]){
      int close=1;
      while(close){
        if(--code==-1) except(file,source,0);
        if(source[code]==']') close++;
        if(source[code]=='[') close--;
      }
    }else if(!(c=='\t' || c=='\n' || c==' ' || c=='<' || c=='>' || c=='[' || c==']')){
      fprintf(stderr,"wtf is this? (%i)\n",c);
      except(file,source,1);
    }
    code++;
  }
  except(file,source,-1);
  return 0;
}

int main(int argc,char** argv){
  if(argc!=2){
    fprintf(stderr,"Usage: brainfuck file\n");
    exit(1);
  }
  FILE* source=fopen(argv[1],"r");
  if(!source){
    fprintf(stderr,"Could not open source file %s\n",argv[1]);
    exit(1);
  }
  return interpret(source);
}
