#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<pthread.h>

#include<unistd.h>
#include<sys/types.h>
#include<time.h>
#include<sys/wait.h>


#define BUFF_SIZE 100           //Job queue size
#define FIN_QUEUE 100       //finished queue sizeof
#define ARR_RATE 1          //arrival rate of job
typedef struct Process   //job queue and finished queue data type
{
    char name[10];       
    char ela_time[10];
    time_t arrival;     
    time_t startTime;    
    time_t finishTime;    
    double cpuTime;      
    double watTime;  
    int priority;       
}PCB;

typedef struct 
{
    char operation[10];
    char job_name[10];
    char burstTime[10];
    int pri;
    char policy[10];
    int job_num;
    int pri_level;
    int max_cpu;
    int min_cpu;
}command_t;

PCB Job_Queue[BUFF_SIZE];       //size of queue
PCB Fin_Queue[FIN_QUEUE];
int stop=0;                     
int in=0;
int out=0;
int count=0;
int fin_count=0;
int total_job=0;
char sys_policy[10]="fcfs";

//  Initializing condition variables and mutex

pthread_mutex_t queue_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full_cv=PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty_cv=PTHREAD_COND_INITIALIZER;

command_t* command_line_parser(char* input)
{
    command_t* command;
    command=(command_t*) malloc(sizeof(command_t));
    memset(command,0,sizeof(command_t));
    sscanf(input,"%s",command->operation);
    if (strcmp(command->operation,"run")==0)
    {  
        sscanf(input,"%s %s %s %d",command->operation,command->job_name,command->burstTime,&command->pri);
        return command;
    }
    else if (strcmp(command->operation,"help")==0)
    {
        return command;
    }
    else if (strcmp(command->operation,"test")==0)
    {
        sscanf(input,"%s %s %s %d %d %d %d",command->operation,command->job_name,command->policy,&command->job_num,&command->pri_level,&command->min_cpu,&command->max_cpu);
        return command;
    }
    else if (strcmp(command->operation,"list")==0)
    {
        return command;
    }
    else if (strcmp(command->operation,"sjf")==0)
    {
        return command;
    }
    else if (strcmp(command->operation,"fcfs")==0)
    {
        return command;
    }
    else if (strcmp(command->operation,"priority")==0)
    {
        return command;
    }
    else if (strcmp(command->operation,"quit")==0)
    {
        return command;
    }
    else
    {
        strcpy(command->operation,"error");
        return command;
    }
}


//      CODE FOR FCFS SCHEDULING POLICY
void fcfs()
{   pthread_mutex_lock(&queue_mutex);       //accuiring the lock
    PCB temp;
    int wait_job;
    int i,j;
    if(count>0)
    {
        wait_job=count-1;
    }
    else
    {
        wait_job=0;
    } 
    for(i=out+1;i!=in;i++)
    {
        if(i==BUFF_SIZE)
        {
            i=0;
        }
        for(j=i+1;j!=in;j++)
        {
            if(j==BUFF_SIZE)        //Checking the pointer's position
            {
                j=0;
            }
            if(Job_Queue[i].arrival > Job_Queue[j].arrival)  
            {  
                temp = Job_Queue[i];  
                Job_Queue[i] = Job_Queue[j];  
                Job_Queue[j] = temp;  
            }  
        }
    }
    strcpy(sys_policy,"fcfs");
    pthread_mutex_unlock(&queue_mutex);         //Releasing the lock
    
}


//      CODE FOR SJF SCHEDULING POLICY
void sjf()
{   
    pthread_mutex_lock(&queue_mutex);       //accuiring the lock
    PCB temp;
    int wait_job;
    int i,j;
    if(count>0)
    {
        wait_job=count-1;
    }
    else
    {
        wait_job=0;
    } 
    for(i=out+1;i!=in;i++)
    {
        if(i==BUFF_SIZE)        //Checking the pointers position
        {
            i=0;
        }
        for(j=i+1;j!=in;j++)
        {
            if(j==BUFF_SIZE)
            {
                j=0;
            }
            if(atoi(Job_Queue[i].ela_time)>atoi(Job_Queue[j].ela_time))  
            {  
                temp = Job_Queue[i];  
                Job_Queue[i] = Job_Queue[j];  
                Job_Queue[j] = temp;  
            }  
        }
    }
    strcpy(sys_policy,"sjf");
    pthread_mutex_unlock(&queue_mutex);         //releasing the lock
}

//      CODE FOR PRIORITY SCHEDULING POLICY
void priority_sort()
{   
    pthread_mutex_lock(&queue_mutex);       //accquiring the lock
    PCB temp;
    int wait_job;
    int i,j;
    if(count>0)
    {
        wait_job=count-1;
    }
    else
    {
        wait_job=0;
    } 
    for(i=out+1;i!=in;i++)
    {
        if(i==BUFF_SIZE)        //Checking the pointers position
        {
            i=0;
        }
        for(j=i+1;j!=in;j++)
        {
            if(j==BUFF_SIZE)
            {
                j=0;
            }
            if(Job_Queue[i].priority < Job_Queue[j].priority)  
            {  
                temp = Job_Queue[i];  
                Job_Queue[i] = Job_Queue[j];  
                Job_Queue[j] = temp;  
            }  
        }
    }
    strcpy(sys_policy,"priority");
   
    pthread_mutex_unlock(&queue_mutex);     //releasing the lock
}


void list_queue()
{   
    struct tm *ptm;                          /*     For Displaying                  */
	time_t time;                            /*                the arrival time    */

    int i;
    pthread_mutex_lock(&queue_mutex);   //accuiring the lock
    printf("Total number of jobs in the waiting queue:%d\n",count);
    printf("Scheduling Policy:%s\n",sys_policy);
    if(count==0)
    {
        printf("Job Queue is currently empty!\n");

    }
    else
    printf("Name\tCPU_Time\tPriority\tArrival_Time\tStatus\n");
    for(i=out;i!=in;i++)
    {   
        if(i==(BUFF_SIZE))
        {
            i=0;
        }
        ptm = localtime( &time );
		
        printf("%s\t%s\t%10d\t",Job_Queue[i].name,Job_Queue[i].ela_time,Job_Queue[i].priority);
        printf("\t%02d:%02d:%02d\t", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
        //info=gmtime(&Job_Queue[i].arrival);
        
        if(i==out)
        {
            printf("Running.....\n");
        }
        else
        printf("\n");
    }
   
    pthread_mutex_unlock(&queue_mutex);         //releasing the lock
}   

void help()
{
    printf("\nrun <job> <time> <pri>:submit a job named <job>,\n");
    printf("                       execution time is <time>,\n");
    printf("                       priority is <pri>.\n");
    printf("\nlist:display the job status.\n"); 
    printf("\nTo change the current scheduling policy, enter \n");
    printf("<fcfs>: for FCFS\n<sjf>: for SJF &\n<priority>: for priority.\n");
    printf("\ntest <benchmark> <policy> <num_of_jobs> <priority_levels> <min_CPU_time> <max_CPU_time>\n");
    printf("\nquit: exit from AUbatch\n");
}



//CODE FOR SCHEDULER

void *scheduler(void *argv)
{   
    double EstTime=0;
    int i,j;
    time_t now;
    command_t *new_command=(command_t*)argv;
    if(strcmp(new_command->operation,"run")==0)
    {  
        pthread_mutex_lock(&queue_mutex);
        while(count==BUFF_SIZE)
        {
            printf("The Job Queue is full, waiting for dispatcher\n");
            pthread_cond_wait(&not_full_cv,&queue_mutex);
        }
        if(count==0)
        {
            pthread_cond_signal(&not_empty_cv);
        }
        count++;
        strcpy(Job_Queue[in].name,new_command->job_name);
        strcpy(Job_Queue[in].ela_time,new_command->burstTime);
        Job_Queue[in].priority=new_command->pri;
        Job_Queue[in].arrival=time(NULL);
        printf("Job <%s> was submitted.\n ",Job_Queue[in].name);
        printf("Total job in the queue:%d.\n",count);  
        for(i=out;i!=in;i++)
        { 
            if(i==BUFF_SIZE)
             {
                i=0;
             }
            EstTime+=atoi(Job_Queue[i].ela_time);
        }
        in++;
        if(in==BUFF_SIZE)
        {
            in=0;
        }
        pthread_mutex_unlock(&queue_mutex);
        if(strcmp(sys_policy,"sjf")==0)
        {
            sjf();
        }
        else if(strcmp(sys_policy,"priority")==0)
        {
            priority_sort();
        }
        else if(strcmp(sys_policy,"fcfs")==0)
        {
            fcfs();
        }
        printf("Expected waiting time:%.0f\n",EstTime);
    }
    else if(strcmp(new_command->operation,"sjf")==0)
        {
            sjf();
            printf("Switching to SJF Scheduling Policy\n");
        }
    else if(strcmp(new_command->operation,"fcfs")==0)
        {
            fcfs();
            printf("Switching to FCFS Scheduling Policy\n");
        }
    else if(strcmp(new_command->operation,"priority")==0)
        {
            priority_sort();
            printf("Switching to Priority Scheduling Policy\n");
        }
    else if(strcmp(new_command->operation,"test")==0)
        {
            printf("\t\t\tAutomated test module\n");
            pthread_mutex_lock(&queue_mutex);
            fin_count=0;
            total_job=new_command->job_num;
            pthread_mutex_unlock(&queue_mutex);
            
            printf("Test Information:\n\n");
            printf("The benchmark name:                         %s\n",new_command->job_name);
            printf("Policy:                                     %s\n",new_command->policy);
            printf("Number of job:                              %d\n", new_command->job_num);
            printf("priority levels:                            %d\n",new_command->pri_level);
            printf("MIN_CPU time:                               %d\n",new_command->min_cpu);
            printf("MAX_CPU time:                               %d\n",new_command->max_cpu);
            printf("default arrive rate:                        %d No/sec\n\n",ARR_RATE);
        
            

            srand(time(NULL));
            for(j=0;j<new_command->job_num;j++)
            {   
                pthread_mutex_unlock(&queue_mutex);
                strcpy(sys_policy,new_command->policy);
                while(count==BUFF_SIZE)
                {
                    pthread_cond_wait(&not_full_cv,&queue_mutex);
                }
                if(count==0)
                {
                    pthread_cond_signal(&not_empty_cv);
                }
                count++;
                strcpy(Job_Queue[in].name,new_command->job_name);
                sprintf(Job_Queue[in].ela_time,"%d",((rand())%(new_command->max_cpu-new_command->min_cpu)+(new_command->min_cpu)));
                Job_Queue[in].priority=rand()%new_command->pri_level;
                Job_Queue[in].arrival=time(NULL);
                in++;
                if(in==BUFF_SIZE)
                {
                    in=0;
                }
                pthread_mutex_unlock(&queue_mutex);
                if(strcmp(sys_policy,"sjf")==0)
                {
                    sjf();
                }
                else if(strcmp(sys_policy,"priority")==0)
                {
                    priority_sort();
                }
                else if(strcmp(sys_policy,"fcfs")==0)
                {
                    fcfs();
                } 
                pthread_mutex_unlock(&queue_mutex);
                sleep(ARR_RATE);
        
            }
        printf("All %d test jobs have been scheduled, Please wait. You can have a look at progress by typing 'list' command\n",new_command->job_num);
        }
    pthread_exit(NULL);
}

//CODE FOR DISPATCHER
void *dispatcher(void *argv)
{ 
    pid_t pid;
    pid_t temp;
    int i;
    char *Exearg[3];
    
    while(stop==0)
    {
        pthread_mutex_lock(&queue_mutex);       

        //Checking if the queue is empty or full
        while(count==0)
        {
            printf("The queue is empty, waiting for new jobs\n");
            pthread_cond_wait(&not_empty_cv,&queue_mutex);
           
        }
        if(stop==1)
        {
            pthread_mutex_unlock(&queue_mutex);
            break;
        }
        Exearg[0]=Job_Queue[out].name;
        Exearg[1]=Job_Queue[out].ela_time;    
        Exearg[2]=NULL;
        Job_Queue[out].startTime=time(NULL);
        pthread_mutex_unlock(&queue_mutex);
        switch((pid=fork()))
        {
            case -1:
            perror("fork");
            break;
            case 0:     //child process replace
            execv(Exearg[0],Exearg);
            puts("Error during execv()");
            exit(-1);
            break;
            default:  //parent process print 
            break;
        }
        wait(NULL);
        pthread_mutex_lock(&queue_mutex);
        Job_Queue[out].finishTime=time(NULL);
        Job_Queue[out].cpuTime=difftime(Job_Queue[out].finishTime,Job_Queue[out].startTime);
        Job_Queue[out].watTime=difftime(Job_Queue[out].startTime,Job_Queue[out].arrival);
        if(count==BUFF_SIZE)
        {
            pthread_cond_signal(&not_full_cv);
        }
        count--;
        memcpy(Fin_Queue+fin_count,Job_Queue+out,sizeof(PCB));      //moving the finished job to queue
        fin_count++;
        out++;
        if(out==BUFF_SIZE)
        {
            out=0;
        }
        if(fin_count==total_job)
        {   
            float avg_turnarond=0;
            float avg_cpu=0;
            float avg_wait=0;
            float throughput=0;
            for(i=0;i<total_job;i++)
            {
                avg_cpu+=(Fin_Queue[i].cpuTime/total_job);
                avg_wait+=(Fin_Queue[i].watTime/total_job);
            }
            avg_turnarond=avg_cpu+avg_wait;
            printf("\nTotal number of job finished:     %d\n",total_job);
            printf("Average Turn around Time:           %3.3f sec\n",avg_turnarond);
            printf("Average CPU Time:                   %3.3f sec\n",avg_cpu);
            printf("Average waiting Time:               %3.3f sec\n",avg_wait);
            printf("Throughput:                         %3.3f No/sec\n",1/avg_turnarond);

        }
        pthread_mutex_unlock(&queue_mutex);
    }
    pthread_exit(NULL);
}


int main()
{
    int i;
    char c;
    pthread_t tid[2];
    command_t *input_command=NULL;
    char input_buff[100];
    pthread_create(&tid[1],NULL,dispatcher,NULL);       //creating the thread

    printf("********************************************************************************************\n");
    printf("\t\t\tWelcome to AuBatch, created by Sai Teja. \n\t\t\t\t\tversion 1.0.1\n");
    printf("*********************************************************************************************\n");
    printf("To know more about the tool, Type 'help' in the terminal.\n");

    while(1)
    {   
        printf(">");
        scanf("%[^\n]",input_buff);
        c=getchar();
        input_command=command_line_parser(input_buff);
        if(strcmp(input_command->operation,"run")==0)
        {
            pthread_create(&tid[0],NULL,scheduler,(void* )input_command);
            pthread_join(tid[0],NULL); 
        }
        if(strcmp(input_command->operation,"list")==0)          //when user asks for list
        {
            list_queue();
        }
        if(strcmp(input_command->operation,"help")==0)          //when user asks for help
        {
            help();
        }
       if(((strcmp(input_command->operation,"sjf"))&&(strcmp(input_command->operation,"fcfs"))&&(strcmp(input_command->operation,"priority")))==0)
        {    
            pthread_create(&tid[0],NULL,scheduler,(void* )input_command);
            pthread_join(tid[0],NULL); 
        } 
        if(strcmp(input_command->operation,"test")==0)
        {
            pthread_create(&tid[0],NULL,scheduler,(void* )input_command);
            pthread_join(tid[0],NULL); 
        }
        if(strcmp(input_command->operation,"error")==0)
        {
            printf("Unknown command! Type <help> to get more information\n");
        }
        if(strcmp(input_command->operation,"quit")==0)                          //when user quits
        {   
            
            printf("\nShutting down the programm\n");
            stop=1;
            float avg_turnarond=0;
            float avg_cpu=0;
            float avg_wait=0;
            float throughput=0;
          
            for(i=0;i<fin_count;i++)
            {
                avg_cpu+=(Fin_Queue[i].cpuTime/fin_count);
                avg_wait+=(Fin_Queue[i].watTime/fin_count);
            }
            avg_turnarond=avg_cpu+avg_wait;

            //Info after quiting 
            printf("\nTotal number of job finished:   %d\n",fin_count);
            printf("Average Turn around Time:       %3.3f\n",avg_turnarond);
            printf("Average CPU Time:               %3.3f\n",avg_cpu);
            printf("Average waiting Time:           %3.3f\n",avg_wait);
            printf("Throughput:                     %3.3f\n\n",1/avg_turnarond);
            pthread_cond_signal(&not_empty_cv);

            break;

        }
    }
    return 0;
}