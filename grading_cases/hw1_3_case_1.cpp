int main(){
    const char* sentences[] = {"would you marry me?","i want to marry you!","hold on .. what?","you are soooooo cute~"};
    int num_sentences = 4;
    char** result = SecretCode(sentences,num_sentences);
    for (int i = 0; i < num_sentences / 2; i++) {
       cout << result[i] << endl;
    }
    return 0;
}
