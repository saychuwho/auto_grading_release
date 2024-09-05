int main(){
    const char* sentences[]={"you made my day", "You Love me", "Hihh", "hI"}
    int num_sentences = 4;
    char** result = SecretCode(sentences,num_sentences);
    for (int i = 0; i < num_sentences / 2; i++) {
       cout << result[i] << endl;
    }
    return 0;
}
