#include "application.h"

Application app;

int main() {

  app.init(1000, 1000);

  app.run();

  app.shutdown();

  return 0;
}
