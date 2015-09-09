#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

void TulisFile(int* IDRobot, int* JenisPeti, int* Simpul,int N);
void BacaFile(char* NamaFile, int *Modal,int *tipeInput,int *NPetiMerah, int *NPetiKuning, int *NPetiHijau,int *NBot,int *&isiSimpul,int *targetSkor,int *batasWaktu,int *NSimpul, float *&LokasiXSimpul, float *&LokasiYSimpul, float *&LokasiZSimpul,bool *validitas);