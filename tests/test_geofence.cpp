#include "../firmware/geofence.h"
#include <cassert>
#include <iostream>
int main() {
  using namespace SafePet;
  assert(distanceMeters(BASE_LAT,BASE_LON)<0.001);
  assert(fabs(distanceMeters(BASE_LAT+0.002,BASE_LON)-222.38985)<0.01);
  assert(!nextAlert(false,100)); assert(nextAlert(false,100.01));
  assert(nextAlert(true,90)); assert(!nextAlert(false,90));
  assert(!nextAlert(true,80)); assert(nextAlert(true,80.01));
  double lat=1,lon=2;
  assert(parsePosition("p -8.088 -34.8775",lat,lon));
  assert(lat==BASE_LAT && lon==BASE_LON);
  for (auto s : {"p 91 0","p 0 -181","p nan 0","p inf 0","p 0","p 0 0 lixo","enter","p 0x 0","p0 0"}) {
    assert(!parsePosition(s,lat,lon)); assert(lat==BASE_LAT && lon==BASE_LON);
  }
  assert(parsePosition("p -90 180",lat,lon));
  std::cout<<"PASS: distancia, limites, histerese, parser e preservacao em erro\n";
}
