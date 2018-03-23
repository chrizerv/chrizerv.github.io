#define WORD ((c>='A' && c<='z') || (c>='0' && c<='9')) //Αποφεύγοντας το μακρυνάρι :D


struct words
{
	char *name;
	int times;
};


int is_empty(FILE *,FILE *);

int words_in_list(FILE *);

void read_wl_file(FILE *,struct words*, int);

int word_len_alloc(FILE *,struct words* ,int);

void display(struct words*, int);

int is_not_correct(FILE *);

void times(FILE*, struct words*,int);

void sel_sort(struct words*, int);

void usage(void);



void usage(void)
{
	printf("  _   _   _   _   _     _   _   _   _   _   _   _  \n");
		printf(" / \\ / \\ / \\ / \\ / \\   / \\ / \\ / \\ / \\ / \\ / \\ / \\ \n");
		printf("( W | o | r | d | s ) ( C | o | u | n | t | e | r ) \n");
		printf(" \\_/ \\_/ \\_/ \\_/ \\_/   \\_/ \\_/ \\_/ \\_/ \\_/ \\_/ \\_/ \n");
		printf("                                                  ..by Chris Zervas\n");


		printf("\n+++++++++++++++++++++++++++++++++++++++++++++\n");
		printf("\nUsage: program {source file} {wordlist}\n");
		printf("\nEx: $./wcounter test.txt list.txt\n\n");
		printf("+++++++++++++++++++++++++++++++++++++++++++++\n");


		printf("\n*RULES-RESTRICTIONS in wordlist file:\n\n");
		
		printf("~ Every word can only contain A-z letters and 0-9 numbers.\n");
		printf("~ Put every different word in a new line.\n");
		printf("~ You can't use extra new lines.\n\n");
}

/*Η συνάρτηση "is_not_correct" ελέγχει άν το αρχείο με τις λέξεις, περιέχει κάποιο χαρακτήρα που δέν συμπεριλαμβάνεται σε λέξη,
 καθώς και για το άν υπάρχουνε περιττές αλλαγές γραμμής.
*/ 
int is_not_correct(FILE *wl)
{
	char c;
	int pos;
	fseek(wl,0,SEEK_SET);
	while(!feof(wl))
	{
		pos = ftell(wl);
		c = fgetc(wl);

		if (!WORD && (c!='\n' && c!=EOF))
		{
			printf("\nError: Wordlist file can only contain (a-z,A-Z,0-9).\n\n");
			return 1;
		}
			
		
		
		if (c == '\n')
		{
			fseek(wl,pos-1,SEEK_SET);
			
			c = fgetc(wl);
			if (!WORD || pos == 0) //Όταν το pos είναι 0 τότε υπάρχει περιττή  αλλαγή γραμμής, πρίν απο την αρχή των λέξεων.
			{
				printf("\nError: There is an extra new line in the wordlist file.\n\n");
				return 1;
			}
			fseek(wl,1,SEEK_CUR);
		}
			
	}


	return 0;
}


/*Η συνάρτηση "is_empty" ελέγχει αν ένα απο τα 2 αρχεία είναι άδεια. */ 
int is_empty(FILE *sf,FILE *wl)
{
	int f,l;
	fseek(sf,0,SEEK_END);
	fseek(wl,0,SEEK_END);
	f = ftell(sf);
	l = ftell(wl);
	if (f == 0)
	{
		printf("\nSource file is empty.\n\n");
		return 1;
	}
	else if (l == 0)
	{
		printf("\nWordlist file is empty.\n\n");
		return 1;
	}
	else
		return 0;
}


/*H συνάρτηση "words_in_list" μετρά πόσες λέξεις έχει η λίστα του αρχείου*/
int words_in_list(FILE *wl){
	char c;
	int num=0;
	fseek(wl,0,SEEK_SET);
	while(!feof(wl))
	{
		c = fgetc(wl);
		if (c == '\n')
			num++;
	}

	return num;
	
}


/*Η συνάρτηση "word_len_alloc" μετρά το μέγεθος κάθε λέξης στην λίστα και δεσμέυει χώρο στην δομή που της αντιστοιχεί.*/
int word_len_alloc(FILE *wl,struct words *wr,int slen)
{
	int i =0;
	char c;
	int wlen =0;
	fseek(wl,0,SEEK_SET);

	while(!feof(wl))
	{
		c= fgetc(wl);
		
		if(c == '\n')
		{
			
			if (i>slen)
			{
				printf("Out of Bounds\n");
				return 1;
			}

			wr[i].name = (char *)malloc(wlen+1); // +1 Για το '\0'.
			if (wr[i].name == NULL)
			{
				printf("\nOut of memory on list reading\n\n");
				return 1;
			}

			i++;
			wlen =0;
		}
		else
			wlen++;

	}


	return 0;
}



/*Η συνάρτηση "read_wl_file" καταχωρεί κάθε λέξη της λίστας στην δομή που της αντιστοιχεί.*/
void read_wl_file(FILE *wl,struct words *wr,int slen)
{
	int i;
	
	fseek(wl,0,SEEK_SET);

	for(i=0;i<slen;i++) 
	{
		fscanf(wl,"%s",wr[i].name);
		wr[i].times = 0;
	}

}



/*Η συνάρτηση "times" ξεκινά να εντοπίζει λέξεις στο αρχείο κειμένου. Όταν βρεθεί στην αρχή κάποιας λέξης, ελέγχει άν είναι ίδια
με κάποια απο αυτές που περιέχει ο πίνακας δομών. Τέλος άν είναι ίδια και μεμονομένη, αυξάνει τον μετρητή της αντίστοιχης δομής. */    
void times(FILE *sf,struct words *wr,int slen)
{
	int i,j=0;
	int chk=0;
	char c;
	int nlen;
	int within =0;
	for(i=0;i<slen;i++)
	{
		nlen =strlen(wr[i].name);
		fseek(sf,0,SEEK_SET);
		while(!feof(sf))
		{
			c = fgetc(sf);
			if (WORD)
			{
				if (within ==0)
				{
					if(c==wr[i].name[0])
					{
						
						while((j<nlen && chk == 0) && WORD)
						{
							if (c !=wr[i].name[j])
								chk = 1;
							else
								j++;

							c = fgetc(sf);
						}

						if (j == nlen && !WORD)
							wr[i].times++;
						
						j=0;
						chk=0;
						
					}

					if(WORD)
						within =1;
				}

			}
			else 
				within=0;
			
				
		}
	}

}


void display(struct words *wr,int slen)
{
	int i=0;

	printf("\n-----------------------------------------\n");
	for(i=0;i<slen;i++)
		printf("\n - %s: %d times \n",wr[i].name,wr[i].times);

	printf("\n------------------------------------------\n");
}


//Selection sort
void sel_sort(struct words *wr,int slen)
{
	int i,j,pos;
	struct words temp;
	for(i=0;i<slen-1;i++)
	{
		pos =i;
		for(j=i+1;j<slen;j++)
			if (wr[j].times > wr[pos].times)
				pos = j;

		temp = wr[i];
		wr[i]= wr[pos];
		wr[pos] = temp;
	}
}