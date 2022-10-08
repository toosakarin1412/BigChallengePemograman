#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "head.h"

int main(){
    char namafile[255];char nfile[255];

    // Input nama file
    printf("Masukkan nama file input (txt): ");scanf(" %[^\n]s", namafile);

    // Hilangkan substring ".txt" pada input argumen
    int len = strlen(namafile);
    strncpy(nfile, namafile, len-4);
    nfile[len-4] = '\0';

    // Pointer file input dan output
    FILE *input;FILE *output;

    // Menu loop
    int pilihan;
    while(true){
        printf("====================\n");
        printf("1.Proses File input\n2.Tampilkan data\n3.Selesai\n");
        printf("\nPilihan anda : ");scanf(" %d", &pilihan);

        if(pilihan == 1){
            // Menghapus tanda baca dan angka pada file input
            // Dan hasil dimasukkan pada file dengan nama _nopunct
            strcpy(namafile, nfile);strcat(namafile, ".txt");
            input = fopen(namafile, "r");

            // Cek apakah file input tidak ditemukan
            if(input == NULL){
                printf("File tidak ditemukan...\n");
                return EXIT_FAILURE;
            }

            strcpy(namafile, nfile);strcat(namafile, "_nopunct.txt");
            output = fopen(namafile, "w");
            deletePunctuation(input, output);
            fclose(input);fclose(output);

            // Menjadikan semua kata pada file _nopunct menjadi list kata
            // dan dimasukkan kedalam file dengan nama _rawword
            strcpy(namafile, nfile);strcat(namafile, "_nopunct.txt");
            input = fopen(namafile, "r");
            strcpy(namafile, nfile);strcat(namafile, "_rawword.txt");
            output = fopen(namafile, "w+");
            parse2word(input, output);
            fclose(input);fclose(output);

            // Menghapus menjadikan setiap kata menjadi huruf kecil
            // dan menhapus kata-kata yang memiliki duplikat/berulang
            // kemudian hasil disimpan pada file dengan nama _word
            strcpy(namafile, nfile);strcat(namafile, "_rawword.txt");
            input = fopen(namafile, "r+");
            strcpy(namafile, nfile);strcat(namafile, "_word.txt");
            output = fopen(namafile, "w");
            eraseDuplicate(input, output);
            fclose(input);fclose(output);

            // Memberi setiap kata score dan panjang kata kemudian
            // hasil akan di simpan pada file dengan nama _score
            strcpy(namafile, nfile);strcat(namafile, "_word.txt");
            input = fopen(namafile, "r+");
            strcpy(namafile, nfile);strcat(namafile, "_score.txt");
            output = fopen(namafile, "w+");
            scoreWord(input, output);
            fclose(input);fclose(output);

            // Membuat list kata dan dengan di urutkan berdasarkan skor
            // dan panjang kata kemudian di simpan kedalam file _hasil
            strcpy(namafile, nfile);strcat(namafile, "_score.txt");
            input = fopen(namafile, "r");
            strcpy(namafile, nfile);strcat(namafile, "_hasil.bin");
            output = fopen(namafile, "wb");
            makeWordList(input, output);

            fclose(input);
            fclose(output);

            printf("\nProses Selesai . . .");
            printf("\nTotal kata : %d kata\n", getTotalWord());
            
        }else if(pilihan == 2){
            strcpy(namafile, nfile);strcat(namafile, "_hasil.bin"); // Membuka file _hasil
            input = fopen(namafile, "rb");
            if(input == NULL){
                printf("\nData belum di proses, silahkan pilih pilihan 1 untuk proses file input\n");
                continue;
            }

            // Baca isi dari file binary yang sudah di isi dengan data kata yang sudah terurut
            int jmlData;
            printf("Berapa banyak data yang ingin anda tampilkan : ");scanf(" %d", &jmlData);
            printf("====================\n");
            readBinHasil(input, jmlData);
            fclose(input);
        }else if(pilihan == 3){
            printf("\nProgram selesai . . .\n");
            return EXIT_SUCCESS;
        }else{
            printf("Silahkan pilih sesuai dengan angka diatas\n");
        }
    }        
    return 0;
}