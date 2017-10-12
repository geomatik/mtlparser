/*************************************************************************
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

#ifndef MTL_H_
#define MTL_H_

/* MTL.txt dosyasinda metadata iceren
 * alanlarin tanimlari. (LPGS)  */

#define DATATYPE         "DATA_TYPE"
#define COLLECTION       "COLLECTION_CATEGORY"
#define SPACECRAFT       "SPACECRAFT_ID"
#define SENSOR           "SENSOR_ID"
#define WRSPATH          "WRS_PATH"
#define WRSROW           "WRS_ROW"
#define ACQDATE          "DATE_ACQUIRED"
#define BANDS            "FILE_NAME_BAND_"
#define SUNAZIMUTH       "SUN_AZIMUTH"
#define SUNELEV          "SUN_ELEVATION"
#define ESUNDIST         "EARTH_SUN_DISTANCE"
#define RADMAX           "RADIANCE_MAXIMUM_BAND_"
#define RADMIN           "RADIANCE_MINIMUM_BAND_"
#define REFLMAX          "REFLECTANCE_MAXIMUM_BAND_"
#define REFLMIN          "REFLECTANCE_MINIMUM_BAND_"
#define QCALMAX          "QUANTIZE_CAL_MAX_BAND_"
#define QCALMIN          "QUANTIZE_CAL_MIN_BAND_"
#define RADMULT          "RADIANCE_MULT_BAND_"
#define RADADD           "RADIANCE_ADD_BAND_"
#define REFLMULT         "REFLECTANCE_MULT_BAND_"
#define REFLADD          "REFLECTANCE_ADD_BAND_"
#define TIRCONSTK1       "K1_CONSTANT_BAND_"
#define TIRCONSTK2       "K2_CONSTANT_BAND_"

/* satir icin buffer boyutu */
#define BUFFSIZE         1024

/* Fonksiyon Tanimlari*/
char *strtok_r(char *str, const char *delim, char **nextp);
char *searchline(char *sstring);
void searchmtl(char *metafile);
void freeMtl(void);
/* MTL.txt dosyasindan parse edilen metaverilerin
 * tutulduğu struct 
 */
typedef struct Metadata {
  double sun_azimuth;
  double sun_elevation;
  double esun_distance;
  double *rad_max;
  double *rad_min;
  double *refl_max;
  double *refl_min;
  double *rad_mult;
  double *rad_add;
  double *refl_mult;
  double *refl_add;
  float *tirconst_k1;
  float *tirconst_k2;
  long *qcal_max;
  long *qcal_min;
  int wrs_path;
  int wrs_row;
  int acq_year;
  int acq_month;
  int acq_day;
  char *datatype;
  char *collection;
  char *spacecraft_id;
  char *sensor_id;
  char **file_names;
} Metadata;

#endif /* MTL_H_ */
