#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<iostream>

 using namespace std;

int main(int argc, char* argv[])
{

  struct stat s;
  for(int i=1;i<argc;i++)
  {
     int fd = open(argv[i],O_WRONLY|O_CREAT);
     mode_t file_perm;
     fstat(fd,&s);
     cout<<"file name :"<<argv[i]<<endl;
     //cout<<"File type : ";
     printf("File type : ");
     if(S_ISREG(s.st_mode))
      printf("Regular file \n");
      
       //cout<<"Regular file"<<endl;
       
     else if(S_ISDIR(s.st_mode))
         printf("Directory file\n");
       //cout<<"Directory file"<<endl;
       
      else if(S_ISLNK(s.st_mode))
         printf("Symbolic Link\n");
       //cout<<"Symbolic Link"<<endl;
       
      else
         printf("Other\n");
        //cout<<"Other "<<endl;
        
     file_perm=s.st_mode&~S_IFMT;
     printf("User permissions: ");
     //cout<<"User permissions: ";
     if(file_perm && S_IRUSR) 
       printf("Readable ");
     //cout<<"readable,";
     else
       printf("Non Readable ");
     //cout<<"Not readable,";
     
     if(file_perm && S_IWUSR) 
       printf("Writable ");
     //cout<<"writable,";
     else
        printf("Not Writable ");
     //cout<<"Not Writable,";
     
     if(file_perm && S_IXUSR) 
        printf("Executable\n");
     //cout<<"executable.\n";
     else
       printf("not Executable\n");
     //cout<<"Not executable.\n\n";
     
     
     
     //cout<<"Group permissions: ";
     printf("Group permissions :");
     if(file_perm && S_IRGRP) 
       printf("Readable ");
     //cout<<"readable,";
     else
       printf("Non Readable ");
     //cout<<"Not readable,";
     
     if(file_perm && S_IWGRP) 
       printf("Writable ");
    // cout<<"writable,";
     else
       printf("Not Writable ");
     //cout<<"Not Writable,";
     
     if(file_perm && S_IXGRP) 
     printf("Executable\n");
    // cout<<"executable.\n";
     else
     printf("not Executable\n");
     //cout<<"Not executable.\n";
     
     //cout<<"Other: ";
     printf("Other :");
     if(file_perm && S_IROTH) 
       printf("Readable ");
     //cout<<"readable,";
     else
       printf("Non Readable ");
     //cout<<"Not readable,";
     
     if(file_perm && S_IWOTH) 
       printf("Writable ");
     //cout<<"writable,";
     else
     printf("Not Writable ");
     //cout<<"Not Writable,";
     
     if(file_perm && S_IXOTH) 
     printf("Executable\n");
     //cout<<"executable .\n";
     else
     printf("not Executable\n");
    // cout<<"Not executable.\n";
     
     
       
     cout<<"File Inode number :"<<s.st_ino<<endl;
     cout<<"User ID :"<<s.st_uid<<endl;
     cout<<"Group ID :"<<s.st_gid<<endl;
     cout<<"File size in bytes :"<<s.st_size<<endl;
     
    // cout<<"File type : "<<s.st_mode<<endl;
     cout<<"Number of links : "<<s.st_nlink<<endl;
     cout<<"Time of last access :"<<ctime(&s.st_atime)<<endl;
     cout<<"Time of last modification :"<<ctime(&s.st_mtime)<<endl;
     //cout<<"File type : %s"<<s.st_mode<<endl;
  
      //printf("File type : %s\n",s.st_mode);
      //printf("Number of links : %d \n",s.st_nlink);
      //printf("Time of last access : %d \n",s.st_atime);
     // printf("File type : %s",s.st_mode);
  
  }


}

     
     
     