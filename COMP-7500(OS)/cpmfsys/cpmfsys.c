/*
  Auburn University - Spring 2023 batch

  Project 4 - cpmfsys file systems.

*/


#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "diskSimulator.h"
#include "cpmfsys.h"

#include <ctype.h>


static bool FreeList[255];					//To make free space on disk


DirStructType *mkDirStruct(int index,uint8_t *e) {

  Extent newEntry;			  //Storing new entries info
  int i = 0;
  for (i=0; i<EXTENT_SIZE; i++) {
    newEntry[i] = *(e + index*EXTENT_SIZE + i);
  }

  DirStructType* d = (DirStructType*)malloc(sizeof(DirStructType));  //Initial DirStruct

  d->status = newEntry[0];
  for (i=0; i<8; i++) {
     if ((d->name[i] = (char) newEntry[i+1]) == ' '){
       break;
     }
  }
  d->name[i] = '\0';
  for (i=0; i<3; i++) {
     if ((d->extension[i] = (char) newEntry[i+9]) == ' '){
       break;
     }       
  }
  d->extension[i] = '\0';
  d->XL = newEntry[12];
  d->BC = newEntry[13];
  d->XH = newEntry[14];
  d->RC = newEntry[15];
  for (i=0; i<BLOCKS_PER_EXTENT; i++){
    d->blocks[i] = newEntry[i+16]; 
  }                                           
  return d;                        
}


// Writing contents to the index of the extent

void writeDirStruct(DirStructType *d, uint8_t index, uint8_t *e){
  Extent newEntry;
  int i;
  newEntry[0] = d->status;
  for (i=0; i<8; i++) {
    if (d->name[i] == '\0'){
      break;
    }
    newEntry[i+1] = (uint8_t) d->name[i];
  }
//      Filling up slots
  while (i<8) {
    newEntry[i+1] = 0;
    i++;
  }
  for (i=0; i<3; i++) {
    if (d->extension[i] == '\0'){
      break;
    }
    newEntry[i+9] = (uint8_t) d->extension[i];
  }

//      Unused slots
  while (i<3) {
    newEntry[i+9] = 0;
    i++;
  }
  newEntry[12] = d->XL;
  newEntry[13] = d->BC;
  newEntry[14] = d->XH;
  newEntry[15] = d->RC;
  for (i=0; i<16; i++){
    newEntry[i+16] = d->blocks[i]; // used disk sectors
  }
// Copying value in extent to memory
  for (i=0; i<EXTENT_SIZE; i++) {
    *(e + ((int) index)*EXTENT_SIZE + i) = newEntry[i];
  }
}

// To free up the space

void makeFreeList(){
  
  uint8_t e[BLOCK_SIZE];
  blockRead(e,0);
  int i,j,index;

//Starting freelist

  for (i=1; i<NUM_BLOCKS; i++){
    FreeList[i] = true;
  }
  FreeList[0] = false;
  for (i=0; i<32; i++){
    if (*(e+i*32) == 0xe5){
      continue;
    }
    else{
      for (j=0; j<16; j++){
        index = (int) *(e+i*32+16+j);
        FreeList[index] = false;
      }
    }
  }
}


// Printing the list
void printFreeList(){
  int i,j;
  //title
  printf("FREE BLOCK LIST: (* means in-use)\n");
  for (i=0; i<16; i++){
    //prefix
    printf("%2x:",i*16);
    for (j=0; j<16; j++){
      if (FreeList[i*16+j] == true){
        //free block
        printf(" .");
      }
      else{
        //block in use
        printf(" *");
      }
    }
    printf("\n");
  }
}


// Finding extent (file dr)

int findExtentWithName(char *name, uint8_t *block0){
  int i;
  DirStructType *d;

//      if name is invalid
  if (checkLegalName(name) == false){
    return -1;
  }


  char *filename = "";          //Name of the file
  char *extname = "";           //It's  extension
  char tempo[9];
  strcpy(tempo,name);
  filename = strtok(tempo,".");

  //  Incase filename is Empty
  if (filename == NULL){
    filename ="";
  }
  else{
    extname = strtok(0,".");
  //  Incase extension is missing

    if (extname == NULL){
      extname ="";
    }
  }

    
  for (i=0; i<32; i++){
    if (*(block0+i*32) == 0xe5){
      continue;
    }
    else{
      d = mkDirStruct(i,block0);

//Find extent
      if ((!strcmp(d->name,filename))&&(!strcmp(d->extension,extname))){
        return i;
      }

    }
  }
//If nothing is found
  return -1;
}

// Checking whether name or extension is too long, name blank, or illegal characters

bool checkLegalName(char *name){
  
  char *filename = "";
  char *extname = "";
  char tempo[64];
  strcpy(tempo,name);
  filename = strtok(tempo,".");
  if (filename == NULL){
    filename ="";
  }
  else{
    extname = strtok(0,".");
    if (extname == NULL){
      extname ="";
    }
  }

//Checking Name length 
  if ((strlen(filename) == 0)||(strlen(filename) > 8)||(strlen(extname) > 3)){
    return false;
  }

//Invalid characters

  strcpy(tempo,filename);
  int i,char_in_check;
  for (i=0;i<strlen(filename);i++){
    char_in_check = (int) tempo[i];
    if (!isalnum(char_in_check)){
      return false;
    }
  }
  return true;
}

//      CpmDir func


void cpmDir(){int index = 0;
  uint8_t block0[BLOCK_SIZE];
  blockRead(block0,0);
  DirStructType *extent;

  printf("\nDIRECTORY LISTING\n");
  for( index = 0; index < 32; index++)
    {

     extent = mkDirStruct(index,block0);

     if( extent->status != 229)
     {   

      int blockCount = 0;

      if(index == 0 ){blockCount++;}

      int j = 0;

      for(j = 0; j < 15; j++)
      {
       
        if (extent->blocks[j] != 0 ) // counts the non empty blocks
        {
          blockCount++;
        }
    
      }
      // formula for calculating length of file
      int size = (blockCount-1)*1024+extent->RC*128+extent->BC;
      printf("%s.%s %d\n", extent->name, extent->extension,size);


     }

    }
}


//    Renaming func


int cpmRename(char *oldName, char * newName){
  char file_name[9];
  char file_extension[4];
  int i = 0;
  bool EOF_name = false;
  int extension_index = 1; 
 uint8_t block0[BLOCK_SIZE];
 blockRead(block0,0);

 DirStructType *extent;
 //Error handling
 int index = 0;
 bool legal_name = true;

 index = findExtentWithName(oldName, block0);
 legal_name = checkLegalName(newName);

 // checking names of file
 if(legal_name == false) 
 {
   return -2;
 }

// if name not found
 if(index < 0) 
 {
    return -1;
 }

 if(index >= 0)
 {


   extent = mkDirStruct(index,block0);

  for (i = 0; i < 9; i++)
  {
    if(EOF_name == true)
    {
        extent->name[i] = '\0';
    }
    else if(newName[i] == '.')
    {
        extent->name[i] = '\0';
        EOF_name = true; 
        extension_index += i; 
    }
    else if(newName[i] == ' ')
    {
        extent->name[i] = '\0'; 
    }
    else
    {
        extent->name[i] = newName[i];
    }
  }
 
  for ( i=0; i <= 3; i++)
  {
    if(newName[i+extension_index] == '.')
    {
        extent->extension[i] = '\0';
    }
    else if(newName[i+extension_index] == ' ')
    {
         extent->extension[i] = '\0';
    }
    else
    {
         extent->extension[i] = newName[extension_index+i];
    }
  }


   writeDirStruct(extent, index, block0);
 
 // writes updates to disk array.
   blockWrite(block0,0); 
 
  }
}

//  Deleting file func

int  cpmDelete(char * name){
  int i,ext_index,block_index;
  uint8_t e[BLOCK_SIZE];
  blockRead(e,0);
  //If name is invalid
  if (checkLegalName(name) == false) {
    return -2;
  }
  //Finding extent by name
  ext_index = findExtentWithName(name,e);
  if (ext_index == -1) {
    return -1;
  }
  DirStructType *d = mkDirStruct(ext_index,e);

  d->status = 0xe5;

  for (i=0;i<16;i++){
    block_index = d->blocks[i];
    if (block_index != 0){
      FreeList[block_index] = true;
    }
  }
  writeDirStruct(d, (uint8_t) ext_index, e);
  blockWrite(e,0);
  return 0;

}


/*  Error code meaning

    0 = Successful completion
   -1 = source file not found
   -2 = invalid  filename
   -3 = filename already exists
   -4 = insufficient disk space

*/