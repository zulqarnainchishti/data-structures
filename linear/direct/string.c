#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void swap(char *a, char *b){
    char temp = *a;
    *a = *b;
    *b = temp;
}

typedef struct{
    char *word;
}String;

String init(char *string){
    int len=0;
    while (string[len]) len++;
    String new;
    new.word = (char *)malloc((len+1)*sizeof(char));
    char *temp=new.word;
    while (*string) *temp++=*string++;
    *temp='\0';
    return new;
}

String copy(String string){
    int len=0;
    while (string.word[len]) len++;
    String new;
    new.word = (char *)malloc((len+1)*sizeof(char));
    char *temp=new.word;
    while (*string.word) *temp++=*string.word++;
    *temp='\0';
    return new;
}

void delete(String *string){
    free(string->word);
    string->word=NULL;
}

void clear(String *string){
    free(string->word);
    string->word=(char *)malloc(sizeof(char));
    *string->word='\0';
}

int length(String string){
    int len=0;
    while (string.word[len]) len++;
    return len;
}

bool isEmpty(String string){
    return *string.word=='\0';
}

bool isIn(String string, char letter){
    while (*string.word){
        if(*string.word==letter) return true;
        string.word++;
    }
    return false;
}

bool isEqual(String string1, String string2){
    int len1=0,len2=0;
    while (string1.word[len1]) len1++;
    while (string2.word[len2]) len2++;
    if(len1!=len2) return false;
    while (*string1.word){
        if(*string1.word!=*string2.word) return false;
        string1.word++;
        string2.word++;
    }
    return true;
}

bool isSimilar(String string1, String string2){
    int len1=0,len2=0;
    while (string1.word[len1]) len1++;
    while (string2.word[len2]) len2++;
    if(len1!=len2) return false;
    while (*string1.word){
        if(*string1.word!=*string2.word && abs(*string1.word-*string2.word)!=32){
            return false;
        }
        string1.word++;
        string2.word++;
    }
    return true;
}

int find(String string, char letter){
    int index=0;
    while(string.word[index])
    {
        if(string.word[index]==letter) return index;
        index++;
    }
    return -1;
}

int count(String string, char letter){
    int freq=0;
    int index=0;
    while(string.word[index])
    {
        if(string.word[index]==letter) freq++;
        index++;
    }
    return freq;
}

char get(String string, int index){
    return string.word[index];
}

void set(String *string, char letter, int index){
    string->word[index]=letter;
}

void replace(String *string, char old, char new){
    char *temp=string->word;
    while (*temp){
        if(*temp==old){
            *temp=new;
            return;
        }
        temp++;        
    }
}

String append(String string1, char *string2){
    int len1=0,len2=0;
    while(string1.word[len1]) len1++;
    while(string2[len2]) len2++;
    String new;
    new.word=(char *)malloc(sizeof(char)*(len1+len2+1));
    char *temp=new.word;
    while (*string1.word) *temp++=*string1.word++;
    while (*string2) *temp++=*string2++;
    *temp='\0';
    return new;
}

String insert(String string1, char *string2, int index){
    int len1=0,len2=0;
    while (string1.word[len1]) len1++;
    while (string2[len2]) len2++;
    String new;
    new.word=(char *)malloc(sizeof(char)*(len1+len2+1));
    char *temp=new.word;
    while (*string1.word && index-->0) *temp++=*string1.word++;
    while (*string2) *temp++=*string2++;
    while (*string1.word) *temp++=*string1.word++;
    *temp='\0';
    return new;
}

String concat(String string1, String string2) {
    int len1=0,len2=0;
    while(string1.word[len1]) len1++;
    while(string2.word[len2]) len2++;
    String new;
    new.word=(char *)malloc(sizeof(char)*(len1+len2+1));
    char *temp=new.word;
    while (*string1.word) *temp++=*string1.word++;
    while (*string2.word) *temp++=*string2.word++;
    *temp='\0';
    return new;
}

String pop(String string, int quantity){
    int len=0;
    while (string.word[len]) len++;
    len-=quantity;
    String new;
    new.word=(char *)malloc(sizeof(char)*(len+1));
    char *temp=new.word;
    while (*string.word && len-->0) *temp++=*string.word++;
    *temp='\0';
    return new;
}

String discard(String string, int index, int quantity){
    int len=0;
    while (string.word[len]) len++;
    len-=quantity;
    String new;
    new.word=(char *)malloc(sizeof(char)*(len+1));
    char *temp=new.word;
    while (*string.word && index-->0) *temp++=*string.word++;
    while (quantity-->0) string.word++;
    while (*string.word) *temp++=*string.word++;
    *temp='\0';
    return new;
}

String slice(String string, int start, int end){
    int len=end-start;
    String new;
    new.word = (char *)malloc((len+1)*sizeof(char));
    char *temp=new.word;
    string.word+=start;
    while (*string.word && len-->0) *temp++=*string.word++;
    *temp='\0';
    return new;
}

bool startsWith(String string1, char *string2){
    while (*string2)
        if(*string1.word++!=*string2++) return false;
    return true;
}

bool endsWith(String string1, char *string2){
    int len1=0,len2=0;
    while (string1.word[len1]) len1++;
    while (string2[len2]) len2++;
    string1.word+=len1-len2;
    while (*string2)
        if(*string1.word++!=*string2++) return false;
    return true;
}

String reverse(String string) {
    int len=0;
    while (string.word[len]) len++;
    String new;
    new.word = (char *)malloc((len+1)*sizeof(char));
    for (int i = 0; i < len; i++)
    {
        new.word[i]=string.word[len-1-i];
    }
    new.word[len]='\0';
    return new;
}

String repeat(String string, int times) {
    int len=0;
    while (string.word[len]) len++;
    String new;
    new.word = (char *)malloc((len*times+1)*sizeof(char));
    char *newTemp=new.word;    
    for (int i = 0; i < times; i++)
    {
        char *oldTemp=string.word;
        while (*oldTemp) *newTemp++=*oldTemp++;
    }
    *newTemp='\0';
    return new;
}

bool isUpper(String string){    
    while (*string.word){
        if(*string.word>='a' && *string.word<='z') return false;
        string.word++;
    }
    return true;
}

String toUpper(String string){
    int len=0;
    while (string.word[len]) len++;
    String new;
    new.word = (char *)malloc((len+1)*sizeof(char));
    char *temp=new.word;
    while (*string.word){
        if(*string.word>='a' && *string.word<='z') *temp=*string.word-32;
        else *temp=*string.word;
        temp++;
        string.word++;
    }
    *temp='\0';
    return new;
}

bool isLower(String string){
    while (*string.word){
        if(*string.word>='A' && *string.word<='Z') return false;
        string.word++;
    }
    return true;
}

String toLower(String string){
    int len=0;
    while (string.word[len]) len++;
    String new;
    new.word = (char *)malloc((len+1)*sizeof(char));
    char *temp=new.word;
    while (*string.word){
        if(*string.word>='A' && *string.word<='Z') *temp=*string.word+32;
        else *temp=*string.word;
        temp++;
        string.word++;
    }
    *temp='\0';
    return new;
}

bool isAlphabet(String string){
    while (*string.word){
        if(!(*string.word>='A' && *string.word<='Z' || *string.word>='a' && *string.word<='z')) return false;
        string.word++;
    }
    return true;
}

bool isNumber(String string){
    while (*string.word){
        if(!(*string.word>='0' && *string.word<='9')) return false;
        string.word++;
    }
    return true;
}

bool isAlphanum(String string){
    while (*string.word){
        if(!(*string.word>='A' && *string.word<='Z' || *string.word>='a' && *string.word<='z' || *string.word>='0' && *string.word<='9')) return false;
        string.word++;
    }
    return true;
}

bool isSpace(String string){
    while (*string.word)
        if(*string.word++!=' ') return false;
    return true;
}

String leftAlign(String string, char letter, int length){
    String new;
    new.word = (char *)malloc(length*sizeof(char));
    char *temp=new.word;    
    while (*string.word && length-->0) *temp++=*string.word++;
    while (length-->0) *temp++=letter;
    *temp='\0';
    return new;
}

String centerAlign(String string, char letter, int length){
    int len2=0;
    while (string.word[len2]) len2++;
    String new;
    new.word = (char *)malloc(length*sizeof(char));
    char *temp=new.word;
    int len1=(length-len2)/2;
    int len3=length-len1-len2;
    while (len1-->0) *temp++=letter;
    while (len2-->0) *temp++=*string.word++;
    while (len3-->0) *temp++=letter;
    *temp='\0';
    return new;
}

String rightAlign(String string, char letter, int length){
    int len2=0;
    while (string.word[len2]) len2++;
    String new;
    new.word = (char *)malloc(length*sizeof(char));
    char *temp=new.word;
    int len1=length-len2;
    while (len1-->0) *temp++=letter;
    while (len2-->0) *temp++=*string.word++;
    *temp='\0';
    return new;
}

String trim(String string, char letter){
    int start=0;
    int end=0;
    while (string.word[end]) end++;
    end--;
    while (start<=end && string.word[start]==letter) start++;
    while (start<=end && string.word[end]==letter) end--;
    String new;
    new.word = (char *)malloc((end-start+2)*sizeof(char));
    char *temp=new.word;
    while (start<=end){
        *temp++=*(string.word+start);
        start++;
    }
    *temp='\0';
    return new;
}

String *split(String string, char delimiter, int *size){
    *size=0;
    int flag=true;
    char *temp=string.word;
    while (*temp)
    {
        if(*temp==delimiter)
            flag=true;
        else if(flag){
            (*size)++;
            flag=false;
        }
        temp++;
    }
    String *array=(String *)malloc((*size)*sizeof(String));
    for (int i = 0; i < *size; i++)
    {
        while (*string.word==delimiter) string.word++;
        int len=0;
        temp=string.word;
        while(*temp!=delimiter){
            len++;
            temp++;
        }
        array[i].word=(char *)malloc((len+1)*sizeof(char));
        temp=array[i].word;
        while(*string.word && *string.word!=delimiter) *temp++=*string.word++;
        *temp='\0';
    }
    return array;
}

String join(String *strings, char delimiter, int size){
    int len=0;
    for (int i = 0; i < size; i++){
        int tempLen=0;
        while (strings[i].word[tempLen]) tempLen++;
        len+=tempLen;
    }
    String new;
    new.word = (char *)malloc((len+size)*sizeof(char));
    char *tempNew=new.word;
    for (int i = 0; i < size; i++)
    {
        char *tempOld=strings[i].word;
        while(*tempOld) *tempNew++=*tempOld++;
        if(i<size-1) *tempNew++=delimiter;
    }
    *tempNew='\0';
    return new;
}

int parse(String string, int base){
    int numericEquivalent(char c){
        if(c>='0' && c<='9') return (int)c-48;
        if(c>='A' && c<='Z') return (int)c-55;
        if(c>='a' && c<='z') return (int)c-87;
        return -1;
    }
    int num=0;
    string=trim(string,' ');
    while (*string.word) num=num*base+numericEquivalent(*string.word++);
    return num;
}

String convert(int num, int base){
    char characterEquivalent(int n){
        if(n>=0 && n<=9) return (char)(n+48);
        if(n>=10 && n<=35) return (char)(n+55);
        return '\0';
    }
    int len=0;
    int temp=num;
    while(temp){
        len++;
        temp/=base;
    }
    String new;
    new.word = (char *)malloc((len+1)*sizeof(char));
    new.word[len]='\0';
    while (len)
    {
        new.word[--len]=characterEquivalent(num%base);
        num/=base;
    }
    return new;
}

int main(){
    String str1=init("Zulqarnain Chishti");
    printf("%s\n",str1.word);
    printf("starts with %d\n",startsWith(str1,"Zulqar"));
    printf("ends with %d\n",endsWith(str1,"Chishti"));
    String str2=pop(str1,5);
    printf("%s\n",str2.word);
    String str3=discard(str1,7,5);
    printf("%s\n",str3.word);
    String str4=slice(str1,7,20);
    printf("%s\n",str4.word);
    String str5=reverse(str1);
    printf("%s\n",str5.word);
    String str6=repeat(str1,3);
    printf("%s\n",str6.word);

    String str7=init("Z4lq6rN6!n");
    printf("\n%s\n",str7.word);
    printf("is upper %d\n",isUpper(str7));
    String str8=toUpper(str7);
    printf("%s\n",str8.word);
    printf("is upper %d\n",isUpper(str8));
    printf("is lower %d\n",isLower(str7));
    String str9=toLower(str7);
    printf("%s\n",str9.word);
    printf("is lower %d\n",isLower(str9));

    String str10=init("ZulqarNa1n");
    printf("\n%s\n",str10.word);
    printf("are alphabets %d\n",isAlphabet(str10));
    printf("are numbers %d\n",isNumber(str10));
    printf("are alphanumeric %d\n",isAlphanum(str10));
    printf("are spaces %d\n",isSpace(str10));
    String str11=leftAlign(str10,'*',15);
    printf("%s\n",str11.word);
    String str12=centerAlign(str10,'*',15);
    printf("%s\n",str12.word);
    String str13=rightAlign(str10,'*',15);
    printf("%s\n",str13.word);

    String str14=init("xxxZulqarnain Chishtixxx");
    printf("\n%s>%d\n",str14.word,length(str14));
    String str15=trim(str14,'x');
    printf("%s>%d\n",str15.word,length(str15));

    int size=0;
    String str16=init(" this is a sample line to try out split and join ");
    String *words=split(str16,' ',&size);
    for (int i = 0; i < size; i++)
    {
        printf("\n%s",toUpper(words[i]).word);
    }
    String line=join(words,size,'|');
    printf("\njoined: %s",line.word);
    
    String str17=init("  1111011  ");
    printf("\n%s>%s",str17.word,convert(parse(str17,2),16));

    return 0;
}