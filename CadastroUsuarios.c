#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Usuario.c" 

 
  FILE *fileDB, *fileDBTemp;
  char *fileNameUsers = "Usuario_BD.txt";
  char *fileNameUsersTemp = "Usuario_BDTemp.txt";
  char *modeInsert = "a";
  char *modeReader = "r";
  char *modeOverride = "w";
  const int  maxCharByLine = 200;
  


 void closeDB(char *fileNameDB){
    if(strcmp(fileNameDB,fileNameUsers)==0){    
      fclose(fileDB);     
    }

    if(strcmp(fileNameDB,fileNameUsersTemp)==0){  
        fclose(fileDBTemp); 
    }
 }

//Simula atualização em em banco criando um novo arquivo e substituindo o mesmo 
void updateBD(){
  closeDB(fileNameUsers);//Força o fechamento do arquivo caso ainda nao tenha ocorrido
  closeDB(fileNameUsersTemp);//Força o fechamento do arquivo caso ainda nao tenha ocorrido

   if(remove(fileNameUsers)==0){//Se conseguir Remover o arquivo Original
        if(rename(fileNameUsersTemp,fileNameUsers)==0){//Renomeia o arquivo temporario para ter o mesmo nome do arquivo original
            remove(fileNameUsersTemp);//remove o arquivo temporario
        }else{
          printf("nao foi possivel remover o arquivo %s\n",fileNameUsersTemp);
        }
      }else{
        printf("nao foi possivel remover o arquivo %s\n",fileNameUsers);
      }
}


//Abre conexao com o Arquivo 
bool openBD(char *fileNameDB,char *mode){
   if(strcmp(fileNameUsers,fileNameDB)==0){
        fileDB = fopen(fileNameDB, mode);      
        if(fileDB != NULL)    
            return true;
    }else{
        if(strcmp(fileNameUsersTemp,fileNameDB)==0){
            fileDBTemp = fopen(fileNameDB, mode);      
            if(fileDBTemp != NULL)    
                return true;
            }
    }
    return false;
}



//Escreve no Arquivo
void writeInDB(char *fileNameDB,char *mode,struct Usuario user){ 
   
      openBD(fileNameDB,mode);
      FILE *db; 
      if(strcmp(fileNameDB,fileNameUsers)==0){
          db = fileDB;           
      } 
      if(strcmp(fileNameDB,fileNameUsersTemp)==0){
         db = fileDBTemp;           
      }
      fprintf(db, "%s\t\t\t\t\t%s\t\t\t%s\n", strlwr(user.login),strlwr(user.password),strlwr(user.email));
      closeDB(fileNameDB);   
}  

bool isLoginExists(char login[50]){
     bool isExists = false; 
    if(openBD(fileNameUsers,modeReader)){
     char *word; 
    char line[maxCharByLine];
    
    /**
     * while -> Enquanto ouver o valor nao for igual a NULL
     * fgets -> 1º Parametro variavel que ira armazenar a linha;   2
     * fgets -> 2º Parametro numero maximo de caracters a ser lido; 
     * fgets -> 3º Parametro Ponteiro para arquivo; 
     * */   
    while(fgets(line, maxCharByLine, fileDB) != NULL){
          //Insere o primeiro texto da linha atualseparando por tabulação (\t) na variavel palavra.(POR ISSO PRIMEIRO ARGUMENTO É A LINHA ATUAL)
          word = strtok(line,"\t");
          if(strcmp(word,login)==0){
            isExists = true;
            break;
          }
          
    }
    closeDB(fileNameUsers);
  }else{
    printf("\nNao foi possivel Abrir/Encontrar o arquivo de usuarios.");
  }
     
    
  return isExists;
}



//Insere um novo usuario
void insertNewUser(){
      system("cls");
      system("color 5");
      struct Usuario usuario;
       
      printf("\n\nO arquivo aberto com sucesso!\n");
      printf("\n\nDigite o nome de usuario!\n");   
      scanf("%s",usuario.login);   

      if(isLoginExists(usuario.login)){  
         printf("\nEste usuario ja existe!!!!\n");
         system("pause"); 
      
      }else{
        printf("\n\nDigite a senha do usuario!\n");    
        scanf("%s",usuario.password);   
          
        printf("\n\nDigite o email para o usuario!\n");    
        scanf("%s",usuario.email);   
        writeInDB(fileNameUsers,modeInsert,usuario);
      }
     
            
}      

//Insere os valores de um array em outro
void setUserValues(char insert[50],char value[50]){    
    int i;
    for ( i=0; i < 50; i++)
    {
        insert[i]=value[i];
    }
}

//deleta Usuario escrito no terminal
void deleteSelectedUser(){
  if(openBD(fileNameUsers,modeReader)){     
      system("cls");
      system("color 9");      
      char selectedUser[50];
      char *word; 
      char line[200]; 
      bool isExist = false;
      printf("Digite o nome do usuario que deseja deletar...\n");
      scanf("%s",selectedUser);   
      while(fgets(line, maxCharByLine, fileDB) != NULL){
          
          word = strtok(line,"\t");
         
          
          if(strcmp(word,selectedUser)!=0){  
           
            struct Usuario usuario;
            
            setUserValues(usuario.login,word);        
            setUserValues(usuario.password,strtok(NULL,"\t"));
            setUserValues(usuario.email,strtok(NULL,"\n"));     
                
            writeInDB(fileNameUsersTemp,modeInsert,usuario);           
          }else{
            isExist = true;
          }

      }
     
      
     updateBD();
     if(isExist){
       printf("\n%s excluido com sucesso!\n",selectedUser);
     }else{
        printf("\n%s não foi encontrado!\n",selectedUser);
     }
      
  }
   system("pause");
}

void findAllUsuarios(){
  if(openBD(fileNameUsers,modeReader)){
    system("cls");
    system("color 6");
    char *word; 
    char line[maxCharByLine];
    
    /**
     * while -> Enquanto ouver o valor nao for igual a NULL
     * fgets -> 1º Parametro variavel que ira armazenar a linha;   2
     * fgets -> 2º Parametro numero maximo de caracters a ser lido; 
     * fgets -> 3º Parametro Ponteiro para arquivo; 
     * */   
    while(fgets(line, maxCharByLine, fileDB) != NULL){
          //Insere o primeiro texto da linha atualseparando por tabulação (\t) na variavel palavra.(POR ISSO PRIMEIRO ARGUMENTO É A LINHA ATUAL)
          word = strtok(line,"\t");
        
          printf("LOGIN: \t\t%s\n", word);
          
          //Insere o segundo texto  da linha atual separando por tabulação (\t) na variavel palavra.(POR ISSO PRIMEIRO ARGUMENTO E NULL)
          word =  strtok(NULL,"\t");
          printf("PASSWORD: \t%s\n", word);

          //Insere o terceiro texto  da linha atual separando por tabulação (\t) na variavel palavra.(POR ISSO PRIMEIRO ARGUMENTO E NULL)
          word =  strtok(NULL,"\n");
          printf("EMAIL: %s\n\n", word);
    }
    closeDB(fileNameUsers);
  }else{
    printf("\nNao foi possivel Abrir/Encontrar o arquivo de usuarios.");
  }
  system("pause");
}


int mainCadastroUsuario()
{   
  
  do{   
    system("cls"); 
    system("color 2");
    printf("\n1. Inserir novo Usuario.\n");
    printf("2. Listar Usuarios.\n");
    printf("3. Excluir Usuario.\n");
    printf("0. Sair do Sistema.\n\n");
    int opcao;
    scanf("%i",&opcao); 
    switch(opcao){      
            case 0:{
                return(0);
            }break;  
            case 1:{         
                insertNewUser();
            }break;
            case 2:{        
                findAllUsuarios();  
            }break;
            case 3:{
                deleteSelectedUser();
            }break;
            default:{
                system("cls");
                printf("\n\nOpção Invalida!!!\n\n");
            }break;
    }
  
    }while(true);
  
  
}
