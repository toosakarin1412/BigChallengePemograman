#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct wordList{
    double skor;
    int jumlah_kata;
    int *panjang_kata;
}wordList;

/*
    Fungsi untuk Mengembalikan nilai scoreCount
    @return : scoreCount
*/
int getScoreCount();

/*
    Fungsi untuk Mengembalikan nilai totalWord
    @return : totalWord
*/
int getTotalWord();

/*
    Fungsi untuk menghilangkan tanda baca (punctuation)
    dari dalam file input dan menuliskan hasil ke dalam file output
    
    @parameter *input : pointer FILE dari file input berupa .txt
    @parameter *output : pointer FILE dari file output berupa .txt
*/
void deletePunctuation(FILE *input, FILE *output);

/*
    Fungsi untuk memecah setiap kata yang ada di dalam file input
    dan menuliskan setiap kata per baris pada file output

    @parameter *input : pointer FILE dari file input berupa .txt
    @parameter *output : pointer FILE dari file output berupa .txt
*/
void parse2word(FILE *input, FILE *output);

/*
    Fungsi untuk menghapus data yang mempunyai duplikat 

    @parameter *input : pointer FILE dari file input berupa .txt
    @parameter *output : pointer FILE dari file output berupa .txt
*/
void eraseDuplicate(FILE *input, FILE *output);

/*
    Fungsi untuk cek kata apakah mempunyai duplikat

    @return : boolean
*/
bool checkWord();

/*
    Fungi untuk mengambil setiap baris pada file _word
    dan memberi setiap kata sebuah nilai dengan menggunakan fungsi
    getScore();

    @parameter *input : pointer FILE dari file input berupa .txt
    @parameter *output : pointer FILE dari file output berupa .txt
*/
void scoreWord(FILE *input, FILE *output);

/*
    Fungsi untuk memberikan sebuah kata nilai dengan aturan pemberian 
    nilai pada permainan scrabble

    @parameter *buffer : pointer dari string kata
    @return : jumlah skor kata
*/
double getScore(char *buffer);

/*
    Fungsi untuk mengecek apakah score sudah pernah ada atau belum
    
    @parameter score : score yang akan di cek
    @return : boolean
*/
bool cekScore(double score);

/*
    Fungi untuk mengurutkan isi hasil dari score yang telah di dapatkan
*/
void sort_scoreList();

/*
    Funsgi untuk memasukkan data kedalam struct dan menuliskan hasil dari data
    kedalam sebuah file binary (.bin)

    @parameter *input : pointer FILE dari file input berupa .txt
    @parameter *output : pointer FILE dari file output berupa .bin
*/
void makeWordList(FILE *input, FILE *output);

/*
    Fungsi untuk bembaca file input yang berupa file binary 
    dan mengeluarkan sebanyak N data

    @parameter *input : pointer FILE dari file input berupa .bin
    @parameter n : banyak data yang akan di tampilkan
*/
void readBinHasil(FILE *input, int n);