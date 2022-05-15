void read_config()
{
    struct Config *headPtr = NULL , *ptr ;
    ptr = malloc(sizeof(struct Config));
    char Identify_1 , Identify_2 , Identify_3;
    
    FILE* fptr; 
	if ((fptr = fopen("dict.dic", "r")) == NULL)
		printf("Open dict.dic error!\n");
	else
	{   
       while( r != EOF) 
      {
        if(r != '#')
        {
            if(fscanf( r ,"%s" , Identify_1 ) == "TOTAL_STATION = ")
            {
                fscanf(r, "%d\n", ptr ->TOTAL_STATION );
            }
            if(fscanf( r ,"%s" , Identify_2 ) == "distance = ")
            {
                fscanf(r, "%d\n", ptr ->distance );
            }
            if(fscanf( r ,"%s" , Identify_3 ) == "Strategy strategy = ")
            {
                fscanf(r, "%s\n", ptr ->Strategy strategy );
            }
        }
        else
        {
            fsanf(fp, "%[^\n]\n", buf); 
        }
        
      }  
    }
    
    fclose(fp);
    return 0;
}