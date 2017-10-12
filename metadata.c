/**************************************************************************
 * Author  : Bahadir Celik
 * Contact : bahadir.geomatik[at]gmail.com
 *
 **************************************************************************
 *
 * Copyright © 2017 Bahadir Celik <bahadir.geomatik@gmail.com>
 * This work is  free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.
 *
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mtl.h"

char *strtok_r(char *str, const char *delim, char **nextp) {
  /* Farkli platformlarla uyumluluk icin;
   * Charlie Gordon tarafindan public domain strtok_r() fonksiyonu
   * comp.lang.c - 14.09.2007
   * https://groups.google.com/forum/#!msg/comp.lang.c/akY2fEnquSU/hGZkhrvssSoJ
   * Public domain olduguna dair:
   * https://groups.google.com/forum/#!msg/comp.lang.c/akY2fEnquSU/nKvvjzI5e3wJ
   */
  char *ret;
  if (str == NULL) {
    str = *nextp;
  }
  str += strspn(str, delim);
  if (*str == '\0') {
    return NULL;
  }
  ret = str;
  str += strcspn(str, delim);
  if (*str) {
    *str++ = '\0';
  }
  *nextp = str;
  return ret;
}


char *searchline(char *sstring) {
  /*
   * Search lines. and split into token
   * to extract Landsat metadata
   */
  char *sptr;
  char *token;
  token = strtok_r(sstring, "=", &sptr);
  token = strtok_r(NULL, "\" \"", &sptr);
  return token;
}

Metadata *MtlPtr;
void searchmtl(char *metafile){
  /*
   * Search and extract metadata
   * from Landsat metadata file
   */
  
  MtlPtr = malloc(sizeof(Metadata));
  int i=0,j=0;
  char *token;
  char *sptr;
  char line[BUFFSIZE];
  FILE *fp = fopen(metafile, "r");

  while(fgets(line, sizeof(line), fp)){

    if(strstr(line, "DATA_TYPE_L0RP")){
      continue;
    }
    if(strstr(line, DATATYPE)){
      /* Datatype Tanimi (L1T, L1TP, L1G, L1GT, L1GS) */
      MtlPtr->datatype = malloc(10 * sizeof(MtlPtr->datatype));
      strcpy(MtlPtr->datatype, searchline(line));
    }
    if(strstr(line, COLLECTION)){
      /* Collection Tanimi */
      MtlPtr->collection = malloc(sizeof(MtlPtr->collection));
      strcpy(MtlPtr->collection, searchline(line));
    }
    if(strstr(line, SPACECRAFT)){
      /* Uydu Platformmu (Landsat5, Landsat8) */
      MtlPtr->spacecraft_id = malloc(10 * sizeof(MtlPtr->spacecraft_id));
      strcpy(MtlPtr->spacecraft_id, searchline(line));
    }
    if(strstr(line, SENSOR)){
      /* Algilayici (TM, ETM+, OLI) */
      MtlPtr->sensor_id = malloc(10 * sizeof(MtlPtr->sensor_id));
      strcpy(MtlPtr->sensor_id, searchline(line));
    }
    if(strstr(line, WRSPATH))
      /* WRS2 sisteminde goruntu sutun numarasi */
      MtlPtr->wrs_path = atoi(searchline(line));
    if(strstr(line, WRSROW))
      /* WRS2 sisteminde goruntu satir numarasi */
      MtlPtr->wrs_row = atoi(searchline(line));
    if(strstr(line, ACQDATE)){
      /* Goruntu edinim tarihi */
      token = strtok_r(line, "=", &sptr);
      token = strtok_r(NULL, "-", &sptr);
      /* Goruntunun tarihi (yil) */
      MtlPtr->acq_year = atoi(token);
      token = strtok_r(NULL, "-", &sptr);
      /* Goruntunun tarihi (ay) */
      MtlPtr->acq_month = atoi(token);
      token = strtok_r(NULL, "-", &sptr);
      /* Goruntunun tarihi (gun) */
      MtlPtr->acq_day = atoi(token);
      break;
    }
  }
  
  if(strcmp(MtlPtr->spacecraft_id, "LANDSAT_5")==0){
    MtlPtr->file_names = malloc(8 * sizeof(MtlPtr->file_names));
    MtlPtr->rad_max = malloc(7 * sizeof(MtlPtr->rad_max));
    MtlPtr->rad_min = malloc(7 * sizeof(MtlPtr->rad_min));
    MtlPtr->refl_max = malloc(6 * sizeof(MtlPtr->refl_max));
    MtlPtr->refl_min = malloc(6 * sizeof(MtlPtr->refl_min));
    MtlPtr->qcal_max = malloc(7 * sizeof(MtlPtr->qcal_max));
    MtlPtr->qcal_min = malloc(7 * sizeof(MtlPtr->qcal_min));
    MtlPtr->rad_mult = malloc(7 * sizeof(MtlPtr->rad_mult));
    MtlPtr->rad_add = malloc(7 * sizeof(MtlPtr->rad_add));
    MtlPtr->refl_mult = malloc(6 * sizeof(MtlPtr->refl_mult));
    MtlPtr->refl_add = malloc(6 * sizeof(MtlPtr->refl_add));
    MtlPtr->tirconst_k1 = malloc(sizeof(MtlPtr->tirconst_k1));
    MtlPtr->tirconst_k2 = malloc(sizeof(MtlPtr->tirconst_k2));
  }
  if(strcmp(MtlPtr->spacecraft_id, "LANDSAT_8")==0){
    MtlPtr->file_names = malloc(12 * sizeof(MtlPtr->file_names));
    MtlPtr->rad_max = malloc(11 * sizeof(MtlPtr->rad_max));
    MtlPtr->rad_min = malloc(11 * sizeof(MtlPtr->rad_min));
    MtlPtr->refl_max = malloc(9 * sizeof(MtlPtr->refl_max));
    MtlPtr->refl_min = malloc(9 * sizeof(MtlPtr->refl_min));
    MtlPtr->qcal_max = malloc(11 * sizeof(MtlPtr->qcal_max));
    MtlPtr->qcal_min = malloc(11 * sizeof(MtlPtr->qcal_min));
    MtlPtr->rad_mult = malloc(11 * sizeof(MtlPtr->rad_mult));
    MtlPtr->rad_add = malloc(11 * sizeof(MtlPtr->rad_add));
    MtlPtr->refl_mult = malloc(9 * sizeof(MtlPtr->refl_mult));
    MtlPtr->refl_add = malloc(9 * sizeof(MtlPtr->refl_add));
    MtlPtr->tirconst_k1 = malloc(2 * sizeof(MtlPtr->tirconst_k1));
    MtlPtr->tirconst_k2 = malloc(2 * sizeof(MtlPtr->tirconst_k2));
  }
  while(fgets(line, sizeof(line), fp)){
    if(strstr(line, BANDS) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_5")==0 && i<=7){
      /* Metadosyasindan bant adlarini al. */
      token = searchline(line);
      MtlPtr->file_names[i] = malloc(strlen(token)+1);
      strcpy(MtlPtr->file_names[i], token);
      i++;
      if(i>7)
	break;
    }
    if(strstr(line, BANDS) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_8")==0 && i<=11){
      token = searchline(line);
      MtlPtr->file_names[i] = malloc(strlen(token)+1);
      strcpy(MtlPtr->file_names[i], token);
      i++;
      if(i>11)
	break;
    }
  }i=0;
  
  while(fgets(line, sizeof(line), fp)){
      if(strstr(line, SUNAZIMUTH))
	/* Goruntu orta noktasi icin Gunes azimuth
	 * acisi (Derece) 
	 */
	MtlPtr->sun_azimuth = atof(searchline(line));
      if(strstr(line, SUNELEV))
	/* Goruntu orta noktasi icin Gunes yukseklik
	 * acisi (Derece) 
	 */
	MtlPtr->sun_elevation = atof(searchline(line));
      if(strstr(line, ESUNDIST)){
	/* Goruntu tarihindeki Dunya-Gunes
	* arasi astronomik birimde (AU) uzaklik
	* (NASA JPL DE405 efemerisine gore)
	*/
	MtlPtr->esun_distance = atof(searchline(line));
	break;
      }
  }
  
  while(fgets(line, sizeof(line), fp)){
    if(strstr(line, RADMAX) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_5")==0 && i<=6){
      MtlPtr->rad_max[i] = atof(searchline(line));
      i++;
    }
    if(strstr(line, RADMIN) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_5")==0 && j<=6){
      MtlPtr->rad_min[j] = atof(searchline(line));
      j++;
      if(j>6)
	break;
    }
    if(strstr(line, RADMAX) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_8")==0 && i<=10){
      MtlPtr->rad_max[i] = atof(searchline(line));
      i++;
    }
    if(strstr(line, RADMIN) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_8")==0 && j<=10){
      MtlPtr->rad_min[j] = atof(searchline(line));
      j++;
      if(j>10)
	break;
    }
  }
  i=0,j=0;
  
  while(fgets(line, sizeof(line), fp)){
    if(strstr(line, REFLMAX) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_5")==0 && i<=5){
      MtlPtr->refl_max[i] = atof(searchline(line));
      i++;
    }
    if(strstr(line, REFLMIN) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_5")==0 && j<=5){
      MtlPtr->refl_min[j] = atof(searchline(line));
      j++;
      if(j>5)
	break;
    }
    if(strstr(line, REFLMAX) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_8")==0 && i<=8){
      MtlPtr->refl_max[i] = atof(searchline(line));
      i++;
    }
    if(strstr(line, REFLMIN) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_8")==0 && j<=8){
      MtlPtr->refl_min[j] = atof(searchline(line));
      j++;
      if(j>8)
	break;
    }
  }
  i=0,j=0;
  while(fgets(line, sizeof(line), fp)){
    if(strstr(line, QCALMAX) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_5")==0 && i<=6){
      MtlPtr->qcal_max[i] = atol(searchline(line));
      i++;
    }
    if(strstr(line, QCALMIN) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_5")==0 && j<=6){
      MtlPtr->qcal_min[j] = atol(searchline(line));
      j++;
      if(j>6)
	break;
    }
    if(strstr(line, QCALMAX) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_8")==0 && i<=10){
      MtlPtr->qcal_max[i] = atol(searchline(line));
      i++;
    }
    if(strstr(line, QCALMIN) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_8")==0 && j<=10){
      MtlPtr->qcal_min[j] = atol(searchline(line));
      j++;
      if(j>10)
	break;
    }
  }
  i=0,j=0;

  while(fgets(line, sizeof(line), fp)){
    if(strstr(line, RADMULT) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_5")==0 && i<=6){
      MtlPtr->rad_mult[i] = atof(searchline(line));
      i++;
    }
    if(strstr(line, RADADD) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_5")==0 && j<=6){
      MtlPtr->rad_add[j] = atof(searchline(line));
      j++;
      if(j>6)
	break;
    }
    if(strstr(line, RADMULT) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_8")==0 && i<=10){
      MtlPtr->rad_mult[i] = atof(searchline(line));
      i++;
    }
    if(strstr(line, RADADD) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_8")==0 && j<=10){
      MtlPtr->rad_add[j] = atof(searchline(line));
      j++;
      if(j>10)
	break;
    }
  }
  i=0,j=0;
  while(fgets(line, sizeof(line), fp)){
    if(strstr(line, REFLMULT) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_5")==0 && i<=5){
      MtlPtr->refl_mult[i] = atof(searchline(line));
      i++;
    }
    if(strstr(line, REFLADD) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_5")==0 && j<=5){
      MtlPtr->refl_add[j] = atof(searchline(line));
      j++;
      if(j>5)
	break;
    }
    if(strstr(line, REFLMULT) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_8")==0 && i<=8){
      MtlPtr->refl_mult[i] = atof(searchline(line));
      i++;
    }
    if(strstr(line, REFLADD) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_8")==0 && j<=8){
      MtlPtr->refl_add[j] = atof(searchline(line));
      j++;
      if(j>8)
	break;
    }
  }
  i=0,j=0;
  while(fgets(line, sizeof(line), fp)){
    if(strstr(line, TIRCONSTK1) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_5")==0){
      MtlPtr->tirconst_k1[0] = atof(searchline(line));
    }
    if(strstr(line, TIRCONSTK2) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_5")==0){
      MtlPtr->tirconst_k2[0] = atof(searchline(line));
    }
    if(strstr(line, TIRCONSTK1) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_8")==0 && j<=1){
      MtlPtr->tirconst_k1[i] = atof(searchline(line));
      i++;
    }
    if(strstr(line, TIRCONSTK2) &&
       strcmp(MtlPtr->spacecraft_id, "LANDSAT_8")==0 && j<=1){
      MtlPtr->tirconst_k2[j] = atof(searchline(line));
      j++;
      if(j>1)
	break;
    }
  }
  fclose(fp);
}
void freeMtl(void){
  free(MtlPtr->spacecraft_id);
  free(MtlPtr->datatype);
  free(MtlPtr->collection);
  free(MtlPtr->sensor_id);
  free(MtlPtr->file_names);
  free(MtlPtr->rad_max);
  free(MtlPtr->rad_min);
  free(MtlPtr->refl_max);
  free(MtlPtr->refl_min);
  free(MtlPtr->qcal_max);
  free(MtlPtr->qcal_min);
  free(MtlPtr->rad_mult);
  free(MtlPtr->rad_add);
  free(MtlPtr->refl_mult);
  free(MtlPtr->refl_add);
  free(MtlPtr->tirconst_k1);
  free(MtlPtr->tirconst_k2);
  free(MtlPtr);
}
