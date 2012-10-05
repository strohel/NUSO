#include <sys/time.h>
#include <sys/resource.h>


/**********************************************************************
 * Funkce second vraci hodnotu user time v sekundach.
 * 
 * Tato verze  funguje na linuxu (testovano s jadrem 2.2-2.4) 
 * s rozlisenim 0.01s.
 **********************************************************************/
double second()
{
  double q;
  struct rusage rusage;

  getrusage(RUSAGE_SELF,&rusage);
  q = (double)(rusage.ru_utime.tv_sec);
  q += (double)(rusage.ru_utime.tv_usec) * 1.0e-06;

  return(q);
}

// "Aliasy" funkce second pro kompatibilitu s ruznymi prekladaci FORTRANu
double second_() {
  return second();
}

double SECOND() {
  return second();
}



