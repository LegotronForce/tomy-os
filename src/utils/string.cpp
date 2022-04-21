/*********************** FUNCTIONS ********************************
* reverse: reverses a string                                      *
* strLen: returns length of a string                              *
* StringsEqu: returns true if strings are equal (max len 80)      *
* StringStartsWith: returns true if s starts with t (max len 80)  *
******************************************************************/
#include "../mem.h"

const char* reverse(const char* str, char* buffer, int len){
	int i;
	for(i = 0; i <= len; i++) {
		buffer[i] = str[len - i];
	}
	buffer[i] = '\0';
	return buffer;
}

int strLen(const char* s) {
	int res;
	for(res = 0; s[res] != 0; res++);
	return res;
}

char * itoa( int value, char * str, int base )
{
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}


bool StringsEqu(const char* s, const char* t){
	const char* g = strLen(s) > strLen(t)? s : t;
	
	bool res = true;
	int i = 0;
	do{
		if(s[i] != t[i] || i > 80){
			res = false;
			break;
		}
		else i++;
	}while(g[i] != 0);
	return res;
}

bool StringStartsWith(const char* s, const char* t){
    const char* g = t;
	
	bool res = true;
	int i = 0;
	do{
		if(s[i] != t[i] || i > 80){
			res = false;
			break;
		}
		else i++;
	}while(g[i] != 0);
	return res;
    
}


int strFindChar(const char* s, char c){
    int i = 0;
	do{
		if(s[i] == c || i > 80){
			break;
		}
		else i++;
	}while(s[i] != 0);
    return(s[i] == 0 || i > 80 ? -1 : i);
}


const char* strDecapitate(const char* s, int n){            // Uses Dynamic Memory Allocation, be careful.
    int newLen = strLen(s) - n;
    if(newLen <= 0) return ""; 
    char* t = (char*)kmalloc(sizeof(char) * (newLen + 1));
    for(int i = 0; i < newLen; i++)
        t[i] = s[i + n];
    return t;
}
