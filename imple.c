#include <ctype.h>
#include "head.h"
#include <float.h>
#include <math.h>

double scoreList[100][2];
int scoreCount = 0;
double totalWord = 0;

int getScoreCount(){
    return scoreCount;
}

int getTotalWord(){
    return totalWord;
}

void deletePunctuation(FILE *input, FILE *output){
    char ch;
    while ((ch = fgetc(input)) != EOF){
        // ispunct adalah fungsi untuk cek apakah char adalah sebuah punctuation (tanda baca)
        // isdigit adalah fugnsi untuk cek apakah char adalah sebuah angka
        if (!(ispunct(ch) || isdigit(ch))){
            fprintf(output, "%c", ch);
        }else{
            fprintf(output, " ");
        }
    }
};

void parse2word(FILE *input, FILE *output){
    char buffer[1024];
    while (fscanf(input, "%s", buffer) != EOF){
        fprintf(output, "%s\n", buffer);
    }
};

void eraseDuplicate(FILE *input, FILE *output){
    char buffer[1024];
    FILE *tmp = fopen("tmp.txt", "w+"); // File temporary untuk proses
    while(fgets(buffer, sizeof(buffer), input)){
        int len = strlen(buffer);

        // Ubah kata menjadi lowercase
        for(int i = 0;i< len;i++){
            buffer[i] = tolower(buffer[i]);
        }
        
        // Cek duplikat kata
        if(checkWord(buffer, tmp) == false){
            fprintf(output, "%s", buffer);
        }
    }
    fclose(tmp); // Tutup file temporary
    remove("tmp.txt"); // Menghapus file temporary
};

bool checkWord(char *kata, FILE *tmp){
    //printf("Kata : %s\n", kata); // Untuk cek proses, biarkan saja
    char buffer[1024];int len;
    fseek(tmp, 0, SEEK_SET);
    while(fgets(buffer, sizeof(buffer), tmp) != NULL){
        if(strcmp(kata, buffer) == 0){
            len = strlen(kata);kata[len-1] = '\0';
            len = strlen(buffer);buffer[len-1] = '\0';
            //printf("Kata : %s...Deleted\n", kata); // Untuk cek proses, biarkan saja
            return true;
        }
    }
    fprintf(tmp, "%s", kata);
    return false;
};

void scoreWord(FILE *input, FILE *output){
    for(int i = 0;i < 100;i++){
        scoreList[i][1] = 0;
    };

    char buffer[1024];double score;int len;
    while (fgets(buffer, sizeof(buffer), input)){
        len = strlen(buffer)-1;
        buffer[len] = '\0';
        score = getScore(buffer);
        if(!cekScore(score)){
            scoreList[scoreCount][0] = score;
            scoreList[scoreCount][1]++;
            // printf("Array : %lf,%lf\n", scoreList[scoreCount][0], scoreList[scoreCount][1]); // Untuk cek proses, biarkan saja
            scoreCount++;
        }else{
            for(int i = 0;i <= scoreCount;i++){
                if(score == scoreList[i][0]){
                    scoreList[i][1]++;
                }
            }
            //  printf("Array : %lf,%lf\n", scoreList[scoreCount][0], scoreList[scoreCount][1]); // Untuk cek proses, biarkan saja
        }
        fprintf(output, "%.0lf,%d,%s\n", score, len, buffer);
    }

    sort_scoreList();

    // Hitung jumlah semua kata
    totalWord = 0;
    for(int i = 0;i < scoreCount;i++){
        //printf("%lf,%lf\n", scoreList[i][0], scoreList[i][1]); // Untuk cek proses, biarkan saja
        totalWord = totalWord + scoreList[i][1];
    };
};

double getScore(char *kata){
    char buffer[1024];
    strcpy(buffer, kata);

    double score = 0;
    //printf("%s\n", buffer); // Untuk cek proses, biarkan saja
    int lenght = strlen(buffer);
    for (int i = 0; i < lenght; i++){
        // Ubah char menjadi huruf kecil
        if(buffer[i] < 91){
            buffer[i] += 32;
        }

        // Beri nilai untuk setiap huruf sesuai dengan aturan pada scrabble game
        if (buffer[i] == 'q' || buffer[i] == 'z'){
            score += 10;
        }else if (buffer[i] == 'j' || buffer[i] == 'x'){
            score += 8;
        }else if (buffer[i] == 'k'){
            score += 5;
        }else if (buffer[i] == 'f' || buffer[i] == 'h' || buffer[i] == 'v' || buffer[i] == 'w' || buffer[i] == 'y'){
            score += 4;
        }else if (buffer[i] == 'b' || buffer[i] == 'c' || buffer[i] == 'm' || buffer[i] == 'p'){
            score += 3;
        }else if (buffer[i] == 'd' || buffer[i] == 'g'){
            score += 2;
        }else{
            score += 1;
        }
    }

    strcpy(kata, buffer);
    return score;
};

bool cekScore(double score){
    for(int i = 0;i < scoreCount;i++){
        // fabs(score - scoreList[i][0]) < DBL_EPSILON
        if(score == scoreList[i][0]){
            //printf("%lf dan %lf Sama\n", score, scoreList[i][0]); // Untuk cek proses, biarkan saja
            return true;
        }
    }
    return false;
}

void sort_scoreList(){
    // Bublesort
    for(int i = 0;i < scoreCount-1;i++){
        for(int j = 0;j < scoreCount - i -1;j++){
            if(scoreList[j][0] < scoreList[j+1][0]){
                //printf("SWAP : %lf,%lf\n", scoreList[j][0], scoreList[j][1]); // Untuk cek proses, biarkan saja
                //printf("DENGAN : %lf,%lf\n", scoreList[j+1][0], scoreList[j+1][1]); // Untuk cek proses, biarkan saja

                double tmp;
                tmp = scoreList[j][0];
                scoreList[j][0] = scoreList[j+1][0];
                scoreList[j+1][0] = tmp;

                tmp = scoreList[j][1];
                scoreList[j][1] = scoreList[j+1][1];
                scoreList[j+1][1] = tmp;
            }
        }
    }
};

void makeWordList(FILE *input, FILE *output){
    char buffer[1024];char *token;
    for(int i = 0;i < scoreCount;i++){
        wordList data;
        data.skor = scoreList[i][0];
        data.jumlah_kata = (int)scoreList[i][1];

        // Alokasi memori pada [panjang_kata] sebanyak jumlah_kata untuk menyimpan
        // setiap [panjang_kata] dari setiap kata yang ada pada skor tersebut
        data.panjang_kata = (int *)calloc(sizeof(int)*(data.jumlah_kata) ,sizeof(int)*(data.jumlah_kata));

        fwrite(&data.skor, sizeof(data.skor), 1, output); // Write [skor] kedalam binary
        fwrite(&data.jumlah_kata, sizeof(data.jumlah_kata), 1, output); // Write [jumlah_kata] kedalam binary

        fseek(input, 0, SEEK_SET); // Kembali ke awal file input
        // Mengambil semua [panjang_kata] pada file input yang memiliki [skor] yang sama
        while(fgets(buffer, sizeof(buffer), input)){
            token = strtok(buffer, ",");
            if(atof(buffer) == scoreList[i][0]){
                token = strtok(NULL, ",");
                *data.panjang_kata = atoi(token);
                data.panjang_kata++;    
            }
        }
        data.panjang_kata-=data.jumlah_kata; // Kembalikan pointer panjang_kata ke awal

        // Mengurutkan panjang kata dengan menggunakan bubblesort
        int tmp;
        for(int j = 0;j < data.jumlah_kata;j++){
            for(int k = 0;k < data.jumlah_kata - j - 1;k++){
                if((*(data.panjang_kata+k)) < (*(data.panjang_kata+k+1))){
                    tmp = *(data.panjang_kata+k);
                    *(data.panjang_kata+k) = *(data.panjang_kata+k+1);
                    *(data.panjang_kata+k+1) = tmp;
                }
            }
        }
        
        //printf("Jml kata : %d\n", data.jumlah_kata); // Untuk cek proses, biarkan saja
        FILE *tmpfl = fopen("tmp.txt", "w+");
        for(int j = 0;j < data.jumlah_kata;j++){
            fseek(input, 0, SEEK_SET);
            while(fgets(buffer, sizeof(buffer), input)){
                token = strtok(buffer, ",");
                if(atof(token) == scoreList[i][0]){
                    token = strtok(NULL, ",");
                    if(atoi(token) == *(data.panjang_kata+j)){
                        token = strtok(NULL, ",");
                        if(checkWord(token, tmpfl) == true){
                            //printf("SKIPPED\n"); // Untuk cek proses, biarkan saja
                            continue;
                        }

                        //printf("COUNT\n"); // Untuk cek proses, biarkan saja
                        token[strlen(token)-1] = '\0';
                        fwrite((data.panjang_kata+j), sizeof(int), 1, output);
                        fwrite(token, sizeof(char), strlen(token), output);                        
                    }else{
                        continue;
                    }
                }else{
                    continue;
                }
                
            }
        }
        remove("tmp.txt");
        free(data.panjang_kata);
        data.panjang_kata = NULL;
    }
};

void readBinHasil(FILE *input, int n){
    long size;int count = 0;
    bool cek = false;

    // Mendapatkan ukuran file
    fseek(input, 0, SEEK_END);
    size = ftell(input);
    rewind(input);

    /*
        Membaca file binary dengan struktur
        [skor]
        [jumlah_kata]
        [
            [panjang_kata]
            [kata]
        ] 
        dibaca sebanyak [jumlah_kata]
    */
    while(true){
        wordList tmpData;
        char *buffer;
        fread(&tmpData.skor, sizeof(tmpData.skor), 1, input); // Baca [skor]
        fread(&tmpData.jumlah_kata, sizeof(tmpData.jumlah_kata), 1, input); // Baca [jumlah_kata]

        // Alokasi memori pada [panjang_kata] sebanyak jumlah_kata untuk menyimpan
        // setiap [panjang_kata] dari setiap kata yang ada pada skor tersebut
        tmpData.panjang_kata = (int *)calloc(sizeof(int)*(tmpData.jumlah_kata) ,sizeof(int)*(tmpData.jumlah_kata));

        /*
            Mengambil 
            [
                [panjang_kata]
                [kata]
            ]
            sebanyak [jumlah_kata]
        */  
        for(int i = 0;i < tmpData.jumlah_kata;i++){
            // Cek jika data yang sudah dibaca susah sebanyak N
            if(count == n){
                cek = true;
                break;
            }
            count++;

            fread(tmpData.panjang_kata+i, sizeof(int), 1, input); // Baca [panjang_kata]

            // Alokasi memori ke dalam buffer sebanyak [panjang_kata] untuk menyimpan kata
            // yang akan dibaca dari dalam file binary
            buffer = (char*)calloc(*(tmpData.panjang_kata+i) ,sizeof(char)*(*(tmpData.panjang_kata+i)));

            fread(buffer, sizeof(char)*(*(tmpData.panjang_kata+i)), 1, input); // Baca [kata]
            printf("%.0lf,%s,%d\n", tmpData.skor, buffer, *(tmpData.panjang_kata+i)); // Output data ke layar
            free(buffer); // Bebaskan memori pada buffer yang sudah dialokasikan agar bisa di alokasikan kembali
            buffer = NULL; // Antisipasi agar tidak terjadi double free 
        }
        free(tmpData.panjang_kata); // Bebaskan memori pada [panjang_kata] yang sudah dialokasikan agar bisa di alokasikan kembali
        tmpData.panjang_kata = NULL; // Antisipasi agar tidak terjadi double free

        // Loop berhenti ketika sudah mencapai ujung file atau sudah membaca N data 
        if(ftell(input) == size || cek == true){
            break;
        }
    }
};
