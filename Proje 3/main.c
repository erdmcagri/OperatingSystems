  #include <stdio.h>
  #include <stdlib.h>
  #include <pthread.h>
  #include <semaphore.h>

  pthread_mutex_t lockAmp;
  pthread_mutex_t lockDolar;
  pthread_mutex_t lockBirlesim;
  pthread_mutex_t lockMakine;

  int amp = 0;
  int dolar = 0;
  int sayac = 1;

  void makina();
  void makinaAmp();
  void makinaDolar();
  void makinaBirlestir();


  int main(int argc, char **argv){

    pthread_t mAmp, mDolar, mBirlestir;
    void *retval;


    if(pthread_mutex_init(&lockAmp,NULL) != 0) {
      printf("mutex hatası\n");
      return 1;
    }
    if(pthread_mutex_init(&lockDolar,NULL) != 0) {
      printf("mutex hatası\n");
      return 1;
    }
    if(pthread_mutex_init(&lockMakine,NULL) != 0) {
      printf("mutex hatası\n");
      return 1;
    }
    if(pthread_mutex_init(&lockBirlesim,NULL) != 0) {
      printf("mutex hatası\n");
      return 1;
    }

    pthread_create(&mAmp, NULL, makinaAmp, NULL);
    pthread_create(&mDolar, NULL, makinaDolar, NULL);
    pthread_create(&mBirlestir, NULL, makinaBirlestir, NULL);
    pthread_join(mAmp, &retval);
    pthread_join(mDolar, &retval);
    pthread_join(mBirlestir, &retval);

  return 0;
  }

  void makina(){
      pthread_mutex_lock(&lockMakine);
  }

  void makinaAmp(){
    while (1) {
      makina();
      sleep(3);
      amp = 1;
      pthread_mutex_unlock(&lockMakine);
      pthread_mutex_unlock(&lockBirlesim);
      printf("&\n");
      pthread_mutex_lock(&lockAmp);
    }
  }

  void makinaDolar(){
   while (1) {
    makina();
    sleep(5);
    dolar = 1;
    pthread_mutex_unlock(&lockMakine);
    pthread_mutex_unlock(&lockBirlesim);
    printf("$\n");
    pthread_mutex_lock(&lockDolar);
   }
  }


  void makinaBirlestir(){
    char ilk,iki;
    while(1){
      if (amp == 1 || dolar == 1) { // hangisi daha önce üretildi onu kontrol ediyor.
        if (dolar == 1) {
          ilk = '$';
          iki = '&';
          pthread_mutex_lock(&lockBirlesim);
        }
        else if (amp == 1) {
          ilk = '&';
          iki = '$';
          pthread_mutex_lock(&lockBirlesim);
        }
      }
      if (amp == 1 && dolar == 1) {
        sleep(4);
        printf("%d_%c%c\n", sayac,ilk,iki);
        pthread_mutex_unlock(&lockAmp);
        pthread_mutex_unlock(&lockDolar);
        amp = 0;
        dolar = 0;
        sayac++;
  }
  }
  }
