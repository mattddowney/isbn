/*
The MIT License

Copyright (c) 2011 Matthew Downey <mattddowney@gmail-NOSPAM-.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

---

This program converts between 10 and 13-digit ISBN (EAN).
ISBN is the number tracking system for books. The old standard was 10-digit ISBN,
whereas the new standard is an EAN based 13-digit system.

The program takes input from the command line, seperated by spaces.
Example:
isbn 0132404168 9780132404167

Yields the following output:
9780132404167
0132404168

To provide input from a file, pipe through xargs:
cat somefile | xargs isbn

*/
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	/* check to see if there are some arguments for input */
	if(argc > 1)
	{
		int i, length, checkdigit;
		char EAN[13], ISBN[11], prefix[3], checkch[0];

		/* convert each argument */
		for(i = 1; i < argc; i++)
		{
			/* empty all arrays */
			EAN[0] = '\0';
			ISBN[0] = '\0';
			prefix[0] = '\0';
			checkch[0] = '\0';
			
			/* figure out the length of each argument */
			length = strlen(argv[i]);
			
			/* if it's 13 digits, assume it's an EAN and convert to ISBN */
			if (length == 13)
			{
				/* get the argument, and stuff it into EAN */
				sprintf(EAN, "%s", argv[i]);
				
				/* get the first 3 digits of the EAN */
				strncat(prefix, EAN, 3);
				
				/* EANs must start with 978 */
				if (!strcmp(prefix, "978"))
				{
					/* calculate the checkdigit */
					checkdigit = 11-((((EAN[3]-'0') * 10) + ((EAN[4]-'0') * 9) + ((EAN[5]-'0') * 8) + ((EAN[6]-'0') * 7) + ((EAN[7]-'0') * 6) + ((EAN[8]-'0') * 5) + ((EAN[9]-'0') * 4) + ((EAN[10]-'0') * 3) + ((EAN[11]-'0') * 2)) % 11);
					if (checkdigit == 10)
						sprintf(checkch, "%s", "X");
					else
						sprintf(checkch, "%d", checkdigit);

					/* output the converted ISBN */
					printf("%c%c%c%c%c%c%c%c%c%c\n", EAN[3], EAN[4], EAN[5], EAN[6], EAN[7], EAN[8], EAN[9], EAN[10], EAN[11], checkch[0]);
				}
				else
					printf("EAN must begin with 978.\n");
			}
			
			/* if input argument is 10 digits, assume it's an ISBN */
			else if (length == 10)
			{
				/* stuff the argument into ISBN */
				sprintf(ISBN, "%s", argv[i]);
				
				/* calculate the checkdigit */
				checkdigit = (38 + ((ISBN[0]-'0') * 3) + (ISBN[1]-'0') + ((ISBN[2]-'0') * 3) + (ISBN[3]-'0') + ((ISBN[4]-'0') * 3) + (ISBN[5]-'0') + ((ISBN[6]-'0') * 3) + (ISBN[7]-'0') + ((ISBN[8]-'0') * 3)) % 10;
				if (checkdigit != 0)
					checkdigit = 10 - checkdigit;
					
				/* output converted EAN */
				printf("978%c%c%c%c%c%c%c%c%c%d\n", ISBN[0], ISBN[1], ISBN[2], ISBN[3], ISBN[4], ISBN[5], ISBN[6], ISBN[7], ISBN[8], checkdigit);
			}
			else
				printf("Invalid input.\n");
		}
	}
	else
	{
		printf("Convert between ISBN-10 and ISBN-13 (EAN) and vice-versa.\n");
		printf("Seperate ISBNs to convert by a space.\n");
		printf("Example Usage: isbn 9781418837235 1418837237\n");
	}

	return 0;
}
