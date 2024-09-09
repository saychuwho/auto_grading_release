#include <iostream>
#include <cstring>

using namespace std;


char** SecretCode(const char* sentences[], int num_sentences){
    char** result = new char*[num_sentences/2];
    for(int i = 0; i < num_sentences/2; i++){
        if(strlen(sentences[2*i]) > strlen(sentences[2*i+1])){
            result[i] = new char[strlen(sentences[2*i]) + 1];
            for(int j = 0; j < strlen(sentences[2*i+1]); j++){
                if(sentences[2*i][j] > sentences[2*i+1][j])
                    result[i][j] = sentences[2*i][j];
                else    
                    result[i][j] = sentences[2*i + 1][j]; 
            }
            for(int k = strlen(sentences[2*i+1]); k < strlen(sentences[2*i]); k++){
                result[i][k] = sentences[2*i][k];
            }
            result[i][strlen(sentences[2*i])] = '\0';
        }
        else{
            result[i] = new char[strlen(sentences[2*i+1]) + 1]; 
            for(int j = 0; j < strlen(sentences[2*i]); j++){
                if(sentences[2*i][j] > sentences[2*i+1][j])
                    result[i][j] = sentences[2*i][j];
                else    
                    result[i][j] = sentences[2*i + 1][j];
            }
            for(int k = strlen(sentences[2*i]); k < strlen(sentences[2*i+1]); k++){
                result[i][k] = sentences[2*i+1][k];
            }
            result[i][strlen(sentences[2*i+1])] = '\0';
        }
    }
    return result;
}


int main(){
    const char* sentences[] = {"would you marry me?","i want to marry you!","hold on .. what?","you are soooooo cute~"};
    int num_sentences = 4;
    char** result = SecretCode(sentences,num_sentences);
    for (int i = 0; i < num_sentences / 2; i++) {
       cout << result[i] << endl;
    }
    return 0;
}
