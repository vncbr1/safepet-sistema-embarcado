#pragma once
#include <math.h>
#include <stdio.h>
#include <string.h>
// Logica pura compartilhada entre firmware e testes em computador.
namespace SafePet {
constexpr double BASE_LAT = -8.0880, BASE_LON = -34.8775;
constexpr double EXIT_METERS = 100.0, RETURN_METERS = 80.0;
// Haversine: distancia aproximada sobre uma Terra esferica, em metros.
inline double distanceMeters(double lat, double lon) {
  constexpr double RAD = 3.14159265358979323846 / 180.0;
  double aLat = (lat - BASE_LAT) * RAD;
  double aLon = (lon - BASE_LON) * RAD;
  double a = sin(aLat/2)*sin(aLat/2) + cos(BASE_LAT*RAD)*cos(lat*RAD)*sin(aLon/2)*sin(aLon/2);
  a = fmax(0.0, fmin(1.0, a));
  return 6371000.0 * 2.0 * atan2(sqrt(a), sqrt(1.0-a));
}
// Histerese: entre 80 e 100 m conserva o estado anterior para reduzir oscilacao.
inline bool nextAlert(bool previous, double meters) {
  return previous ? meters > RETURN_METERS : meters > EXIT_METERS;
}
// Aceita somente "p latitude longitude". Rejeita lixo, NaN, infinito e faixas invalidas.
inline bool parsePosition(const char* line, double& lat, double& lon) {
  if (line[0] != 'p' || line[1] != ' ') return false;
  char extra;
  double a, b;
  if (sscanf(line, "p %lf %lf %c", &a, &b, &extra) != 2) return false;
  if (!isfinite(a) || !isfinite(b) || a < -90 || a > 90 || b < -180 || b > 180) return false;
  lat=a; lon=b; return true;
}
}
