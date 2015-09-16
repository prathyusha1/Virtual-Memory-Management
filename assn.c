#include <stdio.h>
#include <stdlib.h>
#define no_of_processes 10
int page_size,memory_size;
int row_size[no_of_processes];
int replacer;
int page_status;
int counter[no_of_processes]={0};
int arrivalnumber[no_of_processes]={0};
int globalarrivalno=0;
int references;
int pagefaultcount=0;
FILE *out_file;
struct pagetable
{
	int arrivalno;
	int row_no;
	//int no_of_rows;
	int pageno;
	int frameno;
	int pagesize;
	int validity;
	int ref_bit;
	int modify_bit;
	int history_bit;
	struct pagetable *next;
};
struct pagetable *pageprocess[no_of_processes];
void createprocess(int processno)
{
	int j;
	struct pagetable *p1,*p2;
	pageprocess[processno]=(struct pagetable *)malloc(sizeof(struct pagetable));
	pageprocess[processno]->row_no=0;
	pageprocess[processno]->arrivalno=-1;
	pageprocess[processno]->pageno=0;
	pageprocess[processno]->frameno=-1;
	pageprocess[processno]->pagesize=page_size;
	pageprocess[processno]->validity=0;
	pageprocess[processno]->ref_bit=0;
	pageprocess[processno]->modify_bit=0;
	pageprocess[processno]->history_bit=0;
	pageprocess[processno]->next=NULL;
	p1=pageprocess[processno];
	for(j=1;j<row_size[processno];j++)
	{
		p2=(struct pagetable *)malloc(sizeof(struct pagetable));
		p2->row_no=j;
		p2->arrivalno=-1;
		p2->pageno=j;
		p2->frameno=-1;
		p2->pagesize=page_size;
		p2->validity=0;
		p2->ref_bit=0;
		p2->modify_bit=0;
		p2->history_bit=0;
		p2->next=NULL;
		p1->next=p2;
		p1=p1->next;
	}
}

void FIFOlocal(int input1,int input2,struct pagetable *pointer,int limit,int pagereplacement)
{
		int current_process=input1;
		int current_reference=input2;
		struct pagetable *current_ptr=pointer;
		pagefaultcount++;
			
			//printf("page fault occurs\n");
			if(replacer<memory_size&&current_ptr)
			{
				//pagefaultcount++;
				fprintf(out_file,"page fault occured for page %d of process %d\n",current_reference,current_process);
				current_ptr->frameno=replacer;
				current_ptr->validity=1;
				current_ptr->arrivalno=arrivalnumber[current_process];
				arrivalnumber[current_process]++;
				fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_reference,current_ptr->frameno);
				replacer++;
				//int next=((current_ptr->pageno/current_ptr->pagesize)+1)%limit[]
				if(pagereplacement==1)
				{
					if((current_ptr->pageno+1)<limit)
					{
						if(replacer<memory_size&&current_ptr->next)
						{
						current_ptr=current_ptr->next;
						current_ptr->frameno=replacer;
						current_ptr->validity=1;
						current_ptr->arrivalno=arrivalnumber[current_process];
						arrivalnumber[current_process]++;
						fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_ptr->pageno,current_ptr->frameno);

						replacer++;
						}
					}
					else
					{
						current_ptr=pageprocess[current_process];
						if(replacer<memory_size&&current_ptr)
						{
							//current_ptr=current_ptr->next;
							current_ptr->frameno=replacer;
							current_ptr->validity=1;
							current_ptr->arrivalno=arrivalnumber[current_process];
							arrivalnumber[current_process]++;
							fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_ptr->pageno,current_ptr->frameno);
							replacer++;
						}
					}
				}
			}
			else
			{
				//pagefaultcount++;
				//printf("replacer=%d",replacer);
				fprintf(out_file,"page fault occured for page %d of process %d\n",current_reference,current_process);
					//Local page replacement
				struct pagetable *ptr=pageprocess[current_process];
				int leastarrivalno=1000000;
				while(ptr)
				{
					if(ptr->arrivalno==-1)
					{
						ptr=ptr->next;
					}
					else
					{
						if(ptr->arrivalno<leastarrivalno&&ptr->validity==1)
						{
							leastarrivalno=ptr->arrivalno;
						}
						ptr=ptr->next;
					}
				}
				ptr=pageprocess[current_process];
				while(ptr&&ptr->arrivalno!=leastarrivalno)
				{
					ptr=ptr->next;
				}
				if(ptr)
				{
					//printf("page %d for process %d should be replaced,its frameno=%d\n",ptr->pageno,current_process,ptr->frameno);
					fprintf(out_file,"The first in page is %d of process %d\n",ptr->pageno,current_process);
					current_ptr->frameno=ptr->frameno;
					current_ptr->arrivalno=arrivalnumber[current_process];
					arrivalnumber[current_process]++;
					current_ptr->validity=1;
					ptr->validity=0;
					ptr->frameno=-1;
					ptr->arrivalno=-1;
					fprintf(out_file,"page %d of process %d is brought to frame %d\n",current_ptr->pageno,current_process,current_ptr->frameno);
				}
			}
		
	//}

}
void LRUlocal(int input1,int input2,struct pagetable *pointer,int limit,int pagereplacement)
{
	int current_process=input1;
		int current_reference=input2;
		struct pagetable *current_ptr=pointer;
				{
			
			//printf("page fault occurs\n");
			if(replacer<memory_size)
			{
				pagefaultcount++;
				fprintf(out_file,"page fault occured for page %d of process %d\n",current_reference,current_process);
				current_ptr->frameno=replacer;
				current_ptr->validity=1;
				current_ptr->ref_bit=1;
				current_ptr->arrivalno=arrivalnumber[current_process];
				arrivalnumber[current_process]++;
				fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_reference,current_ptr->frameno);
				replacer++;
				if(pagereplacement==1)
				{
					if((current_ptr->pageno+1)<limit)
					{
						if(replacer<memory_size&&current_ptr->next)
						{
						current_ptr=current_ptr->next;
						current_ptr->frameno=replacer;
						current_ptr->validity=1;
						current_ptr->ref_bit=1;
						current_ptr->arrivalno=arrivalnumber[current_process];
						arrivalnumber[current_process]++;
						fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_ptr->pageno,current_ptr->frameno);
						replacer++;
						}
					}
					else
					{
						current_ptr=pageprocess[current_process];
						if(replacer<memory_size&&current_ptr)
						{
						//current_ptr=current_ptr->next;
						current_ptr->frameno=replacer;
						current_ptr->validity=1;
						current_ptr->ref_bit=1;
						current_ptr->arrivalno=arrivalnumber[current_process];
						arrivalnumber[current_process]++;
						fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_ptr->pageno,current_ptr->frameno);
						replacer++;
						}


					}
				}
				
			}
			else
			{
				pagefaultcount++;
				fprintf(out_file,"page fault occured for page %d of process %d\n",current_reference,current_process);				
				//Local page replacement
				struct pagetable *ptr=pageprocess[current_process];
				int leastarrivalno=100000000;
				while(ptr)
				{
					if(ptr->arrivalno==-1)
					{
						ptr=ptr->next;
					}
					else
					{
						if(ptr->arrivalno<leastarrivalno&&ptr->validity==1)
						{
							leastarrivalno=ptr->arrivalno;
						}
						ptr=ptr->next;
					}
				}
				ptr=pageprocess[current_process];
				while(ptr&&ptr->arrivalno!=leastarrivalno)
				{
					ptr=ptr->next;
				}
				if(ptr&&ptr->arrivalno!=-1&&ptr->validity==1&&ptr->ref_bit==1)
				{
					fprintf(out_file,"The least recently used page is page %d of process %d\n",ptr->pageno,current_process);
					//printf("page %d for process %d should be replaced,its frameno=%d\n",ptr->pageno,current_process,ptr->frameno);
					current_ptr->frameno=ptr->frameno;
					current_ptr->arrivalno=arrivalnumber[current_process];
					arrivalnumber[current_process]++;
					current_ptr->validity=1;
					current_ptr->ref_bit=1;
					ptr->validity=0;
					ptr->frameno=-1;
					ptr->arrivalno=-1;
					ptr->ref_bit=0;
					fprintf(out_file,"page %d of process %d is brought to frame %d\n",current_ptr->pageno,current_process,current_ptr->frameno);
				}
			}
		}
	//}

}
void ClockLocal(int input1,int input2,struct pagetable *pointer,int limit,int pagereplacement)
{
	int current_process=input1;
	int current_reference=input2;
	struct pagetable *current_ptr=pointer;
	
			if(replacer<memory_size&&current_ptr)
			{
				pagefaultcount++;
				fprintf(out_file,"page fault occured for page %d of process %d\n",current_reference,current_process);
				current_ptr->frameno=replacer;
				current_ptr->validity=1;
				current_ptr->ref_bit=1;
				current_ptr->arrivalno=arrivalnumber[current_process];
				arrivalnumber[current_process]++;
				fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_reference,current_ptr->frameno);
				replacer++;
				if(pagereplacement==1)
				{
					if((current_ptr->pageno+1)<limit)
					{
						if(replacer<memory_size&&current_ptr->next)
						{
						current_ptr=current_ptr->next;
						current_ptr->frameno=replacer;
						current_ptr->validity=1;
						current_ptr->arrivalno=arrivalnumber[current_process];
						arrivalnumber[current_process]++;
						current_ptr->ref_bit=0;
						fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_ptr->pageno,current_ptr->frameno);
						replacer++;
						}
						else
						{
								current_ptr=pageprocess[current_process];
								if(replacer<memory_size&&current_ptr)
								{
								//current_ptr=current_ptr->next;
								current_ptr->frameno=replacer;
								current_ptr->validity=1;
								current_ptr->arrivalno=arrivalnumber[current_process];
								arrivalnumber[current_process]++;
								current_ptr->ref_bit=0;
								fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_ptr->pageno,current_ptr->frameno);
								replacer++;
								}
						}
					}
				}
			}
			else
			{
				pagefaultcount++;
				fprintf(out_file,"page fault occured for page %d of process %d\n",current_reference,current_process);
				struct pagetable *ptr=pageprocess[current_process];
				int leastarrivalno=10000000;

				while(ptr)
				{
					if(ptr->arrivalno==-1)
					{
						ptr=ptr->next;
					}
					else
					{
						if(ptr->arrivalno<leastarrivalno&&ptr->validity==1)
						{
							leastarrivalno=ptr->arrivalno;
						}
						ptr=ptr->next;
					}
				}

				ptr=pageprocess[current_process];
				while(ptr&&ptr->arrivalno!=leastarrivalno)
				{
					ptr=ptr->next;
				}
				while(ptr)
				{
					if(ptr&&ptr->arrivalno!=-1&&ptr->validity==1)
					{
						if(ptr->ref_bit==0)
						{
							fprintf(out_file,"can be replaced\n");
							current_ptr->frameno=ptr->frameno;
							current_ptr->ref_bit=1;
							current_ptr->validity=1;
							current_ptr->arrivalno=arrivalnumber[current_process];
							arrivalnumber[current_process]++;
							ptr->validity=0;
							ptr->frameno=-1;
							ptr->arrivalno=-1;
							fprintf(out_file,"According to clock alogrithm page to be replaced is page %d of process %d\n",ptr->pageno,current_process);
							fprintf(out_file,"page %d of process %d is brought to frame %d\n",current_ptr->pageno,current_process,current_ptr->frameno);
							//ptr=ptr->next;
							break;
						}
						else if(ptr->ref_bit==1)
						{
							ptr->ref_bit=0;
							ptr->arrivalno=arrivalnumber[current_process];
							arrivalnumber[current_process]++;
							//ptr=ptr->next;
							int secondleastarrivalno=100000000;
							struct pagetable *ptr2=pageprocess[current_process];
							while(ptr2)
							{
								if(ptr2->arrivalno==-1)
								{
									ptr2=ptr2->next;
								}
								else
								{
									if(ptr2->arrivalno<secondleastarrivalno&&ptr->validity==1&&ptr2->arrivalno>leastarrivalno)
									{
										secondleastarrivalno=ptr->arrivalno;
									}
									ptr2=ptr2->next;				
								}
							}
							ptr2=pageprocess[current_process];
							while(ptr2&&ptr2->arrivalno!=secondleastarrivalno)
							{
								ptr2=ptr2->next;
							}
							ptr=ptr2;
						}
						else
						{
							ptr=ptr->next;
						}
					}
					else
					{
						ptr=ptr->next;
					}
					if(ptr->next==NULL)
					{
						ptr=pageprocess[current_process];
					}
				}
				/*while(ptr)
				{
					if(ptr->ref_bit==0&&ptr->validity==1)
					{
						printf("can be replaced");
						current_ptr->frameno=ptr->frameno;
						current_ptr->ref_bit=1;
						current_ptr->validity=1;
						ptr->validity=0;
						ptr->frameno=-1;
						ptr=ptr->next;
						printf("According to clock alogrithm page to be replaced is page %d of process %d\n",ptr->pageno,current_process);
						printf("page %d of process %d is brought to frame %d\n",current_ptr->pageno,current_process,current_ptr->frameno);
						break;

					}
					else if(ptr->ref_bit==1&&ptr->validity==1)
					{
						ptr->ref_bit=0;
						ptr->arrivalno=arrivalnumber[current_process];
						arrivalnumber[current_process]++;
						ptr=ptr->next;
					}
					else
					{
						ptr=ptr->next;
					}
					if(ptr->next==NULL)
					{
						ptr=pageprocess[current_process];
					}
				}*/
			}
	}

	void FIFOglobal(int input1,int input2,struct pagetable *pointer,int limit,int pagereplacement)
	{
		int current_process=input1;
		int i;
		int current_reference=input2;
		struct pagetable *current_ptr=pointer;
			pagefaultcount++;
			//printf("page fault occurs\n");
			if(replacer<memory_size)
			{
				fprintf(out_file,"page fault occured for page %d of process %d\n",current_reference,current_process);
				current_ptr->frameno=replacer;
				current_ptr->validity=1;
				current_ptr->ref_bit=1;
				current_ptr->arrivalno=globalarrivalno;
				globalarrivalno++;
				//arrivalnumber[current_process]++;
				fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_reference,current_ptr->frameno);
				replacer++;
				if(pagereplacement==1)
				{
					if((current_ptr->pageno+1)<limit)
					{
						if(replacer<memory_size&&current_ptr->next)
						{
						current_ptr=current_ptr->next;
						current_ptr->frameno=replacer;
						current_ptr->validity=1;
						current_ptr->arrivalno=globalarrivalno;
						globalarrivalno++;
						//arrivalnumber[current_process]++;
						fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_ptr->pageno,current_ptr->frameno);

						replacer++;
						}
					}
					else
					{
						current_ptr=pageprocess[current_process];
						if(replacer<memory_size&&current_ptr)
						{
						//current_ptr=current_ptr->next;
						current_ptr->frameno=replacer;
						current_ptr->validity=1;
						current_ptr->arrivalno=globalarrivalno;
						globalarrivalno++;
						//arrivalnumber[current_process]++;
						fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_ptr->pageno,current_ptr->frameno);

						replacer++;
						}
					}
				}
			}
			else
			{
				int leastarrivalno=100000000;
				//printf("replacer=%d",replacer);
				fprintf(out_file,"page fault occured for page %d of process %d\n",current_reference,current_process);
				for(i=0;i<no_of_processes;i++)
				{
					struct pagetable *ptr=pageprocess[i];
					while(ptr)
					{
						if(ptr->arrivalno==-1)
						{
							ptr=ptr->next;
						}
						else
						{
							if(ptr->arrivalno<leastarrivalno&&ptr->validity==1)
							{
							leastarrivalno=ptr->arrivalno;
							}
							ptr=ptr->next;

						}
					}

				}

				for(i=0;i<no_of_processes;i++)
				{
					struct pagetable *ptr=pageprocess[i];
					while(ptr&&ptr->arrivalno!=leastarrivalno)
					{
						ptr=ptr->next;
					}
					if(ptr)
					{
										//printf("page %d for process %d should be replaced,its frameno=%d\n",ptr->pageno,current_process,ptr->frameno);
							fprintf(out_file,"The first in page is %d of process %d\n",ptr->pageno,current_process);
							current_ptr->frameno=ptr->frameno;
							current_ptr->arrivalno=globalarrivalno;
							//arrivalnumber[current_process]++;
							globalarrivalno++;
							current_ptr->validity=1;
							current_ptr->ref_bit=1;
							ptr->validity=0;
							ptr->frameno=-1;
							ptr->ref_bit=0;
							ptr->arrivalno=-1;
							fprintf(out_file,"page %d of process %d is brought to frame %d\n",current_ptr->pageno,current_process,current_ptr->frameno);
							break;
					}
				}

			}
		

	}

	void LRUglobal(int input1,int input2,struct pagetable *pointer,int limit,int pagereplacement)
	{
		int current_process=input1;
		int current_reference=input2;
		int i;
		struct pagetable *current_ptr=pointer;
			pagefaultcount++;
			//printf("page fault occurs\n");
			if(replacer<memory_size)
			{
				fprintf(out_file,"page fault occured for page %d of process %d\n",current_reference,current_process);
				current_ptr->frameno=replacer;
				current_ptr->validity=1;
				current_ptr->arrivalno=globalarrivalno;
				current_ptr->ref_bit=1;
				globalarrivalno++;
				fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_reference,current_ptr->frameno);
				replacer++;
				if(pagereplacement==1)
				{
					if((current_ptr->pageno+1)<limit)
					{
						if(replacer<memory_size&&current_ptr->next)
						{
						current_ptr=current_ptr->next;
						current_ptr->frameno=replacer;
						current_ptr->validity=1;
						current_ptr->arrivalno=globalarrivalno;
						globalarrivalno++;
						current_ptr->ref_bit=1;
						fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_ptr->pageno,current_ptr->frameno);
						replacer++;
						}
					}
					else
					{
						current_ptr=pageprocess[current_process];
						if(replacer<memory_size&&current_ptr)
						{
						//current_ptr=current_ptr->next;
						current_ptr->frameno=replacer;
						current_ptr->validity=1;
						current_ptr->arrivalno=globalarrivalno;
						globalarrivalno++;
						current_ptr->ref_bit=1;
						fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_ptr->pageno,current_ptr->frameno);
						replacer++;
						}

					}
				}
			}
			else
			{
				fprintf(out_file,"page fault occured for page %d of process %d\n",current_reference,current_process);				
				int leastarrivalno=1000000;
				for(i=0;i<no_of_processes;i++)
				{
					struct pagetable *ptr=pageprocess[i];
					while(ptr)
					{
						if(ptr->arrivalno==-1)
						{
							ptr=ptr->next;
						}
						else
						{
							if(ptr->arrivalno<leastarrivalno&&ptr->validity==1)
							{
								leastarrivalno=ptr->arrivalno;
							}
							ptr=ptr->next;
						}
					}
				}
				for(i=0;i<no_of_processes;i++)
				{
					struct pagetable *ptr=pageprocess[i];
					while(ptr&&ptr->arrivalno!=leastarrivalno)
					{
						ptr=ptr->next;
					}
					if(ptr&&ptr->arrivalno!=-1&&ptr->validity==1&&ptr->ref_bit==1)
					{
						fprintf(out_file,"The least recently used page is page %d of process %d\n",ptr->pageno,current_process);
						//printf("page %d for process %d should be replaced,its frameno=%d\n",ptr->pageno,current_process,ptr->frameno);
						current_ptr->frameno=ptr->frameno;
						current_ptr->arrivalno=globalarrivalno;
						globalarrivalno++;
						current_ptr->validity=1;
						current_ptr->ref_bit=1;
						ptr->validity=0;
						ptr->frameno=-1;
						ptr->ref_bit=0;
						ptr->arrivalno=-1;
						fprintf(out_file,"page %d of process %d is brought to frame %d\n",current_ptr->pageno,current_process,current_ptr->frameno);
						break;
					}

				}
	
	
			}
	
	}

	void ClockGlobal(int input1,int input2,struct pagetable *pointer,int limit,int pagereplacement)
	{
		int current_process=input1;
		int current_reference=input2;
		int i;
		struct pagetable *current_ptr=pointer;
		pagefaultcount++;
		if(replacer<memory_size&&current_ptr)
			{
				//pagefaultcount++;
				fprintf(out_file,"page fault occured for page %d of process %d\n",current_reference,current_process);
				current_ptr->frameno=replacer;
				current_ptr->validity=1;
				current_ptr->ref_bit=1;
				current_ptr->arrivalno=globalarrivalno;
				globalarrivalno++;
				fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_reference,current_ptr->frameno);
				replacer++;
				if(pagereplacement==1)
				{
					if((current_ptr->pageno+1)<limit)
					{
						if(replacer<memory_size&&current_ptr->next)
						{
						current_ptr=current_ptr->next;
						current_ptr->frameno=replacer;
						current_ptr->validity=1;
						current_ptr->arrivalno=globalarrivalno;
						globalarrivalno++;
						current_ptr->ref_bit=0;
						fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_ptr->pageno,current_ptr->frameno);
						replacer++;
						}
					}
					else
					{
						if(replacer<memory_size&&current_ptr)
						{
						//current_ptr=current_ptr->next;
						current_ptr->frameno=replacer;
						current_ptr->validity=1;
						current_ptr->arrivalno=globalarrivalno;
						globalarrivalno++;
						current_ptr->ref_bit=0;
						fprintf(out_file,"page %d is brought to memory at frameno %d\n",current_ptr->pageno,current_ptr->frameno);
						replacer++;
						}

					}
				}
			}
			else
			{
				int leastarrivalno=100000000;
				fprintf(out_file,"pafefault occured for page %d of process %d\n",current_reference,current_process);
				for(i=0;i<no_of_processes;i++)
				{
					struct pagetable *ptr=pageprocess[i];
					while(ptr)
					{
						if(ptr->arrivalno==-1)
						{
							ptr=ptr->next;
						}
						else
						{
							if(ptr->arrivalno<leastarrivalno&&ptr->validity==1)
							{
							leastarrivalno=ptr->arrivalno;
							}
							ptr=ptr->next;

						}
					}

				}

				for(i=0;i<no_of_processes;i++)
				{
						struct pagetable *ptr=pageprocess[i];
						while(ptr&&ptr->arrivalno!=leastarrivalno)
						{
							ptr=ptr->next;
						}
						while(ptr)
						{
							if(ptr&&ptr->arrivalno!=-1&&ptr->validity==1)
							{
								if(ptr->ref_bit==0)
								{
									fprintf(out_file,"can be replaced\n");
									current_ptr->frameno=ptr->frameno;
									current_ptr->ref_bit=1;
									current_ptr->validity=1;
									current_ptr->arrivalno=globalarrivalno;
									//arrivalnumber[current_process]++;
									globalarrivalno++;
									ptr->validity=0;
									ptr->frameno=-1;
									ptr->arrivalno=-1;
									fprintf(out_file,"According to clock alogrithm page to be replaced is page %d of process %d\n",ptr->pageno,current_process);
									fprintf(out_file,"page %d of process %d is brought to frame %d\n",current_ptr->pageno,current_process,current_ptr->frameno);
									//ptr=ptr->next;
									break;
								}
								else if(ptr->ref_bit==1)
								{
									ptr->ref_bit=0;
									ptr->arrivalno=globalarrivalno;
									globalarrivalno++;
									//arrivalnumber[current_process]++;
									//ptr=ptr->next;
									int secondleastarrivalno=100000000;
									struct pagetable *ptr2=pageprocess[i];
									while(ptr2)
									{
										if(ptr2->arrivalno==-1)
										{
											ptr2=ptr2->next;
										}
										else
										{
											if(ptr2->arrivalno<secondleastarrivalno&&ptr->validity==1&&ptr2->arrivalno>leastarrivalno)
											{
												secondleastarrivalno=ptr->arrivalno;
											}
											ptr2=ptr2->next;				
										}
									}
									ptr2=pageprocess[i];
									while(ptr2&&ptr2->arrivalno!=secondleastarrivalno)
									{
										ptr2=ptr2->next;
									}
									ptr=ptr2;
								}
								else
								{
									ptr=ptr->next;
								}
							}
							else
							{
								ptr=ptr->next;
							}
							if(ptr->next==NULL)
							{
								ptr=pageprocess[i];
							}
						}
				}



			}


	}
int main()
{
	out_file=fopen("output.txt","w");
if(out_file==NULL)
{
	printf("Cannot open the output file\n");
	exit(-1);
}
	
	printf("Enter the pagesize 1,2,4...\n");
	scanf("%d",&page_size);

	memory_size=512/page_size;
	
	fprintf(out_file,"memory_size=%d\n",memory_size);
	int main_memory[memory_size];
	
	int limit[no_of_processes][2];
	
	FILE *input_file1=fopen("progAddrLimitsasn6.txt","r");
	if(input_file1==NULL)
	{
		printf("Error!, could not open the file\n");
		return;
	}
	int var1,var2;
	int i,j;
	for(i=0;i<no_of_processes;i++)
	{
			fscanf(input_file1,"%d %d",&var1,&var2);
			limit[i][0]=var1;
			limit[i][1]=var2;
	}
		int total_rows=0;
	for(i=0;i<no_of_processes;i++)
	{
		row_size[i]=limit[i][1];
	}
	for(i=0;i<no_of_processes;i++)
	{
		createprocess(i);
	}

	int trace_list[2];
	FILE *input_file2=fopen("progMemoryrefasn.txt","r");
	//FILE *input_file2=fopen("ref.txt","r");
	
		int option3;
		printf("Enter 0 for without prepaging\n, 1 for with prepaging\n");
		scanf("%d",&option3);

		int option1;
		printf("Enter 1 for local page replacement\nEnter 2 for global page replacement\n");
		scanf("%d",&option1);

		printf("\n");
		int option2;
		printf("enter 1 for FIFO\nEnter 2 for LRU\nEnter 3 for Clock\n");
		scanf("%d",&option2);

		switch(option3)
		{
			case 1:
				switch(option1)
				{
					case 1:
							switch(option2)
							{
								case 1:
									while(fscanf(input_file2,"%d %d",&trace_list[0],&trace_list[1])!=EOF)
									{	
									references++;									
													int current_process=trace_list[0];
													int current_reference=trace_list[1];
											if(current_reference>=limit[current_process][1])
											{
												fprintf(out_file,"Process %d misbehaving, Suspended...\n",current_process);
												continue;
											}
											struct pagetable *current_ptr;
											current_ptr=pageprocess[current_process];
											while(current_ptr&&current_ptr->pageno!=current_reference)
											{
												current_ptr=current_ptr->next;
											}
											if(current_ptr&&current_ptr->validity==1&&current_ptr->pageno==current_reference)
											{
												current_ptr->ref_bit=1;
												fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
												continue;
											}
											else if(current_ptr&&current_ptr->validity==0)
											{
												FIFOlocal(current_process,current_reference,current_ptr,limit[current_process][1],1);
											}
									}
								break;

								case 2:
										while(fscanf(input_file2,"%d %d",&trace_list[0],&trace_list[1])!=EOF)
										{
											references++;
											int current_process=trace_list[0];
											int current_reference=trace_list[1];
											if(current_reference>=limit[current_process][1])
											{
												fprintf(out_file,"Process %d misbehaving, Suspended...\n",current_process);
												continue;
											}
											struct pagetable *current_ptr;
											current_ptr=pageprocess[current_process];
											while(current_ptr&&current_ptr->pageno!=current_reference)
											{
												current_ptr=current_ptr->next;
											}
											if(current_ptr->validity==1&&current_ptr&&current_ptr->ref_bit==0&&current_ptr->pageno==current_reference&&current_ptr->arrivalno!=-1)
											{
												current_ptr->ref_bit=1;
												fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
												current_ptr->arrivalno=arrivalnumber[current_process];
												arrivalnumber[current_process]++;
												continue;
											}
											else if(current_ptr->validity==1&&current_ptr&&current_ptr->ref_bit==1&&current_ptr->pageno==current_reference&&current_ptr->arrivalno!=-1)
											{
												current_ptr->ref_bit=1;
												fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
												current_ptr->arrivalno=arrivalnumber[current_process];
												arrivalnumber[current_process]++;
												continue;
											}
											else if(current_ptr->arrivalno==-1&&current_ptr&&current_ptr->ref_bit==0&&current_ptr->validity==0)
											{
												LRUlocal(current_process,current_reference,current_ptr,limit[current_process][1],1);
											}
										}
										break;

								case 3:
										while(fscanf(input_file2,"%d %d",&trace_list[0],&trace_list[1])!=EOF)
										{
											references++;
											int current_process=trace_list[0];
											int current_reference=trace_list[1];
											if(current_reference>=limit[current_process][1])
											{
												fprintf(out_file,"Process %d is misbehaving, Suspended...\n",current_process);
												continue;
											}
											struct pagetable *current_ptr;
											current_ptr=pageprocess[current_process];
											while(current_ptr&&current_ptr->pageno!=current_reference)
											{
												current_ptr=current_ptr->next;
											}
											if(current_ptr&&current_ptr->validity==1&&current_ptr->ref_bit==0)
											{
												fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
												current_ptr->ref_bit=1;
												//current_ptr->arrivalno=arrivalnumber[current_process];
												//arrivalnumber[current_process]++;
												continue;
											}
											else if(current_ptr&&current_ptr->validity==1&&current_ptr->ref_bit==1)
											{
												fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
												current_ptr->ref_bit=1;
												//current_ptr->arrivalno=arrivalnumber[current_process];
												//arrivalnumber[current_process]++;
												continue;
											}
											else if(current_ptr&&current_ptr->validity==0)
											{
												ClockLocal(current_process,current_reference,current_ptr,limit[current_process][1],1);
											}
										}
										break;

								default: 
										printf("wrong input\n");
										break;
							}
							break;

							case 2:
									switch(option2)
									{
										case 1:
											while(fscanf(input_file2,"%d %d",&trace_list[0],&trace_list[1])!=EOF)
											{
												references++;
												 //FIFO
														int current_process=trace_list[0];
															int current_reference=trace_list[1];
													if(current_reference>=limit[current_process][1])
													{
														fprintf(out_file,"Process %d misbehaving, Suspended...\n",current_process);
														continue;
													}
													struct pagetable *current_ptr;
													current_ptr=pageprocess[current_process];
													while(current_ptr&&current_ptr->pageno!=current_reference)
													{
														current_ptr=current_ptr->next;
													}
													if(current_ptr&&current_ptr->validity==1)
													{
														current_ptr->ref_bit=1;
														fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
														continue;
													}
													else if(current_ptr)
													{
														FIFOglobal(current_process,current_reference,current_ptr,limit[current_process][1],1);
														//FIFOlocal(current_process,current_reference,current_ptr);
													}
											}
												break;
										case 2: //LRU
													while(fscanf(input_file2,"%d %d",&trace_list[0],&trace_list[1])!=EOF)
													{
														references++;
											
														int current_process=trace_list[0];
														int current_reference=trace_list[1];
														if(current_reference>=limit[current_process][1])
														{
															fprintf(out_file,"Process %d misbehaving, Suspended...\n",current_process);
															continue;
														}
														struct pagetable *current_ptr;
														current_ptr=pageprocess[current_process];
														while(current_ptr&&current_ptr->pageno!=current_reference)
														{
															current_ptr=current_ptr->next;
														}
														if(current_ptr->validity==1&&current_ptr)
														{
															current_ptr->ref_bit=1;
															fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
															current_ptr->arrivalno=globalarrivalno;
															globalarrivalno++;
															continue;
														}
														else if(current_ptr)
														{
															LRUglobal(current_process,current_reference,current_ptr,limit[current_process][1],1);
														}
													}
										
												break;
										case 3: //clock
												while(fscanf(input_file2,"%d %d",&trace_list[0],&trace_list[1])!=EOF)
												{
													references++;
													int current_process=trace_list[0];
													int current_reference=trace_list[1];
													if(current_reference>=limit[current_process][1])
													{
														fprintf(out_file,"Process %d is misbehaving, Suspended...\n",current_process);
														continue;
													}
													struct pagetable *current_ptr;
													current_ptr=pageprocess[current_process];
													while(current_ptr&&current_ptr->pageno!=current_reference)
													{
														current_ptr=current_ptr->next;
													}
													if(current_ptr&&current_ptr->validity==1&&current_ptr->ref_bit==0)
													{
														fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
														current_ptr->ref_bit=1;
														//current_ptr->arrivalno=arrivalnumber[current_process];
														//arrivalnumber[current_process]++;
														continue;
													}
													else if(current_ptr&&current_ptr->validity==1&&current_ptr->ref_bit==1)
													{
														fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
														current_ptr->ref_bit=1;
														//current_ptr->arrivalno=arrivalnumber[current_process];
														//arrivalnumber[current_process]++;
														continue;
													}
													else if(current_ptr&&current_ptr->validity==0)
													{
														ClockGlobal(current_process,current_reference,current_ptr,limit[current_process][1],1);
													}
												}
												
												break;
										default:
												printf("Invalid input\n");
												break;
									}
									//printf("Global page replacement\n");
									break;

							default:
							        printf("You have entered the wrong option\n");
							        break;

				}
		case 0:
					switch(option1)
				{
					case 1:
							switch(option2)
							{
								case 1:
									while(fscanf(input_file2,"%d %d",&trace_list[0],&trace_list[1])!=EOF)
									{	
									references++;									
													int current_process=trace_list[0];
													int current_reference=trace_list[1];
											if(current_reference>=limit[current_process][1])
											{
												fprintf(out_file,"Process %d misbehaving, Suspended...\n",current_process);
												continue;
											}
											struct pagetable *current_ptr;
											current_ptr=pageprocess[current_process];
											while(current_ptr&&current_ptr->pageno!=current_reference)
											{
												current_ptr=current_ptr->next;
											}
											if(current_ptr&&current_ptr->validity==1&&current_ptr->pageno==current_reference)
											{
												current_ptr->ref_bit=1;
												fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
												continue;
											}
											else if(current_ptr&&current_ptr->validity==0)
											{
												FIFOlocal(current_process,current_reference,current_ptr,limit[current_process][1],0);
											}
									}
								break;

								case 2:
										while(fscanf(input_file2,"%d %d",&trace_list[0],&trace_list[1])!=EOF)
										{
											references++;
											int current_process=trace_list[0];
											int current_reference=trace_list[1];
											if(current_reference>=limit[current_process][1])
											{
												fprintf(out_file,"Process %d misbehaving, Suspended...\n",current_process);
												continue;
											}
											struct pagetable *current_ptr;
											current_ptr=pageprocess[current_process];
											while(current_ptr&&current_ptr->pageno!=current_reference)
											{
												current_ptr=current_ptr->next;
											}
											if(current_ptr->validity==1&&current_ptr&&current_ptr->ref_bit==0&&current_ptr->pageno==current_reference&&current_ptr->arrivalno!=-1)
											{
												current_ptr->ref_bit=1;
												fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
												current_ptr->arrivalno=arrivalnumber[current_process];
												arrivalnumber[current_process]++;
												continue;
											}
											else if(current_ptr->validity==1&&current_ptr&&current_ptr->ref_bit==1&&current_ptr->pageno==current_reference&&current_ptr->arrivalno!=-1)
											{
												current_ptr->ref_bit=1;
												fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
												current_ptr->arrivalno=arrivalnumber[current_process];
												arrivalnumber[current_process]++;
												continue;
											}
											else if(current_ptr->arrivalno==-1&&current_ptr&&current_ptr->ref_bit==0&&current_ptr->validity==0)
											{
												LRUlocal(current_process,current_reference,current_ptr,limit[current_process][1],0);
											}
										}
										break;

								case 3:
										while(fscanf(input_file2,"%d %d",&trace_list[0],&trace_list[1])!=EOF)
										{
											references++;
											int current_process=trace_list[0];
											int current_reference=trace_list[1];
											if(current_reference>=limit[current_process][1])
											{
												fprintf(out_file,"Process %d is misbehaving, Suspended...\n",current_process);
												continue;
											}
											struct pagetable *current_ptr;
											current_ptr=pageprocess[current_process];
											while(current_ptr&&current_ptr->pageno!=current_reference)
											{
												current_ptr=current_ptr->next;
											}
											if(current_ptr&&current_ptr->validity==1&&current_ptr->ref_bit==0)
											{
												fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
												current_ptr->ref_bit=1;
												//current_ptr->arrivalno=arrivalnumber[current_process];
												//arrivalnumber[current_process]++;
												continue;
											}
											else if(current_ptr&&current_ptr->validity==1&&current_ptr->ref_bit==1)
											{
												fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
												current_ptr->ref_bit=1;
												//current_ptr->arrivalno=arrivalnumber[current_process];
												//arrivalnumber[current_process]++;
												continue;
											}
											else if(current_ptr&&current_ptr->validity==0)
											{
												ClockLocal(current_process,current_reference,current_ptr,limit[current_process][1],0);
											}
										}
										break;

								default: 
										printf("wrong input\n");
										break;
							}
							break;

							case 2:
									switch(option2)
									{
										case 1:
											while(fscanf(input_file2,"%d %d",&trace_list[0],&trace_list[1])!=EOF)
											{
												references++;
												 //FIFO
														int current_process=trace_list[0];
															int current_reference=trace_list[1];
													if(current_reference>=limit[current_process][1])
													{
														fprintf(out_file,"Process %d misbehaving, Suspended...\n",current_process);
														continue;
													}
													struct pagetable *current_ptr;
													current_ptr=pageprocess[current_process];
													while(current_ptr&&current_ptr->pageno!=current_reference)
													{
														current_ptr=current_ptr->next;
													}
													if(current_ptr&&current_ptr->validity==1)
													{
														current_ptr->ref_bit=1;
														fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
														continue;
													}
													else if(current_ptr)
													{
														FIFOglobal(current_process,current_reference,current_ptr,limit[current_process][1],0);
														//FIFOlocal(current_process,current_reference,current_ptr);
													}
											}
												break;
										case 2: //LRU
													while(fscanf(input_file2,"%d %d",&trace_list[0],&trace_list[1])!=EOF)
													{
														references++;
											
														int current_process=trace_list[0];
														int current_reference=trace_list[1];
														if(current_reference>=limit[current_process][1])
														{
															fprintf(out_file,"Process %d misbehaving, Suspended...\n",current_process);
															continue;
														}
														struct pagetable *current_ptr;
														current_ptr=pageprocess[current_process];
														while(current_ptr&&current_ptr->pageno!=current_reference)
														{
															current_ptr=current_ptr->next;
														}
														if(current_ptr->validity==1&&current_ptr)
														{
															current_ptr->ref_bit=1;
															fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
															current_ptr->arrivalno=globalarrivalno;
															globalarrivalno++;
															continue;
														}
														else if(current_ptr)
														{
															LRUglobal(current_process,current_reference,current_ptr,limit[current_process][1],0);
														}
													}
										
												break;
										case 3: //clock
												while(fscanf(input_file2,"%d %d",&trace_list[0],&trace_list[1])!=EOF)
												{
													references++;
													int current_process=trace_list[0];
													int current_reference=trace_list[1];
													if(current_reference>=limit[current_process][1])
													{
														fprintf(out_file,"Process %d is misbehaving, Suspended...\n",current_process);
														continue;
													}
													struct pagetable *current_ptr;
													current_ptr=pageprocess[current_process];
													while(current_ptr&&current_ptr->pageno!=current_reference)
													{
														current_ptr=current_ptr->next;
													}
													if(current_ptr&&current_ptr->validity==1&&current_ptr->ref_bit==0)
													{
														fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
														current_ptr->ref_bit=1;
														//current_ptr->arrivalno=arrivalnumber[current_process];
														//arrivalnumber[current_process]++;
														continue;
													}
													else if(current_ptr&&current_ptr->validity==1&&current_ptr->ref_bit==1)
													{
														fprintf(out_file,"page %d of process %d is already in memory..\n",current_reference,current_process);
														current_ptr->ref_bit=1;
														//current_ptr->arrivalno=arrivalnumber[current_process];
														//arrivalnumber[current_process]++;
														continue;
													}
													else if(current_ptr&&current_ptr->validity==0)
													{
														ClockGlobal(current_process,current_reference,current_ptr,limit[current_process][1],0);
													}
												}
												
												break;
										default:
												printf("Invalid input\n");
												break;
									}
									//printf("Global page replacement\n");
									break;

							default:
							        printf("You have entered the wrong option\n");
							        break;

				}

				break;
		default:
					printf("You have entered the wrong input\n");
					break;
	}
				//without page replacement;

	printf("Total no of references given=%d\n",references);
	printf("Total number of page faults occured=%d\n",(pagefaultcount));
	//printf("Total number of page faults occured=%d\n",(pagefaultcount/10));
	
}