#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void incs(char sym, int pos, int arg){
	fprintf(stderr, "permcot: incorrect symbol '%c' at position %d in argument %d\n", sym, pos+1, arg);
}

int main(int argc, char* argv[]) {
	if (argc < 2 || (argc < 3 && strcmp(argv[1], "-u") == 0)) {
		puts("permcot - UNIX Permissions Converter Tool.");
		puts("Usage: permcot [-u] <number or -rwxrwxrwx> ...");
		puts("  -u     number/result for umask");
		return 0;
	}
	int um = strcmp(argv[1], "-u") == 0;
	for (int c = um ? 2 : 1; c < argc; c++){
		char* str = argv[c];
		int slen = strlen(str);
		int inv = 0;
		if (slen == 10) {
			int res = um ? -777 : 0;
			if (str[0] == '-' || str[0] == 'd' || str[0] == 'l' ||
			str[0] == 'b' || str[0] == 'c' || str[0] == 'p' || str[0] == 's') {
				for (int i = 1; i < 10; i++) {
					if (i % 3 == 0) {
						char spec = i < 9 ? 's' : 't';
						if (str[i] != 'x' && tolower(str[i]) != spec && str[i] != '-') {
							incs(str[i], i, c);
							inv = 1;
							continue;
						}
							res += ((tolower(str[i]) == spec && !um) ? (i < 4 ? 4000 : i < 7 ? 2000 : 1000) : 0) + 
								((str[i] == 'x' || str[i] == spec) ? (i < 4 ? 100 : i < 7 ? 10 : 1) : 0);
					} else {
						char sym = i % 3 == 2 ? 'w' : 'r';
						int add = (i % 3 == 2 ? 2 : 4) * (i < 4 ? 100 : i < 7 ? 10 : 1);
						if (str[i] != sym && str[i] != '-') {
							incs(str[i], i, c);
							inv = 1;
							continue;
						}
						res += str[i] == sym ? add : 0;
					}
				}
				if (!inv) {
					printf("%04d\n", abs(res));
				}
			} else {
				fprintf(stderr, "permcot: incorrect file type '%c'\n", str[0]);
				continue;
			}
		} else if (slen < 5 && isdigit(str[0])) {
			char res[11];
			strcpy(res, um ? "drwxrwxrwx" : "----------");
			int pos = 7;
			if (slen == 4 && !um) {
				int num = str[0] - '0';
				if (num >= 4) {
					num -= 4;
					res[3] = 'S';
				}
				if (num >= 2) {
					num -= 2;
					res[6] = 'S';
				}
				if (num >= 1) {
					num--;
					res[9] = 'T';
				}
			}
			for (int i = slen - 1 ; i >= (slen == 4) ; i--) {
				if (!isdigit(str[i]) || str[i] - '0' > 7) {
					incs(str[i], i, c);
					inv = 1;
					continue;
				}
				int num = str[i] - '0';
				if (num >= 4) {
					num -= 4;
					res[pos] = um ? '-' : 'r';
				}
				pos++;
				if (num >= 2) {
					num -= 2;
					res[pos] = um ? '-' : 'w';
				}
				pos++;
				if (num >= 1) {
					num--;
					res[pos] = um ? '-' : (res[pos] == '-' ? 'x' : tolower(res[pos]));
				}
				pos-=5;
			}
			if (um && !inv) {
				char resnx[11];
				strcpy(resnx, res);
				resnx[0] = '-';
				resnx[3] = '-';
				resnx[6] = '-';
				resnx[9] = '-';
				printf("%s     %s\n", resnx, res);
			}
			else if (!inv) {
				printf("%s\n", res);
			}
		} else {
			fprintf(stderr, "permcot: incorrect argument %d\n", c);
			continue;
		}
	}
	return 0;
}
