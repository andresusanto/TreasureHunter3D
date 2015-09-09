#include "DXUT.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "fExternal.h"

using namespace std;

void TulisFile(int* IDRobot, int* JenisPeti, int* Simpul,int N)
{
	ofstream fout("output.txt");
	int i;
	for (i=0;i<N;i++){
		fout<< "Robot (" << IDRobot[i] << ") ";
		fout<< " Simpul (" << Simpul[i]<<") ";
		switch (JenisPeti[i])
		{
			case 0 : fout << "gagal ngambil peti";
					 break;
			case 1 : fout << "ngambil peti merah";
					 break;
			case 2 : fout << "ngambil peti kuning";
					 break;
			case 3 : fout << "ngambil peti hijau";
					 break;
		}
		fout << " gan." << endl;
	}
	fout.close();
}

void BacaFile(char* NamaFile, int *Modal,int *tipeInput,int *NPetiMerah, int *NPetiKuning, int *NPetiHijau,int *NBot,int *&isiSimpul,int *targetSkor,int *batasWaktu,int *NSimpul, float *&LokasiXSimpul, float *&LokasiYSimpul, float *&LokasiZSimpul,bool *validitas) {
    ifstream inputFile;
    inputFile.open(NamaFile);
    string INSTR;
    bool valid;
    int counter;
    int i,j;

    inputFile.is_open(); // open file

    counter = 0;
    valid = true;
    while (!inputFile.eof())
    {
        inputFile >> INSTR;
        stringstream ss(INSTR);
        if (valid)
        {
            if (counter <= 4 )
            {
                switch(counter)
                {
                    case 0 : if (INSTR == "[Modal]")
                                counter++;
                             else
                                valid = false;
                             break;
                    case 1 : ss >> (*Modal);
                             if (ss)
                                counter++;
                             else
                                valid = false;
                             break;
                    case 2 : if (INSTR == "[Simpul]")
                                counter++;
                             else
                                valid = false;
                             break;
                    case 3 : ss >> (*NSimpul);
                             if (ss)
                             {
                                isiSimpul = new int[(*NSimpul)+1];
                                LokasiXSimpul = new float[(*NSimpul)+1];
                                LokasiYSimpul = new float[(*NSimpul)+1];
                                LokasiZSimpul = new float[(*NSimpul)+1];
                                counter++;
                                for (i=1;i<=(*NSimpul);i++)
                                {
                                    isiSimpul[i] = 0; // inisialisasi simpul masi kosong
                                }
                             }
                             else
                                valid = false;
                             break;
                    case 4 : if (INSTR == "[Given]")
                             {
                                (*tipeInput) = 1;
                                counter++;
                             }
                             else if (INSTR == "[Random]")
                             {
                                (*tipeInput) = 2;
                                counter++;
                             }
                             else
                                valid = false;
                             break;
                }
            }
            else // tipeInput telah dipilih
            {
                if (*tipeInput == 1) // Given
                {
                    switch (counter)
                    {
                        case 5 : if (INSTR == "[Jumlah")
                                    counter++;
                                 else
                                    valid = false;
                                 break;
                        case 6 : if (INSTR == "Peti,Jumlah")
                                    counter++;
                                 else
                                    valid = false;
                                 break;
                        case 7 : if (INSTR=="Bot,MKH,Pemain,Bot]")
                                    counter++;
                                 else
                                    valid = false;
                                 break;
                        case 8 : ss >> (*NPetiMerah);
                                 if ((*NPetiMerah) >= 0)
                                    counter++;
                                 else
                                    valid = false;
                                 break;
                        case 9 : ss >> (*NPetiKuning);
                                 if ((*NPetiKuning) >= 0)
                                    counter++;
                                 else
                                    valid = false;
                                 break;
                        case 10 : ss >> (*NPetiHijau);
                                  if ((*NPetiHijau) >= 0)
                                     counter++;
                                  else
                                     valid = false;
                                  break;
                        case 11 : ss >> (*NBot);
                                  if ((*NBot) > 0)
                                    counter++;
                                  else
                                    valid = false;
                                  valid = (((*NPetiMerah) + (*NPetiKuning) + (*NPetiHijau)+ 1 + (*NBot)) == (*NSimpul));
                                  break;
                        case 12 : if ((*NPetiMerah) > 0)
                                  {
                                    ss >> i;
                                    isiSimpul[i] = 1;
                                    for (i=1;i<(*NPetiMerah);i++)
                                    {
                                        inputFile >> INSTR;
										stringstream ss(INSTR);
                                        ss >> j;
                                        isiSimpul[j] = 1;
                                    }
                                    counter++;
                                  }
                                  else if (*NPetiMerah == 0)
                                    counter++;
                                  else
                                    valid = false;
                                  break;
                        case 13 : if ((*NPetiKuning) > 0)
                                  {
                                    ss >> i;
                                    isiSimpul[i] = 2;
                                    for (i=1;i<(*NPetiKuning);i++)
                                    {
                                        inputFile >> INSTR;
                                        stringstream ss(INSTR);
                                        ss >> j;
                                        isiSimpul[j] = 2;
                                    }
                                    counter++;
                                  }
                                  else if ((*NPetiKuning) == 0)
                                    counter++;
                                  else
                                    valid = false;
                                  break;
                        case 14 : if ((*NPetiHijau) > 0)
                                  {
                                    ss >> i;
                                    isiSimpul[i] = 3;
                                    for (i=1;i<(*NPetiHijau);i++)
                                    {
                                        inputFile >> INSTR;
                                        stringstream ss(INSTR);
                                        ss >> j;
                                        isiSimpul[j] = 3;
                                    }
                                    counter++;
                                  }
                                  else if ((*NPetiHijau) == 0)
                                    counter++;
                                  else
                                    valid = false;
                                  break;
                        case 15 : ss >> i;
                                  isiSimpul[i] = 4; // letak orang
                                  counter++;
                                  break;
                        case 16 : ss >> i;
                                  isiSimpul[i] = 5; // bot
                                  for (i=1;i<(*NBot);i++)
                                  {
                                      inputFile >> INSTR;
                                      stringstream sb(INSTR);
                                      sb >> j;
                                      isiSimpul[j] = 5;
                                  }

                                  counter++;
                                  break;
                        case 17 : if (INSTR == "[Skor]")
                                    counter++;
                                  else
                                    valid = false;
                                  break;
                        case 18 : ss >> (*targetSkor);
                                  if (*targetSkor > 0)
                                    counter ++;
                                  else
                                    valid = false;
                                  break;
                        case 19 : if (INSTR == "[Waktu]")
                                    counter++;
                                  else
                                    valid = false;
                                  break;
                        case 20 : ss >> (*batasWaktu);
                                  if (*batasWaktu > 0)
                                    counter ++;
                                  else
                                    valid = false;
                                  break;
                        case 21 : if (INSTR == "[Posisi")
                                  {
                                    inputFile >> INSTR;
                                    if (INSTR == "Simpul]")
                                    {
                                        counter++;
                                    }
                                    else
                                        valid = false;
                                  }
                                  else
                                    valid = false;
                                  break;
                        case 22 : ss >> LokasiXSimpul[1];

                                  inputFile >> INSTR;
                                  stringstream SS(INSTR);
                                  SS >> LokasiYSimpul[1];

                                  inputFile >> INSTR;
                                  stringstream Ss(INSTR);
                                  Ss >> LokasiZSimpul[1];
                                  for (i=2;i<=(*NSimpul);i++)
                                  {
                                      inputFile >> INSTR;
                                      stringstream sx(INSTR);
                                      sx >> LokasiXSimpul[i];
                                      inputFile >> INSTR;
                                      stringstream sy(INSTR);
                                      sy >> LokasiYSimpul[i];
                                      inputFile >> INSTR;
                                      stringstream sz(INSTR);
                                      sz >> LokasiZSimpul[i];
                                  }
                                  break;
                    }
                }
                else // Random
                {
                    switch(counter)
                    {
                        case 5 : if (INSTR == "[Jumlah")
                                 {
									 
                                     inputFile >> INSTR;
                                     if (INSTR == "Peti,Jumlah")
                                     {
                                         inputFile >> INSTR;
                                         if (INSTR=="Bot]")
                                            counter ++;
                                         else
                                            valid = false;
                                     }
                                     else
                                        valid = false;
                                 }
                                 else
                                    valid = false;
                                 break;
                        case 6 : ss >> (*NPetiMerah);
                                 if ((*NPetiMerah) >= 0)
                                    counter++;
                                 else
                                    valid = false;
                                 break;
                        case 7 : ss >> (*NPetiKuning);
                                 if ((*NPetiKuning) >= 0)
                                    counter++;
                                 else
                                    valid = false;
                                 break;
                        case 8 : ss >> (*NPetiHijau);
                                 if ((*NPetiHijau) >= 0)
                                    counter++;
                                 else
                                    valid = false;
                                 break;
                        case 9 : ss >> (*NBot);
                                 if ((*NBot)>0)
                                    counter++;
                                 else
                                    valid = false;
                                 break;
                        case 10 :  if (INSTR == "[Skor]")
                                    counter++;
                                   else
                                    valid = false;
                                   break;
                        case 11 :  ss >> (*targetSkor);
                                   if (*targetSkor > 0)
                                     counter ++;
                                   else
                                     valid = false;
                                   break;
                        case 12 : if (INSTR == "[Waktu]")
                                    counter++;
                                  else
                                    valid = false;
                                  break;
                        case 13 : ss >> (*batasWaktu);
                                  if (*batasWaktu > 0)
                                    counter ++;
                                  else
                                    valid = false;
                                  break;
                        case 14 : if (INSTR == "[Posisi")
                                  {
                                    inputFile >> INSTR;
                                    if (INSTR == "Simpul]")
                                    {
                                        counter++;
                                    }
                                    else
                                        valid = false;
                                  }
                                  else
                                    valid = false;
                                  break;
                        case 15 : ss >> LokasiXSimpul[1];
                                  inputFile >> INSTR;
                                  stringstream SS(INSTR);
                                  SS >> LokasiYSimpul[1];

                                  inputFile >> INSTR;
                                  stringstream Ss(INSTR);
                                  Ss >> LokasiZSimpul[1];
                                  for (i=2;i<=(*NSimpul);i++)
                                  {
                                      inputFile >> INSTR;
                                      stringstream sx(INSTR);
                                      sx >> LokasiXSimpul[i];
                                      inputFile >> INSTR;
                                      stringstream sy(INSTR);
                                      sy >> LokasiYSimpul[i];
                                      inputFile >> INSTR;
                                      stringstream sz(INSTR);
                                      sz >> LokasiZSimpul[i];
                                  }
                                  for (i=1;i<=(*NSimpul);i++)
                                  {
                                      if ((LokasiXSimpul[i] >=(-5.5f)) && (LokasiXSimpul[i] <= (5.5f)) &&
                                          (LokasiYSimpul[i] >=(-2.5f)) && (LokasiYSimpul[i] <=(2.5f)) &&
                                          (LokasiZSimpul[i] >=(-5.5f)) && (LokasiZSimpul[i] <= (5.5f)))
                                            valid = true;
                                      else {
                                            valid = false;
                                            break;
                                      }
                                  }
                                  break;
                    }
                }
            }
        }
        else
            break;
    }

    inputFile.close();


    if (valid)
    {
        srand((unsigned)time(NULL));
        if (*tipeInput == 2)
        {
           for (i=1;i<=(*NPetiMerah);i++)
           {
               do
               {
                   j = rand() % (*NSimpul) + 1;
               } while (isiSimpul[j] != 0); // random simpul sampe ketemu tempat yang kosong

               isiSimpul[j] = 1;
           }

           for (i=1;i<=(*NPetiKuning);i++)
           {
               do
               {
                   j = rand() % (*NSimpul) + 1;
               } while (isiSimpul[j] != 0); // random simpul sampe ketemu tempat yang kosong

               isiSimpul[j] = 2;
           }

           for (i=1;i<=(*NPetiHijau);i++)
           {
               do
               {
                   j = rand() % (*NSimpul) + 1;
               } while (isiSimpul[j] != 0); // random simpul sampe ketemu tempat yang kosong

               isiSimpul[j] = 3;
           }

           for (i=1;i<=(*NBot);i++)
           {
               do
               {
                   j = rand() % (*NSimpul) + 1;
               } while (isiSimpul[j] != 0); // random simpul sampe ketemu tempat yang kosong

               isiSimpul[j] = 5;
           }

           for (i=1;i<=(*NSimpul);i++)
           {
               if (isiSimpul[i] == 0)
                    isiSimpul[i] = 4; // isi Orang
           }

        }
    }

    *validitas = valid;
}
